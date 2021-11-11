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
    

    char query[BUFSIZ];
    createLogginQueery(user, query);

    sendQuery(dataBase, query);
    
    res = mysql_use_result(dataBase->connection);
    
   
    row = mysql_fetch_row(res);
    if(row != NULL){
        
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

    }else{
        printf("ERROR: %d USER NOT FOUND\n", USER_NOT_FOUND);
        mysql_free_result(res);
        return EMPTY_SET;
    }

    
    mysql_free_result(res);     
    return ERROR_OK;
}


ERROR_CODE getAllUsers(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Semestre, Correo, F_nacimiento, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios;";

    
    sendQuery(dataBase, query);
    
    res = mysql_use_result(dataBase->connection);
    
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);
        printf("\nROWS: %d || COLUMNS: %d\n", numRows, numColumns);

        //printf("ROWS --> %d\n", getNumRows(data));
        if(numRows >= getNumRows(data) - 1){
            
            //setNumRows(data, numRows);
            //printf("NEW ROWS --> %d\n", getNumRows(data));
            reallocBdDataRows(data);
        }
            
       
        setbdData(data, row, indexRows, 10);
        
        indexRows++;        
    }

    setDataRows(data, numRows);
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



ERROR_CODE addUserToDB(DataBase dataBase, Data data){

    char query[BUFSIZ];
    createAddUserQuery(data, query);

    if(sendQuery(dataBase, query) == ERROR_QUERY)
        return ERROR_QUERY;

    printf("ADD USER SUCCESSFULLY\n");
    return ERROR_OK;
}


// SATICS
static ERROR_CODE getUsersByName(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchUserByNameQuery(data, query);
    
    
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
            setNumRows(data, numRows);
            reallocBdDataRows(data);
        }
            

        setbdData(data, row, indexRows, 10);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++; 
    }

    setDataRows(data, numRows);
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

    puts("SEARCH BY ID");
    
    
    res = mysql_use_result(dataBase->connection);


    while ((row = mysql_fetch_row(res)) != NULL){
        
        if(!isData){
            clearData(data);
            isData = TRUE;
        } 

        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data)){
            setNumRows(data, numRows);
            reallocBdDataRows(data);
        }

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    setDataRows(data, numRows);
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
            setNumRows(data, numRows);
            reallocBdDataRows(data);
        }

        setbdData(data, row, indexRows, numColumns);
        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    setDataRows(data, numRows);
    mysql_free_result(res);

    if(!isData)
        return EMPTY_SET;

    
    return ERROR_OK;
}

static ERROR_CODE getUserByNombreLibro(DataBase dataBase, Data data){

    
    MYSQL_RES* res = NULL;
    MYSQL_ROW row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0, isData = 0;
    char query[BUFSIZ];

    createSearchUserByNombreLibro(data, query);
    
    puts(query);
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
            setNumRows(data, numRows);
            reallocBdDataRows(data);
        }

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    setDataRows(data, numRows);
    mysql_free_result(res);
    if(!isData)
        return EMPTY_SET;

    return ERROR_OK;
}

static char* createLogginQueery(User user, char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre,Ap_pat, Ap_mat, Carrera, Semestre, Correo, F_nacimiento, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios WHERE Correo='";

    strcat(query, getCorreo(user));
    strcat(query, "' AND (AES_DECRYPT(Password, 'AES'))='");
    strcat(query, getPassword(user));
    strcat(query, "';");

    printf("QUERY: %s\n", query);
    strcpy(queryToUse, query);
    return queryToUse;
}



static char* createSearchUserByNameQuery(Data data, char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios WHERE Nombre = '";
    strcat(query, getArgumentSearch(data));
    strcat(query, "';");

    strcpy(queryToUse, query);
    puts(queryToUse);
    return queryToUse;
}


static char* createSearchUserByIdQuery(Data data , char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios WHERE Id_cuenta = ";
    strcat(query, getArgumentSearch(data));
    strcat(query, ";");
    
    
    strcpy(queryToUse, query);
    puts(queryToUse);
    return queryToUse;
}


static char* createSearchUserByCarreraQuery(Data data , char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios WHERE Carrera = '";
    strcat(query, getArgumentSearch(data));
    strcat(query, "';");
    
    strcpy(queryToUse, query);
    puts(queryToUse);
    return queryToUse;
}



static char* createSearchUserByNombreLibro(Data data , char* queryToUse){

    char query[BUFSIZ] = "SELECT USR.Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios AS USR, Sistema_prestamos AS SP,  Catalogo AS C  WHERE C.Nombre_lb = '";
    strcat(query, getArgumentSearch(data));
    strcat(query, "' AND C.Isbn = SP.Isbn AND SP.Id_cuenta = USR.Id_cuenta;");
    
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

    sprintf(query, "INSERT INTO Usuarios(User_type, Nombre, Ap_pat, Ap_mat, Carrera, Semestre, Correo,  Password, F_nacimiento) VALUES(%s, '%s', '%s', '%s', '%s', %s, '%s', AES_ENCRYPT('%s', 'AES'), '%s');", getArgumentInsert(data, 0), getArgumentInsert(data, 1), getArgumentInsert(data, 2), getArgumentInsert(data, 3), getArgumentInsert(data, 4), getArgumentInsert(data, 5), getArgumentInsert(data, 6), getArgumentInsert(data, 7), getArgumentInsert(data, 8));

    printf("\nQUERY:\n %s\n", query);

    strcpy(queryToUse, query);
    return strdup(query);
}