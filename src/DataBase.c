#include "../libs/DataBase.h"
#include "../libs/Errors.h"


static MYSQL* connectionDataBase(DataBase dataBase);
static char* createLogginQueery(User user, char* queryToUse);
static ERROR_CODE sendQuery(DataBase dataBase, const char* query);
static ERROR_CODE getUsersByName(DataBase dataBase, Data data);
static ERROR_CODE getUserByNombreLibro(DataBase dataBase, Data data);
static ERROR_CODE getUserByCarrera(DataBase dataBase, Data data);
static ERROR_CODE getUsersById(DataBase dataBase, Data data);
static char* createSearchUserByNameQuery(Data data, char* queryToUse);
static char* createSearchUserByIdQuery(Data data , char* queryToUse);
static char* createSearchUserByCarreraQuery(Data data , char* queryToUse);
static char* createSearchUserByNombreLibro(Data data , char* queryToUse);
static char* createAddUserQuery(Data data, char* queryToUse);
static char* createSearchLibroByNameQuery(Data data, char* queryToUse);
static char* createSearchLibroByISBNQuery(Data data, char* queryToUse);
static char* createSearchLibroByEditorialQuery(Data data, char* queryToUse);
static char* createSearchLibroByNumEjemplaresQuery(Data data, char* queryToUse);
static ERROR_CODE getLibroByName(DataBase dataBase, Data data);
static ERROR_CODE getLibrosByIsbn(DataBase dataBase, Data data);
static ERROR_CODE getLibrosByEditorial(DataBase dataBase, Data data);
static ERROR_CODE getLibrosByNumEjemplares(DataBase dataBase, Data data);
static char* createRentQuery(Data data, User user, char* queryToUse);
static char* createDevolucionesQuery(Data data, User user, char* queryToUse);
static char* creategetAllLibrosRentadosQuery(User user, char* queryToUse);


struct _DataBase{
    char* server;
    char* user;
    char* password;
    char* dataBaseName;
    MYSQL* connection;
};


DataBase initDataBase(void){
    
    DataBase newDB = NULL;
    newDB = malloc(sizeof(struct _DataBase));

    if(!newDB){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, MEM_ERROR);
        exit(MEM_ERROR);
    }
       

    newDB->server = "localhost";
    newDB->dataBaseName = "ic20szs";
    newDB->password = "217924";
    newDB->user = "ic20szs";
    newDB->connection = connectionDataBase(newDB);


    return newDB;
}


DataBase freeDataBase(DataBase database){

    puts("database free");

    mysql_close(database->connection);
    free(database);
    database = NULL;

    return database;
}

static MYSQL* connectionDataBase(DataBase dataBase){

    MYSQL* connection;
    
    connection = mysql_init(NULL);
    if(!mysql_real_connect(connection, dataBase->server, dataBase->user, dataBase->password, dataBase->dataBaseName,0, NULL, 0)){
        
        fprintf(stderr, "ERROR: %s %d %d-->%d", __FILE__, __LINE__, ERROR_CONNECTION_DB, mysql_errno(connection));
        exit(ERROR_CONNECTION_DB);    
    }

    puts("Connection SUCCES!!");
    return connection;
}


ERROR_CODE login(DataBase dataBase, User user){

    MYSQL_RES* res;
    MYSQL_ROW row;
    
    uint8_t isData = 0;
    char query[BUFSIZ];
    createLogginQueery(user, query);

    if(sendQuery(dataBase, query) == ERROR_QUERY){
        puts("USER NOT FOUND");
        return EMPTY_SET;
    }
       
    
    res = mysql_use_result(dataBase->connection);
    
   
    
    while((row = mysql_fetch_row(res))!= NULL){
        
        isData = 1;
        setId(user, row[0]);
        setTypeUser(user, row[1]);
        setNombre(user, row[2]);
        setApellidoPaterno(user, row[3]);
      
        
        if(!row[4])
            setApellidoMaterno(user, "null");
        
        else
            setApellidoMaterno(user, row[4]);
        
        if(!row[5])
            setCarrera(user, "null");
        else
            setSemestre(user, row[5]);
      
        if(!row[6])
            setSemestre(user, "0");
        else
            setSemestre(user, row[6]);
        setCorreo(user, row[7]);
        setFechaNacimiento(user, row[8]);
        setPassword(user, row[9]);

    }
    
    while(mysql_next_result(dataBase->connection) == 0)
        
    
    mysql_free_result(res);    

    if(!isData)
        return EMPTY_SET; 
    return ERROR_OK;
}


