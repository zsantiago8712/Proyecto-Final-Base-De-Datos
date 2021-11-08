#ifndef DATA_H
#define DATA_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <mysql/mysql.h>
#include "Errors.h"
#include "Memory.h"

typedef struct _Data* Data;

// Constructor
Data initData(void);

// Destructor
Data freeData(Data data);


// Setters
ERROR_CODE setbdData(Data data, MYSQL_ROW newData, uint8_t indexRows, uint8_t numColumns);
ERROR_CODE setNumRows(Data data, uint8_t rows);
ERROR_CODE setNumColumns(Data data, uint8_t columns);
ERROR_CODE setArgumentSearch(Data data, char* newArgumentSearch);


// Getters
uint8_t getNumRows(Data data);
uint8_t getNumColumns(Data data);
char* getArgumentSearch(Data data);

#endif