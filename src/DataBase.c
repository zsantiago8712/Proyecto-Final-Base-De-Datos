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
    MYSQL_ROW* row;
    uint8_t numRows = 0, numColumns = 0;

    char query[BUFSIZ];
    createLogginQueery(user, query);

    sendQuery(dataBase, query);
    
    res = mysql_use_result(dataBase->connection);
    /* output table name */
    printf("MySQL Tables in mysql database:\n");

   
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
        return USER_NOT_FOUND;
    }

    
    mysql_free_result(res);     
    return ERROR_OK;
}

ERROR_CODE getAllUsers(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW* row = NULL;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;

    if(getUsersByName(dataBase, data) == ERROR_OK)
        res = mysql_use_result(dataBase->connection);

    else if(getUsersById(dataBase, data) == ERROR_OK)
        res = mysql_use_result(dataBase->connection);

    else if(getUserByCarrera(dataBase, data) == ERROR_OK)
        res = mysql_use_result(dataBase->connection);

    else if(getUserByNombreLibro(dataBase, data) == ERROR_OK)
        res  = mysql_use_result(dataBase->connection);
    
    if(res == NULL){
        mysql_free_result(res);
        return EMPTY_SET;
    }


    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data))
            setNumRows(data, numRows);

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    mysql_free_result(res);

    return ERROR_OK;
}


ERROR_CODE searchUser(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW* row = NULL;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;


    
    if(res == NULL){
        mysql_free_result(res);
        return EMPTY_SET;
    }


    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data))
            setNumRows(data, numRows);

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    mysql_free_result(res);

    return ERROR_OK;
}

static ERROR_CODE getUsersByName(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW* row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ];

    createSearchUserByNameQuery(data, query);
    
    if(sendQuery(dataBase, query) == EMPTY_SET)
        return EMPTY_SET;


    res = mysql_use_result(dataBase->connection);
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data))
            setNumRows(data, numRows);

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    mysql_free_result(res);
    return ERROR_OK;
}


static ERROR_CODE getUsersById(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW* row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ];

    createSearchUserByIdQuery(data, query);
    
    if(sendQuery(dataBase, query) == EMPTY_SET)
        return EMPTY_SET;


    res = mysql_use_result(dataBase->connection);
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data))
            setNumRows(data, numRows);

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    mysql_free_result(res);
    return ERROR_OK;
}


static ERROR_CODE getUserByCarrera(DataBase dataBase, Data data){

    MYSQL_RES* res;
    MYSQL_ROW* row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ];

    createSearchUserByCarreraQuery(data, query);
    
    if(sendQuery(dataBase, query) == EMPTY_SET)
        return EMPTY_SET;


    res = mysql_use_result(dataBase->connection);
    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data))
            setNumRows(data, numRows);

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    mysql_free_result(res);
    return ERROR_OK;
}

static ERROR_CODE getUserByNombreLibro(DataBase dataBase, Data data){

    MYSQL_RES* res = NULL;
    MYSQL_ROW* row;
    uint8_t numRows = 0, numColumns = 0, indexRows = 0;
    char query[BUFSIZ];

    createSearchUserByNombreLibro(data, query);
    
    if(sendQuery(dataBase, query) == EMPTY_SET)
        return EMPTY_SET;


    res = mysql_use_result(dataBase->connection);
    if(!res){
        mysql_free_result(res);
        return EMPTY_SET;
    }


    while ((row = mysql_fetch_row(res)) != NULL){
        
        numRows = mysql_num_rows(res);
        numColumns = mysql_num_fields(res);

        if(numRows > getNumRows(data))
            setNumRows(data, numRows);

        setbdData(data, row, indexRows, numColumns);

        printf("ROWS: %d || COLUMNS: %d\n", numRows, numColumns);
        indexRows++;        
    }

    mysql_free_result(res);
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

    char query[BUFSIZ] = "SELECT Id_cuenta, Nombre, Nombre,Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento FROM Usuarios WHERE Nombre = '";
    strcat(query, getArgumentSearch(data));
    strcat(query, "';");

    strcpy(queryToUse, query);
    return queryToUse;
}


static char* createSearchUserByIdQuery(Data data , char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento FROM Usuarios WHERE UserID = ";
    strcat(query, getArgumentSearch(data));
    strcat(query, ";");
    
    strcpy(queryToUse, query);
    return queryToUse;
}


static char* createSearchUserByCarreraQuery(Data data , char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Correo, Semestre, F_nacimiento FROM Usuarios WHERE Carrera = '";
    strcat(query, getArgumentSearch(data));
    strcat(query, "';");
    
    strcpy(queryToUse, query);
    return queryToUse;
}



static char* createSearchUserByNombreLibro(Data data , char* queryToUse){

    char query[BUFSIZ] = "SELECT Id_cuenta, User_type, Nombre, Ap_pat, Ap_mat, Carrera, Semestre, Correo, (AES_DECRYPT(Password, 'AES')) AS Password FROM Usuarios AS USR, Sistema_prestamos AS SP,  Catalogo AS C  WHERE C.Nombre_lb = '";
    strcat(query, getArgumentSearch(data));
    strcat(query, "' AND C.Isbn = SP.Isbn AND SP.Id_cuenta = USR.Id_cuenta;';");
    
    strcpy(queryToUse, query);
    return queryToUse;
}
static ERROR_CODE sendQuery(DataBase dataBase, const char* query){

    if(mysql_query(dataBase->connection, query)){
        fprintf(stderr, "ERROR: %s %d %d-->%d", __FILE__, __LINE__, USER_NOT_FOUND, mysql_errno(dataBase->connection));
        return EMPTY_SET; 
    }

    return ERROR_OK;
}