#include "../libs/Library.h"


static ERROR_CODE validateLogin(GtkButton *boton, gpointer userData);
static ERROR_CODE dialogWarning(int8_t error);
static ERROR_CODE validateEmail(char* correo);
static ERROR_CODE validateEmptyFileds(char* correo, char* password);
static gboolean delete_event_handler(GtkWidget *widget, GdkEvent *event, gpointer userData);
static void cerrar(GtkWidget *widget, gpointer userData);
static ERROR_CODE chanegeWindow(GtkButton *boton, gpointer userData);
static void init_list(GtkWidget *list);
static void add_to_list(GtkWidget *list, gchar **str);
static void on_changed(GtkWidget *widget, gpointer label);
static ERROR_CODE searchUserAction(GtkButton *boton, gpointer userData);

struct _Library{

    //STRCUTS
    User user;
    DataBase dataBase;
    Data data;
    
    //FUNCIONES
    ERROR_CODE (*ventanas[3])(Library);

    //VENTANAS
    WINDOWS nextWindow;

    

    //WIDGETS GTK
    GtkWidget* window;
    GtkWidget* correo;
    GtkWidget* password;
    GtkWidget* list;
    GtkWidget* searchEntry;
};



Library initLibrary(void){

    Library newLibrary = NULL;
    newLibrary = malloc(sizeof(struct _Library));
    
    if(!newLibrary){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, MEM_ERROR);
        exit(MEM_ERROR); 
    }

    
    newLibrary->ventanas[0] = &windowLoggin;
    newLibrary->ventanas[1] = &windowMenuPrincipal;
    newLibrary->ventanas[2] = &editUsersWindow;
    newLibrary->user = initUser();
    newLibrary->dataBase = initDataBase();
    newLibrary->data = initData();
    newLibrary->nextWindow = LOGIN;

    return newLibrary;
}


Library freeLibrary(Library library){
    
    
    library->dataBase = freeDataBase(library->dataBase);
    library->user = freeUser(library->user);
    library->data = freeData(library->data);
   
    free(library);
    puts("free library");
    library = NULL;
    return library;
}

ERROR_CODE windowLoggin(Library library){

    GtkWidget *cajaV, *cajaH4, *cajaH, *cajaH2, *cajaH3, *separador,
              *etiqueta, *etiquetaCorreo, *etiquetaPassword, *botonLogin;

    library->window= gtk_window_new(GTK_WINDOW_TOPLEVEL);
    cajaV = gtk_vbox_new(TRUE, 5);
    cajaH = gtk_hbox_new(TRUE, 5);
    cajaH2 = gtk_hbox_new(TRUE, 5);
    cajaH3 = gtk_hbox_new(TRUE, 5);
    cajaH4 = gtk_hbox_new(TRUE, 5);
    etiqueta = gtk_label_new("Ingresa tu correo y contraseña!");
    etiquetaCorreo = gtk_label_new("Correo:");
    etiquetaPassword = gtk_label_new("Contraseña:");
    library->correo =  gtk_entry_new();
    library->password = gtk_entry_new();
    botonLogin = gtk_button_new_with_mnemonic("_LOGIN");
    separador = gtk_hseparator_new();
    
    library->nextWindow = SALIR;

    
    // Atributos
    gtk_window_set_title(GTK_WINDOW(library->window), "Libreria JSS");
    gtk_window_set_default_size(GTK_WINDOW(library->window), 400, 300);
    gtk_entry_set_max_length(GTK_ENTRY(library->password), 16);
    gtk_entry_set_visibility(GTK_ENTRY(library->password), FALSE);


    g_signal_connect(G_OBJECT(library->window), "delete_event", G_CALLBACK(delete_event_handler), library);
    g_signal_connect(G_OBJECT(library->window), "destroy", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonLogin), "clicked", G_CALLBACK(validateLogin), library);


    gtk_box_pack_start_defaults(GTK_BOX(cajaH4), etiqueta);
    gtk_box_pack_start_defaults(GTK_BOX(cajaV), cajaH4);
    gtk_box_pack_start_defaults(GTK_BOX(cajaH), etiquetaCorreo);
    gtk_box_pack_start_defaults(GTK_BOX(cajaH), library->correo);
    gtk_box_pack_start_defaults(GTK_BOX(cajaV), cajaH);
    gtk_box_pack_start_defaults(GTK_BOX(cajaH2), etiquetaPassword);
    gtk_box_pack_start_defaults(GTK_BOX(cajaH2), library->password);
    gtk_box_pack_start_defaults(GTK_BOX(cajaV), cajaH2);
    gtk_box_pack_start_defaults(GTK_BOX(cajaV), separador);
    gtk_box_pack_start_defaults(GTK_BOX(cajaH3), botonLogin);
    gtk_box_pack_start_defaults(GTK_BOX(cajaV), cajaH3);




    gtk_container_add(GTK_CONTAINER(library->window), cajaV);
    gtk_widget_show_all(library->window);
   
    
    return ERROR_OK;

}

