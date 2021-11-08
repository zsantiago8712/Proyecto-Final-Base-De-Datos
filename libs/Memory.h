#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Errors.h"



char*** createMatrixString(char*** matrixString, uint16_t rows, uint16_t columns);
char*** freeMatrixString(char*** matrixString, uint16_t rows);
char** setMoreColumns(char** listString, size_t columns);
char*** setMoreRows(char*** matrixString, size_t rows, size_t columns);
char* reallocStringSize(char* columnData, char* word);
char** createListString(char** listStirng, size_t columns);



#endif