ERROR_CODE getAllUsers(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ] = "CALL getAllUsers();";

    
    sendQuery(dataBase, query);
    
    res = mysql_use_result(dataBase->connection);
    
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows >= getNumRows(data) - 1)
            reallocBdDataRows(data);
        
            
       
        setbdData(data, row, indexRows, numColumns);
        
        indexRows++;        
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)

    mysql_free_result(res);
    
    
    return ERROR_OK;
}


ERROR_CODE getAllLibros(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ] = "CALL getAllLibros();";

    
    if(sendQuery(dataBase, query) == EMPTY_SET)
        return EMPTY_SET;
    
    res = mysql_use_result(dataBase->connection);
    
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows >= getNumRows(data) - 1)
            reallocBdDataRows(data);
        
            
       
        setbdData(data, row, indexRows, numColumns);
        indexRows++;        
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);
    
    return ERROR_OK;

}

ERROR_CODE getAllLibrosRentados(DataBase dataBase, Data data, User user){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ];

    creategetAllLibrosRentadosQuery(user, query);
    
    if(sendQuery(dataBase, query) == EMPTY_SET)
        return EMPTY_SET;
    
    res = mysql_use_result(dataBase->connection);
    
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows >= getNumRows(data) - 1)
            reallocBdDataRows(data);
        
            
       
        setbdData(data, row, indexRows, numColumns);
        indexRows++;        
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)

    mysql_free_result(res);
    
    return ERROR_OK;
}

ERROR_CODE searchUser(DataBase dataBase, Data data){


    if(getUsersByName(dataBase, data) == ERROR_OK){
        puts("FOUND BY NOMBRE");
        return ERROR_OK;
    }
    else if(getUsersById(dataBase, data) == ERROR_OK){
        puts("FOUND BY ID");
        return ERROR_OK;
    }
    else if(getUserByCarrera(dataBase, data) == ERROR_OK){
        puts("FOUND BY CARRERA");
        return ERROR_OK;
    }
    else if(getUserByNombreLibro(dataBase, data) == ERROR_OK){
        puts("FOUND BY NOMBRE LIBRO");
        return ERROR_OK;
    }
        
    return EMPTY_SET;   
}



ERROR_CODE searchLibro(DataBase dataBase, Data data){

    if(getLibroByName(dataBase, data) == ERROR_OK){
        puts("FOUND BY NOMBRE");
        return ERROR_OK;
    }
    else if(getLibrosByIsbn(dataBase, data) == ERROR_OK){
        puts("FOUND BY ISBN");
        return ERROR_OK;
    }
    else if(getLibrosByEditorial(dataBase, data) == ERROR_OK){
        puts("FOUND BY EDITORIAL");
        return ERROR_OK;
    }
    else if(getLibrosByNumEjemplares(dataBase, data) == ERROR_OK){
        puts("FOUND BY NUMEOR EJEMPLARES");
        return ERROR_OK;
    }
        
    return EMPTY_SET; 

}


ERROR_CODE addUserToDB(DataBase dataBase, Data data){

    char query[BUFSIZ];
    createAddUserQuery(data, query);

    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;

    return ERROR_OK;
}


ERROR_CODE addRenta(DataBase dataBase, Data data, User user){

    char query[BUFSIZ];
    createRentQuery(data, user, query);

    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;
    
    puts("Renta SUCCESSFULLY");
    return ERROR_OK;
}


