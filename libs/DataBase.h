#ifndef DATABASE_H
#define DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include "User.h"
#include "Data.h"
#include <gtk/gtk.h>


typedef struct _DataBase* DataBase;

// Constructor
DataBase initDataBase(void);

// Destructor
DataBase freeDataBase(DataBase database);

ERROR_CODE login(DataBase dataBase, User user);
ERROR_CODE getAllUsers(DataBase dataBase, Data data);
ERROR_CODE searchUser(DataBase dataBase, Data data);
ERROR_CODE addUserToDB(DataBase dataBase, Data data);
ERROR_CODE getAllLibros(DataBase dataBase, Data data);
ERROR_CODE searchLibro(DataBase dataBase, Data data);
ERROR_CODE addRenta(DataBase dataBase, Data data, User user);
ERROR_CODE devoluciones(DataBase dataBase, Data data, User user);
ERROR_CODE getAllLibrosRentados(DataBase dataBase, Data data, User user);

#endif