#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "../libs/Library.h"



gint main(gint argc, gchar* argv[]){

   
    gtk_init(&argc, &argv);

    Library library = NULL;
    library = initLibrary();
    getNextWindow(library, LOGIN);

    gtk_main();
}