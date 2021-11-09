#include "../libs/User.h"
#include "../libs/Errors.h"


struct _User
{
    char* nombre;
    char* apellido_paterno;
    char* apellido_materno;
    char* carrera;
    char* correo;
    char* password;
    char* fecha_nacimiento;
    uint8_t semestre;
    uint8_t id;
    uint8_t typeUser;

};


User initUser(void){

    User newUser = NULL;
    newUser = malloc(sizeof(struct _User));

    if(!newUser){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, MEM_ERROR);
        exit(MEM_ERROR);
    }

    newUser->nombre = "\0";
    newUser->apellido_paterno = "\0";
    newUser->apellido_materno = "\0";
    newUser->carrera = "\0";
    newUser->correo = "\0";
    newUser->fecha_nacimiento = "\0";
    newUser->semestre = 0;
    newUser->id = 0;
    newUser->typeUser = CLIENT;


    return newUser;
}


User freeUser(User user){

    puts("user free");
    if(user->nombre[0] != '\0')
        free(user->nombre);

    if(user->carrera[0] != '\0')
        free(user->carrera);

    if(user->correo[0] != '\0')
        free(user->correo);

    if(user->apellido_paterno[0] != '\0')
        free(user->apellido_paterno);
    
    if(user->apellido_materno[0] != '\0')
        free(user->apellido_materno);

    if(user->fecha_nacimiento[0] != '\0')
        free(user->fecha_nacimiento);

    free(user);
    user = NULL;
    
    return user;
}


// SETTERS

ERROR_CODE setCorreo(User user, const char* newCorreo){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->correo = calloc(strlen(newCorreo), sizeof(char));
    user->correo = strdup(newCorreo);
    
    return ERROR_OK;
}


ERROR_CODE setPassword(User user, const char* newPassword){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->password = calloc(strlen(newPassword), sizeof(char));
    user->password = strdup(newPassword);
    
    
    return ERROR_OK;
}

ERROR_CODE setId(User user, const char* id){

    char *prt;
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->id = strtol(id, &prt, 5);    
    return ERROR_OK;
}



ERROR_CODE setNombre(User user, const char* newNombre){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->nombre = calloc(strlen(newNombre), sizeof(char));
    user->nombre = strdup(newNombre);
    
    
    return ERROR_OK;
}


ERROR_CODE setApellidoPaterno(User user, const char* newApellidoPaterno){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->apellido_paterno = calloc(strlen(newApellidoPaterno), sizeof(char));
    user->apellido_paterno = strdup(newApellidoPaterno);
    
    
    return ERROR_OK;
}



ERROR_CODE setApellidoMaterno(User user, const char* newApellidoMaterno){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->apellido_materno = calloc(strlen(newApellidoMaterno), sizeof(char));
    user->apellido_materno = calloc(strlen(newApellidoMaterno), sizeof(char));
    
    return ERROR_OK;
}


ERROR_CODE setCarrera(User user, const char* newCarrera){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->carrera = calloc(strlen(newCarrera), sizeof(char));
    user->carrera = strdup(newCarrera);
    
    return ERROR_OK;
}


ERROR_CODE setSemestre(User user, const char* semestre){

    char *prt;
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->semestre = strtol(semestre, &prt, 5);  

    return ERROR_OK;
}


ERROR_CODE setTypeUser(User user, const char* typeUser){

    char *prt;
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    if(strtol(typeUser, &prt, 5))
        user->typeUser = ADMIN;
    else
        user->typeUser = CLIENT;
        

    return ERROR_OK;
}


ERROR_CODE setFechaNacimiento(User user, const char* newFechaNacimiento){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        return EMPTY_STRUCT;
    }

    user->fecha_nacimiento = calloc(strlen(newFechaNacimiento), sizeof(char));
    user->fecha_nacimiento = strdup(newFechaNacimiento);
    
    return ERROR_OK;
}



// GETTERS
char* getCorreo(User user){
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->correo;
}


char* getPassword(User user){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->password;
}

int8_t getTypeUser(User user){

    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->typeUser;
}


char* getNombre(User user){
    
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->nombre;
}

char* getApellidoPaterno(User user){
    
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->apellido_paterno;
}


char* getApellidoMaterno(User user){
    
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->apellido_paterno;
}

char* getFechaNacimiento(User user){
    
    if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->fecha_nacimiento;
}

uint8_t getId(User user){

     if(!user){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    }

    return user->id;
}