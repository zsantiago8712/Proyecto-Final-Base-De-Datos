#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Errors.h"





//CONSTRUCTORS
char*** createMatrixString(char*** matrixString, uint8_t rows, uint8_t columns);
char** createListString(char** listStirng, uint8_t columns);


//FREES
char*** freeMatrixString(char*** matrixString, uint8_t rows);
char** freeListString(char** listStirng);
char* freeString(char* string);


//SETTERS
char*** setMoreRows(char*** matrixString, uint8_t rows, uint8_t columns);
char** setMoreColumns(char** listString, uint8_t columns);
char* setSizeColumn(char* columnData, char* word);
char* setResizeColumn(char* columnData, char* word);


#endif 

