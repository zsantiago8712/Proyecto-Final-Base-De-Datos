#include "../libs/Memory.h"


static ERROR_CODE checkMatrixStringAlloc(char ***matrixString);
static ERROR_CODE checkListStringAlloc(char **listStirng);
static ERROR_CODE checkColumnAlloc(char* columnData);

char*** createMatrixString(char*** matrixString, uint16_t rows, uint16_t columns){
    
    matrixString = calloc(rows, sizeof(char**));
    checkMatrixStringAlloc(matrixString);
    for(size_t i = 0; i < columns; i++){
        matrixString[i] = calloc(columns, sizeof(matrixString[i]));
        checkListStringAlloc(matrixString[i]);
    }

    return matrixString;
}


char*** freeMatrixString(char*** matrixString, uint16_t rows){

    for(int i = 0; i < rows; i++){
        free(matrixString[i]);
        matrixString[i] = NULL;
    }

    free(matrixString);
    matrixString = NULL;


    return matrixString;
}


char** setMoreColumns(char** listString, size_t columns){
    
    listString = realloc(listString, sizeof(char*) * columns * 2);
    checkListStringAlloc(listString);

    return listString;
}



char*** setMoreRows(char*** matrixString, size_t rows, size_t columns){

	char*** tempPtr;
	tempPtr = realloc(matrixString, sizeof(matrixString) * rows * 2);
    checkMatrixStringAlloc(tempPtr);

	for(size_t i = 0; i < rows * 2; i++){
		tempPtr[rows + i] = createListString(tempPtr[rows + i], columns);
		checkListStringAlloc(tempPtr[rows + i]);
	}

	matrixString = tempPtr;

    return matrixString;
}

char* reallocStringSize(char* columnData, char* word){
    
    columnData = realloc(columnData, sizeof(char) * strlen(word));
    checkColumnAlloc(columnData);
    
    return columnData;
}


char** createListString(char** listStirng, size_t columns){
    
    listStirng = calloc(columns, sizeof(char*));
    checkListStringAlloc(listStirng);


    return listStirng;
}

static ERROR_CODE checkMatrixStringAlloc(char ***matrixString){

    if(matrixString == NULL){
        perror("ERROR::");
        exit(MEM_ERROR);
    }

    return ERROR_OK;
}


static ERROR_CODE checkListStringAlloc(char **listStirng){

    if(listStirng == NULL){
        perror("ERROR::");
        exit(MEM_ERROR);
    }
    return ERROR_OK;
}


static ERROR_CODE checkColumnAlloc(char* columnData){
    
    if(columnData == NULL){
        perror("ERROR::");
        exit(MEM_ERROR);
    }
    return ERROR_OK;
}