ERROR_CODE windowMenuPrincipal(Library library){

    GtkWidget *cajaV, *etiqueta, *botonAdquirirLib, *botonPrestamosLib, *botonSalir, 
            *botonAgregarUsrs, *cajaH, *cajaV2, *image;
    
    char* images[] = {"images/Menu.jpg"};
    
    library->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    cajaV = gtk_hbox_new(FALSE, 5);
    cajaV2 = gtk_vbox_new(FALSE, 5);
    cajaH = gtk_vbox_new(FALSE, 5); 
    botonAdquirirLib = gtk_button_new_with_label("Adquirir Libro");
    botonSalir = gtk_button_new_with_label("Salir");
    botonPrestamosLib = gtk_button_new_with_label("Libors Prestados");
    image = gtk_image_new_from_file(images[0]);

    library->nextWindow = SALIR;
    

    
    
    gtk_window_set_default_size(GTK_WINDOW(library->window), 200, 200);
    gtk_window_set_title(GTK_WINDOW(library->window), "LIBRERIA JSS");
    
    g_signal_connect(G_OBJECT(library->window), "delete_event", G_CALLBACK(delete_event_handler), library);
    g_signal_connect(G_OBJECT(library->window), "destroy", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonSalir), "clicked", G_CALLBACK(cerrar), library);


   

    gtk_box_pack_start(GTK_BOX(cajaV2), image, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaV2, TRUE, TRUE, 0);
    //-----------------------------------------------------------
    gtk_box_pack_start(GTK_BOX(cajaH), botonAdquirirLib, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH), botonPrestamosLib, FALSE, FALSE,0);
    
    
     if(getTypeUser(library->user) == ADMIN){

        botonAgregarUsrs = gtk_button_new_with_label("Agregar Usuarios");
        etiqueta = gtk_label_new("ADMIN");
        g_signal_connect(G_OBJECT(botonAgregarUsrs), "clicked", G_CALLBACK(chanegeWindow), library);
        gtk_box_pack_start(GTK_BOX(cajaH), botonAgregarUsrs, FALSE, FALSE, 5);
    }else
         etiqueta = gtk_label_new("CLIENTE");
        


    //-----------------------------------------------------------
    gtk_box_pack_start(GTK_BOX(cajaH), botonSalir, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH), etiqueta, TRUE, TRUE,40);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH, FALSE, FALSE, 5);
    
    gtk_container_add(GTK_CONTAINER(library->window), cajaV);
    gtk_widget_show_all(library->window);
    return ERROR_OK;
}


ERROR_CODE editUsersWindow(Library library){
    

    GtkWidget *vbox, *hbox, *label, *botonBuscar, *botonAgregarUsr, *sw;
    GtkTreeSelection *selection; 

    if(library->nextWindow == EDIT_USERS)
        getAllUsers(library->dataBase, library->data);

    

    library->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    library->list = gtk_tree_view_new();
    botonBuscar = gtk_button_new_with_mnemonic("_ENTER");
    vbox = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
    label = gtk_label_new("SEARCH:");
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(library->list));
    library->searchEntry = gtk_entry_new();
    botonAgregarUsr = gtk_button_new_with_mnemonic("ADD USER");
    library->nextWindow = SALIR;
    sw = gtk_scrolled_window_new(NULL, NULL);

    init_list(library->list);
    for(uint8_t i = 0; i < getDataRows(library->data); i++)
        add_to_list(library->list, getRowDataBd(library->data, i));


    gtk_window_set_title(GTK_WINDOW(library->window), "List view");
    gtk_window_set_position(GTK_WINDOW(library->window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(library->window), 10);
    gtk_window_set_default_size(GTK_WINDOW(library->window), 300, 250);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(library->list), TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);


    g_signal_connect(G_OBJECT(library->window), "delete_event", G_CALLBACK(delete_event_handler), library);
    g_signal_connect(G_OBJECT(library->window), "destroy", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonBuscar), "clicked", G_CALLBACK(searchUserAction), library);
    g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);
    
    gtk_container_add(GTK_CONTAINER(sw), library->list);

    gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 5);
    //gtk_box_pack_start(GTK_BOX(vbox), library->list, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), library->searchEntry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), botonBuscar, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), botonAgregarUsr, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(library->window), vbox);

   

    
    gtk_widget_show_all(library->window);

    return ERROR_OK;
}



// GETTERS
ERROR_CODE getNextWindow(Library library, WINDOWS nextWindow){
 
    if(!library){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    } 

    if(nextWindow != LOGIN){
        library->nextWindow = nextWindow;
        gtk_widget_destroy(library->window);

        if(nextWindow == UPDATE)
            library->ventanas[2](library);
        else
            library->ventanas[nextWindow](library);
    }
    else
        library->ventanas[nextWindow](library);

    return ERROR_OK;
}



