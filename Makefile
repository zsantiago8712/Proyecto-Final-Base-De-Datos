CC = gcc
CXXFLAGS = -std=c11 -Wall -g -D_GNU_SOURCE
LDFLAGS = `mysql_config --cflags --libs `
INCLUDES = -I/usr/include/mysql

MKDIR_P = mkdir -p

APPNAME = myapp.out
SRCDIR = src
OBJDIR = obj

SRC = $(wildcard $(SRCDIR)/*.c)
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

RM = rm
DELOBJ = $(OBJ)

all: $(APPNAME) 


# Builds the app
$(APPNAME): $(OBJ) 
	$(CC) $(INCLUDES) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) `pkg-config --libs gtk+-2.0`


# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%.c | obj
	$(CC) $(INCLUDES) $(CXXFLAGS) -o $@ -c $< $(LDFLAGS) `pkg-config --cflags gtk+-2.0`

#Si no existe el directirio para los .o lo crea
obj:
	${MKDIR_P} ${OBJDIR}


.PHONY: clean
clean:
	$(RM) $(DELOBJ) $(APPNAME)
