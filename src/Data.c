#include "../libs/Data.h"


struct _Data{

    char*** bdData;
    char* argumentSearch;
    uint8_t rows, columns, rowsData, columnInsert;
    char** argumentsInsert;
};


Data initData(void){

    Data newData = NULL;
    newData = malloc(sizeof(struct _Data));
    if(!newData){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, MEM_ERROR);
        exit(MEM_ERROR); 
    }

    newData->rows = 10;
    newData->columns = 10;
    newData->rowsData = 0;
    newData->bdData = createMatrixString(newData->bdData, newData->rows, newData->columns);
    newData->columnInsert = 10;
    newData->argumentsInsert = createListString(newData->argumentsInsert, newData->columnInsert);
    newData->argumentSearch = "\0";
    return newData;
}



Data freeData(Data data){

    puts("free Data");
    data->bdData = freeMatrixString(data->bdData, data->rows);
    data->argumentsInsert = freeListString(data->argumentsInsert);


    if(data->argumentSearch[0] != '\0')
        free(data->argumentSearch);
         
    
    free(data);
    data = NULL;

    return data;
}




ERROR_CODE clearData(Data data){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    data->bdData = freeMatrixString(data->bdData, data->rows);
    data->rows = 10;
    data->columns = 10;
    data->rowsData = 0;

    if(data->bdData == NULL)
        data->bdData = createMatrixString(data->bdData, data->rows, data->columns);
    
       
    if(data->bdData != NULL)
        return ERROR_OK;
    
    fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, MEM_ERROR);
    exit(MEM_ERROR);
}



ERROR_CODE clearDataInsert(Data data){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    data->argumentsInsert = freeListString(data->argumentsInsert);
    data->columnInsert = 10;
    data->argumentsInsert = createListString(data->argumentsInsert, data->columnInsert);
    return ERROR_OK;
}


ERROR_CODE reallocBdDataColumns(Data data){

    for(uint8_t i = 0; i < data->rows; i++){
        data->bdData[i] = setMoreColumns(data->bdData[i], data->columns);
    }
    
    return ERROR_OK;
}

ERROR_CODE reallocBdDataRows(Data data){

    data->bdData = setMoreRows(data->bdData, data->rows, data->columns);
    data->rows *= 2;
    return ERROR_OK;
}
// SETTERS

ERROR_CODE setbdData(Data data, char** newData, uint8_t indexRows, uint8_t numColumns){

    
    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    /*if(numColumns >= data->columns){
        setNumColumns(data, numColumns);
        reallocBdDataColumns(data);
    }*/
    

    for(int i = 0; i < numColumns; i++){

        if(!newData[i]){
            data->bdData[indexRows][i] = calloc(strlen("null"), sizeof(char));
            data->bdData[indexRows][i] = strdup("null");
        }else{
          
            data->bdData[indexRows][i] = calloc(strlen(newData[i]), sizeof(char));
            data->bdData[indexRows][i] = strdup(newData[i]);
            printf("%s || ", data->bdData[indexRows][i]);
        }
            
        
    }

    return ERROR_OK;
}


ERROR_CODE setNumRows(Data data, uint8_t rows){


    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    
    while(data->rows <= rows)
        data->rows *= 2;
        

    return ERROR_OK;
}

ERROR_CODE setDataRows(Data data, uint8_t newDataRows){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    data->rowsData = newDataRows;

    return ERROR_OK;
}


ERROR_CODE setNumColumns(Data data, uint8_t columns){


    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

   while(data->columns <= columns)
        data->columns *= 2;

  
    return ERROR_OK;
}

ERROR_CODE setArgumentSearch(Data data, const char* newArgumentSearch){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }
    
    data->argumentSearch = calloc(strlen(newArgumentSearch), sizeof(char));
    data->argumentSearch = strdup(newArgumentSearch);
    

    return ERROR_OK;
}


ERROR_CODE setArgumentInsert(Data data, const char* newArgumentInsert, uint8_t index){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    data->argumentsInsert[index] = calloc(strlen(newArgumentInsert), sizeof(char));
    data->argumentsInsert[index] = strdup(newArgumentInsert);

    printf("NEW DATA INSERT : %s", data->argumentsInsert[index]);
    return ERROR_OK;
}


// Getters

char** getRowDataBd(Data data, uint8_t index){
    
    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->bdData[index];
}


char* getColumnDataBd(Data data, uint8_t indexRow, uint8_t indexColumn){
    
    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->bdData[indexRow][indexColumn];
}


uint8_t getNumRows(Data data){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->rows;
}


uint8_t getNumColumns(Data data){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->columns;
}


uint8_t getDataRows(Data data){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->rowsData;
}

char* getArgumentSearch(Data data){
    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->argumentSearch;
}

char* getArgumentInsert(Data data, uint8_t index){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->argumentsInsert[index];
}

