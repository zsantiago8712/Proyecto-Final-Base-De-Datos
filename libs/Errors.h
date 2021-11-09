#ifndef ERRORS_H
#define ERRORS_H


#include <stdlib.h>
#include <stdio.h>



typedef enum{

    MEM_ERROR,
    ERROR_OK,
    EMPTY_STRUCT,
    USER_NOT_FOUND,
    BOOK_NOT_FOUND,
    PASSWORD_ERROR,
    ERROR_CONNECTION_DB,
    EMPTY_FIELDS,
    INVALID_EMAIL,
    EMPTY_SET,
    ERROR_QUERY

}ERROR_CODE;


#endif