ERROR_CODE devoluciones(DataBase dataBase, Data data, User user){

    char query[BUFSIZ];
    createDevolucionesQuery(data, user, query);

    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;
    
    puts("Devolucino SUCCESSFULLY");
    return ERROR_OK;

}

// SATICS
static ERROR_CODE getUsersByName(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchUserByNameQuery(data, query);
    
    
    if(sendQuery(dataBase, query) == ERROR_QUERY){
        return ERROR_QUERY;
    }
       


    res = mysql_use_result(dataBase->connection);

    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        }

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);


        if(numRows > getNumRows(data)){
            
            reallocBdDataRows(data);
        }
            

        setbdData(data, row, indexRows, numColumns);

        indexRows++; 
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);

    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;
}


static ERROR_CODE getUsersById(DataBase dataBase, Data data){

    MYSQL_RES* res = NULL;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchUserByIdQuery(data, query);
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;
    
    
    res = mysql_use_result(dataBase->connection);


    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        } 

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data)){
            
            reallocBdDataRows(data);
        }

        setbdData(data, row, indexRows, numColumns);

        indexRows++;        
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);

    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;
}


static ERROR_CODE getUserByCarrera(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchUserByCarreraQuery(data, query);
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;

    res = mysql_use_result(dataBase->connection);
   

    while ((row = mysql_fetch_row(res)) != NULL){
        
         if(!isData){
            clearData(data);
            isData = TRUE;
        }
           
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data)){
            reallocBdDataRows(data);
        }

        setbdData(data, row, indexRows, numColumns);
        indexRows++;        
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);

    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;
}

static ERROR_CODE getUserByNombreLibro(DataBase dataBase, Data data){

    
    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchUserByNombreLibro(data, query);
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;

    
    

    res = mysql_use_result(dataBase->connection);


    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        }

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data)){
            reallocBdDataRows(data);
        }

        setbdData(data, row, indexRows, numColumns);

        indexRows++;        
    }

    setDataRows(data, numRows);
    while (mysql_next_result(dataBase->connection) == 0)

    mysql_free_result(res);
    if(!isData)
        return EMPTY_SET;

    return ERROR_OK;
}



static ERROR_CODE getLibroByName(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchLibroByNameQuery(data, query);
    
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;


    res = mysql_use_result(dataBase->connection);

    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        }

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);


        if(numRows >= getNumRows(data)){
            reallocBdDataRows(data);
        }
            

        setbdData(data, row, indexRows, numColumns);

        indexRows++; 
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);
    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;

}


static ERROR_CODE getLibrosByIsbn(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchLibroByISBNQuery(data, query);
    
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;


    res = mysql_use_result(dataBase->connection);

    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        }

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);


        if(numRows > getNumRows(data)){
            
            reallocBdDataRows(data);
        }
            

        setbdData(data, row, indexRows, numColumns);

        indexRows++; 
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);
    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;

}



static ERROR_CODE getLibrosByEditorial(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchLibroByEditorialQuery(data, query);
    
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;


    res = mysql_use_result(dataBase->connection);

    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        }

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);


        if(numRows > getNumRows(data)){
            
            reallocBdDataRows(data);
        }
            

        setbdData(data, row, indexRows, numColumns);

        indexRows++; 
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);
    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;

}


static ERROR_CODE getLibrosByNumEjemplares(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchLibroByNumEjemplaresQuery(data, query);
    
    
    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;


    res = mysql_use_result(dataBase->connection);

    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        }

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);


        if(numRows > getNumRows(data)){
            
            reallocBdDataRows(data);
        }
            

        setbdData(data, row, indexRows, numColumns);

        indexRows++; 
    }

    setDataRows(data, numRows);

    while (mysql_next_result(dataBase->connection) == 0)
    mysql_free_result(res);
    
    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;

}



//QUERYS
static char* createLogginQueery(User user, char* queryToUse){

    char query[BUFSIZ];

    sprintf(query, "CALL login('%s', '%s');", getCorreo(user), getPassword(user));
    strcpy(queryToUse, query);

    return queryToUse;
}