// STATICS FUNCTIONS
static ERROR_CODE validateLogin(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;

    setCorreo(library->user, gtk_entry_get_text(GTK_ENTRY(library->correo)));
    setPassword(library->user, gtk_entry_get_text(GTK_ENTRY(library->password)));

    printf("CORREO: %s\n", getCorreo(library->user));
    printf("PASSWORD: %s\n", getPassword(library->user));

 
    if(validateEmptyFileds(getCorreo(library->user), getPassword(library->user)) == EMPTY_FIELDS){
        dialogWarning(1);
        return EMPTY_FIELDS;
    }
    else if(validateEmail(getCorreo(library->user)) == INVALID_EMAIL){
        dialogWarning(2);
        return INVALID_EMAIL;
    }
    else if(login(library->dataBase, library->user) == USER_NOT_FOUND){
        dialogWarning(0);
        return USER_NOT_FOUND;
    }
        
    getNextWindow(library, MENU_PRINCIPAL);
    return ERROR_OK;
}



static ERROR_CODE chanegeWindow(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;
    
    getNextWindow(library, EDIT_USERS);

    return ERROR_OK;
}

static ERROR_CODE dialogWarning(int8_t error){

    GtkWidget *windowError, *errorDialog;
    gint respuesta;
    const char* errorMessage[4] = {"SE INGRESO MAL EL CORREO O LA CONTRASEÑA\n", "SE DEJO ALGUN CAMPO VACIO\n", "EL CORREO INGRESADO NO ES VALIDO", "NO SE ENCONTRO NINGUN USURIO"};

    windowError = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    errorDialog = gtk_message_dialog_new(GTK_WINDOW(windowError), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, errorMessage[error], NULL);

    gtk_widget_show_all(errorDialog);
    respuesta = gtk_dialog_run(GTK_DIALOG(errorDialog));

    if(respuesta == GTK_RESPONSE_OK)
        gtk_widget_destroy(errorDialog);
    
    return ERROR_OK;
}


static ERROR_CODE validateEmptyFileds(char* correo, char* password){

    if(strlen(correo) == 0 || strlen(password) == 0){
        printf("ERROR EMPTY FIELDS: %d\n", EMPTY_FIELDS);
        return EMPTY_FIELDS;
    }

    return ERROR_OK;
}



static ERROR_CODE validateEmail(char* correo){

    const char* validsEmails[7] = {"@gmail.com", "@gmail.com.mx", "@yahoo.com", "@yahoo.com.mx", "@icloud.com", "@outlook.com", "@outlook.com.mx"};
    char correoToCheck[BUFSIZ];
    int8_t isCorreo = 0;

    for(int i = 0; i < strlen(correo); i++){
        
        if(correo[i] == '@' && i == 0){
            printf("ERROR INVALID EMAIL: %d\n", INVALID_EMAIL);
            return INVALID_EMAIL;
        }
            

        else if(correo[i] == '@' || isCorreo){

            correoToCheck[isCorreo] = tolower(correo[i]);
            isCorreo++;
        }
    }

    correoToCheck[isCorreo] = '\0';

    for(int i = 0; i < 7; i++){
        
        if(strcmp(correoToCheck, validsEmails[i]) == 0)
            return ERROR_OK;
    }

    printf("ERROR INVALID EMAIL: %d\n", INVALID_EMAIL);
    return INVALID_EMAIL;
}

static gboolean delete_event_handler(GtkWidget *widget, GdkEvent *event, gpointer userData){
    
    Library library = (Library) userData;
    library->nextWindow = SALIR;
    g_print("En delete_event_handler.\n");
    return FALSE;
}

static void cerrar(GtkWidget *widget, gpointer userData){

    Library library = (Library) userData;
    if(library->nextWindow != SALIR)
        return;
    
    library = freeLibrary(library);
    gtk_main_quit();
        
}


static void init_list(GtkWidget *list) {

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Id", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Type_User", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nombre", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Apellido Pat", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Apellido Pat", renderer, "text", 4, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Carrera", renderer, "text", 5, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Semestre", renderer, "text", 6, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);
    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Correo", renderer, "text", 7, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);


    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Fech Nacimiento", renderer, "text", 8, NULL);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Password", renderer, "text", 9, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    store = gtk_list_store_new(10, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

static void add_to_list(GtkWidget *list, gchar **str) {
    
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);

    gtk_list_store_set(store, &iter, 0, str[0],
                    1, str[1],
                    2, str[2],
                    3, str[3],
                    4, str[4],
                    5, str[5],
                    6, str[6],
                    7, str[7],
                    8, str[8],
                    9, str[9],
                    -1);
}

static void on_changed(GtkWidget *widget, gpointer label) {
    
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(
      GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 0, &value,  -1);
    gtk_label_set_text(GTK_LABEL(label), value);
    g_free(value);
  }
}


static ERROR_CODE searchUserAction(GtkButton *boton, gpointer userData){
    
    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter  iter;
    Library library = (Library) userData;

    setArgumentSearch(library->data,  gtk_entry_get_text(GTK_ENTRY(library->searchEntry)));
   
    
    
    if(searchUser(library->dataBase, library->data) == EMPTY_SET){
        dialogWarning(3);
        return EMPTY_SET;
    }



    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(library->list)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(library->list));

    if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
        return MEM_ERROR;
    }
    
    gtk_list_store_clear(store);


    for(uint8_t i = 0; i < getDataRows(library->data); i++)
        add_to_list(library->list, getRowDataBd(library->data, i));
    
        
    return ERROR_OK;
}
