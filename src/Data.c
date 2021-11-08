#include "../libs/Data.h"


struct _Data{

    char*** bdData;
    char* argumentSearch;
    uint8_t rows, columns;

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
    newData->bdData = createMatrixString(newData->bdData, 10, 10);
    newData->argumentSearch = "\0";
    return newData;
}



Data freeData(Data data){

    puts("free Data");
    data->bdData = freeMatrixString(data->bdData, data->rows);

    if(data->argumentSearch[0] != '\0')
        free(data->argumentSearch);
    

        
    free(data);
    data = NULL;

    return data;
}


// SETTERS

ERROR_CODE setbdData(Data data, MYSQL_ROW newData, uint8_t indexRows, uint8_t numColumns){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    if(numColumns > data->columns)
        setNumColumns(data, numColumns);

    for(int i = 0; i < numColumns; i++){
        reallocStringSize(data->bdData[indexRows][i], newData[i]);
        strcpy(data->bdData[indexRows][i], newData[i]);
    }

    return ERROR_OK;
}

ERROR_CODE setNumRows(Data data, uint8_t rows){

    uint8_t temp = data->rows;

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }


    while(temp < rows){

        temp * 2;
        temp++;
    }

    data->rows = temp;

    return ERROR_OK;
}



ERROR_CODE setNumColumns(Data data, uint8_t columns){

    uint8_t temp = data->columns;

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    while(temp < columns){

        temp * 2;
        temp++;
    }

    data->columns = temp;

    return ERROR_OK;
}

ERROR_CODE setArgumentSearch(Data data, char* newArgumentSearch){

    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    data->argumentSearch = reallocStringSize(data->argumentSearch, newArgumentSearch);
    strcpy(data->argumentSearch, newArgumentSearch);

    return ERROR_OK;
}


// Getters
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

char* getArgumentSearch(Data data){
    if(!data){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return data->argumentSearch;
}