static char* createSearchUserByNameQuery(Data data, char* queryToUse){

    char query[BUFSIZ];

    sprintf(query, "CALL searchUserByNombre('%s')", getArgumentSearch(data));
    strcpy(queryToUse, query);

    return queryToUse;
}


static char* createSearchUserByIdQuery(Data data , char* queryToUse){

    char query[BUFSIZ];

    sprintf(query, "CALL searchUserByIdCuenta(%s);", getArgumentSearch(data));
    strcpy(queryToUse, query);

    return queryToUse;
}


static char* createSearchUserByCarreraQuery(Data data , char* queryToUse){

    char query[BUFSIZ];

    sprintf(query, "CALL searchUserByCarrera('%s');", getArgumentSearch(data));
    strcpy(queryToUse, query);

    return queryToUse;
}



static char* createSearchUserByNombreLibro(Data data , char* queryToUse){

    char query[BUFSIZ];

    sprintf(query, "CALL searchUserByNombreLib('%s');", getArgumentSearch(data));
    strcpy(queryToUse, query);

    return queryToUse;
}


static ERROR_CODE sendQuery(DataBase dataBase, const char* query){

    if(mysql_query(dataBase->connection, query)){
        fprintf(stderr, "ERROR: %s %d %d --> %d\n", __FILE__, __LINE__, USER_NOT_FOUND, mysql_errno(dataBase->connection));
        return ERROR_QUERY; 
    }

    return ERROR_OK;
}


static char* createAddUserQuery(Data data, char* queryToUse){

    char query[BUFSIZ];

    sprintf(query, "CALL addNewUser(%s, '%s', '%s', '%s', '%s', %s, '%s', '%s', '%s');", getArgumentInsert(data, 0), getArgumentInsert(data, 1), getArgumentInsert(data, 2), getArgumentInsert(data, 3), getArgumentInsert(data, 4), getArgumentInsert(data, 5), getArgumentInsert(data, 6), getArgumentInsert(data, 7), getArgumentInsert(data, 8));

    strcpy(queryToUse, query);
    return strdup(query);
}



static char* createSearchLibroByNameQuery(Data data, char* queryToUse){
    char query[BUFSIZ];
    sprintf(query, "CALL getLibroByName('%s');", getArgumentSearch(data));

    strcpy(queryToUse, query);

    return queryToUse;
}


static char* createSearchLibroByISBNQuery(Data data, char* queryToUse){
    char query[BUFSIZ];
    sprintf(query, "CALL getLibroByIsbn(%s);", getArgumentSearch(data));

    strcpy(queryToUse, query);

    return queryToUse;
}

static char* createSearchLibroByEditorialQuery(Data data, char* queryToUse){
    char query[BUFSIZ];
    sprintf(query, "CALL getLibroByEditorial('%s');", getArgumentSearch(data));

    strcpy(queryToUse, query);
    return queryToUse;
}

static char* createSearchLibroByNumEjemplaresQuery(Data data, char* queryToUse){
    char query[BUFSIZ];
    sprintf(query, "CALL getLibroByNumeroEjemplares(%s);", getArgumentSearch(data));

    strcpy(queryToUse, query);
    return queryToUse;
}


static char* createRentQuery(Data data, User user, char* queryToUse){

    char query[BUFSIZ];
    sprintf(query, "CALL rentaLibro(%d, %s);", getId(user), getArgumentInsert(data, 0));

    strcpy(queryToUse, query);
    return queryToUse;
}

static char* creategetAllLibrosRentadosQuery(User user, char* queryToUse){

    char query[BUFSIZ];
    sprintf(query, "CALL getLibrosPrestados(%d);", getId(user));

    strcpy(queryToUse, query);
    return queryToUse;
}

static char* createDevolucionesQuery(Data data, User user, char* queryToUse){

    char query[BUFSIZ];
    sprintf(query, "CALL regresarLibro(%s, %d, %s);", getArgumentInsert(data, 0), getId(user), getArgumentInsert(data, 1));

    strcpy(queryToUse, query);
    return queryToUse;

}