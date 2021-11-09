#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include "Errors.h"
#include "User.h"
#include "DataBase.h"
#include "Data.h"
#include <gtk/gtk.h>


#define PASS_MAX 18

typedef struct _Library* Library;


typedef enum{

    LOGIN,
    MENU_PRINCIPAL,
    EDIT_USERS,
    RENTA_LIBROS,
    UPDATE,
    SALIR
}WINDOWS;



// Constructor
Library initLibrary(void);

//Destructor
Library freeLibrary(Library library);

// VENTANAS -- INTERFACES
ERROR_CODE windowLoggin(Library library);
ERROR_CODE windowMenuPrincipal(Library library);
ERROR_CODE getNextWindow(Library library, WINDOWS nextWindow);
ERROR_CODE editUsersWindow(Library library);
#endif