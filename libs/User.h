#ifndef USER_H
#define USER_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "Errors.h"


#define ADMIN 2
#define CLIENT 3




typedef struct _User* User;

// Constructor
User initUser(void);

// Destructor
User freeUser(User user);

// SETTERS
ERROR_CODE setCorreo(User newUser, const char* newCorreo);
ERROR_CODE setPassword(User user, const char* newPassword);
ERROR_CODE setId(User user, const char* id);
ERROR_CODE setNombre(User user, const char* newNombre);
ERROR_CODE setApellidoPaterno(User user, const char* newApellidoPaterno);
ERROR_CODE setApellidoMaterno(User user, const char* newApellidoMaterno);
ERROR_CODE setCarrera(User user, const char* newCarrera);
ERROR_CODE setSemestre(User user, const char* semestre);
ERROR_CODE setTypeUser(User user, const char* typeUser);
ERROR_CODE setFechaNacimiento(User user, const char* newFechaNacimiento);



// GETTERS
char* getCorreo(User user);
char* getPassword(User user);
int8_t getTypeUser(User user);
char* getNombre(User user);
char* getApellidoPaterno(User user);
char* getApellidoMaterno(User user);
char* getFechaNacimiento(User user);
uint8_t getId(User user);

#endif