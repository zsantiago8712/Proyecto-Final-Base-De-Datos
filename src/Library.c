#include "../libs/Library.h"


static ERRORS_ENTRY validateLogin(GtkButton *boton, gpointer userData);
static ERROR_CODE dialogWarning(int8_t error);
static ERRORS_ENTRY validateEmail(char* correo);
static ERROR_CODE validateEmptyFileds(const char* strintToValiidate);
static gboolean delete_event_handler(GtkWidget *widget, GdkEvent *event, gpointer userData);
static void cerrar(GtkWidget *widget, gpointer userData);
static void init_list_users(GtkWidget *list);
static void add_to_list_users(GtkWidget *list, gchar **str);
static void on_changed(GtkWidget *widget, gpointer label);
static ERROR_CODE searchUserAction(GtkButton *boton, gpointer userData);
static ERROR_CODE addUser(GtkWidget *widget, gpointer userData);
static ERROR_CODE setTypeAdmin(GtkWidget *widget, gpointer userData);
static ERROR_CODE setTypeClient(GtkWidget *widget, gpointer userData);
static ERRORS_ENTRY validatePasswordField(const char* entryToValidate);
static ERRORS_ENTRY validateCorreoField(const char* entryToValidate);
static ERRORS_ENTRY validateSemestreField(uint8_t entryToValidate);
static ERRORS_ENTRY validateCarreraField(const char* entryToValidate);
static ERRORS_ENTRY validateApellidoPatField(const char* entryToValidate);
static ERRORS_ENTRY validateApellidoMatField(const char* entryToValidate);
static ERRORS_ENTRY validateNameField(const char* entryToValidate);
static ERRORS_ENTRY saveNewUser(GtkWidget *widget, gpointer userData);
static const char* convertSemestreToString(uint8_t semestre);
static ERROR_CODE cancelSaveNewUser(GtkWidget *widget, gpointer userData);
static ERROR_CODE setFechaNacimientoFromCalendar(GtkWidget *widget, gpointer userData);
static ERROR_CODE updateList(Library library, UPDATES typeUpdate);
static ERROR_CODE searchLibroAction(GtkButton *boton, gpointer userData);
static ERROR_CODE changeWindow(GtkButton *boton, gpointer userData);
static void init_list_libros(GtkWidget *list);
static void add_to_list_libros(GtkWidget *list, gchar **str);
static void on_changedRentar(GtkWidget *widget, gpointer label);
static ERROR_CODE rentarLibroAction(GtkButton *boton, gpointer userData);
static ERROR_CODE dialogSucces(uint8_t succes);
static void on_close(GtkDialog *dialog, gint response_id, gpointer userData);
static void ventanaAcercaDe(GtkButton *boton, gpointer userData);
static void changeWindowFromSubMenuRL(GtkButton *boton, gpointer userData);
static void changeWindowFromSubMenuMP(GtkButton *boton, gpointer userData);
static void changeWindowFromSubMenuQT(GtkButton *boton, gpointer userData);
static void changeWindowFromSubMenuAU(GtkButton *boton, gpointer userData);


struct _Library{

    //STRCUTS
    User user;
    DataBase dataBase;
    Data data;
    
    //FUNCIONES
    ERROR_CODE (*ventanas[5])(Library);

    //VENTANAS
    WINDOWS nextWindow;

    

    //WIDGETS GTK
    GtkWidget* window, *secundaryWindow;
    GtkWidget* correo;
    GtkWidget* password;
    GtkWidget* list;
    GtkWidget* searchEntry;
    GtkWidget *nombreEntry, *apelidoPatEntry, *apellidoMatEntry, *carreraEntry, *semestreEntry, *correoEntry, *fechaNacimientoEntry, *passwordEntry;
    uint8_t typeUser;

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
    newLibrary->ventanas[3] = &rentarLibro;
    newLibrary->user = initUser();
    newLibrary->dataBase = initDataBase();
    newLibrary->data = initData();
    newLibrary->nextWindow = LOGIN;
    newLibrary->typeUser = ADMIN;

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





/****************************************************************************************************/
/****************************************************************************************************/

// VENTANAS
ERROR_CODE windowLoggin(Library library){

    GtkWidget *cajaV, *cajaH4, *cajaH, *cajaH2, *cajaH3, *separador, *etiqueta, *etiquetaCorreo, *etiquetaPassword, *botonLogin;

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

    GtkWidget *cajaV, *etiqueta, *botonAdquirirLib, *botonPrestamosLib, *botonSalir, *botonAgregarUsrs, *cajaH, *cajaV2, *image;
    
    
    char* images[] = {"images/Menu.jpg"};
    
    library->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    cajaV = gtk_hbox_new(FALSE, 5);
    cajaV2 = gtk_vbox_new(FALSE, 5);
    cajaH = gtk_vbox_new(FALSE, 5); 
    botonAdquirirLib = gtk_button_new_with_label("Rentar Libros");
    botonSalir = gtk_button_new_with_label("Salir");
    botonPrestamosLib = gtk_button_new_with_label("Libros Prestados");
    image = gtk_image_new_from_file(images[0]);
    library->nextWindow = SALIR;
    

    
    
    gtk_window_set_default_size(GTK_WINDOW(library->window), 200, 200);
    gtk_window_set_title(GTK_WINDOW(library->window), "LIBRERIA JSS");
    
    g_signal_connect(G_OBJECT(library->window), "delete_event", G_CALLBACK(delete_event_handler), library);
    g_signal_connect(G_OBJECT(library->window), "destroy", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonSalir), "clicked", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonAdquirirLib), "clicked", G_CALLBACK(changeWindow), library);
    
    

   

    gtk_box_pack_start(GTK_BOX(cajaV2), image, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaV2, TRUE, TRUE, 0);
    //-----------------------------------------------------------
    gtk_box_pack_start(GTK_BOX(cajaH), botonAdquirirLib, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH), botonPrestamosLib, FALSE, FALSE,0);
    
    
    
     if(getTypeUser(library->user) == ADMIN){

        botonAgregarUsrs = gtk_button_new_with_label("Agregar Usuarios");
        etiqueta = gtk_label_new("ADMIN");
        gtk_box_pack_start(GTK_BOX(cajaH), botonAgregarUsrs, FALSE, FALSE, 5);
        g_signal_connect(G_OBJECT(botonAgregarUsrs), "clicked", G_CALLBACK(changeWindow), library);
        
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
    

    GtkWidget *vbox, *hbox, *hbox2, *label, *botonBuscar, *botonAgregarUsr, *sw,
    *menuBar, *menuBarMenuPrincipal, *menuBarRentarLibro, *menuBarSalir, *opciones, *opcionesLabel, *ayudaLabel, *ayudaMenu, *acercaDeLabel;

    GtkTreeSelection *selection; 
  

    getAllUsers(library->dataBase, library->data);

    

    library->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    library->list = gtk_tree_view_new();
    botonBuscar = gtk_button_new_with_mnemonic("_ENTER");
    vbox = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
    hbox2 = gtk_hbox_new(FALSE, 0);
    label = gtk_label_new("SEARCH:");
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(library->list));
    library->searchEntry = gtk_entry_new();
    botonAgregarUsr = gtk_button_new_with_mnemonic("ADD USER");
    library->nextWindow = SALIR;
    sw = gtk_scrolled_window_new(NULL, NULL);
    opcionesLabel = gtk_menu_item_new_with_label("Menu");
    opciones = gtk_menu_new();
    menuBar = gtk_menu_bar_new();
    menuBarRentarLibro = gtk_image_menu_item_new_with_label("Rentar Libro");
    menuBarMenuPrincipal = gtk_image_menu_item_new_from_stock(GTK_STOCK_HOME, NULL);
    menuBarSalir =  gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    ayudaLabel = gtk_menu_item_new_with_label("Ayuda");
    ayudaMenu = gtk_menu_new(); 
    acercaDeLabel = gtk_menu_item_new_with_label("Acerca de");

    


    init_list_users(library->list);
    for(uint8_t i = 0; i < getDataRows(library->data); i++)
        add_to_list_users(library->list, getRowDataBd(library->data, i));


    gtk_window_set_title(GTK_WINDOW(library->window), "LIBRERIA JSS");
    gtk_window_set_position(GTK_WINDOW(library->window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(library->window), 10);
    gtk_window_set_default_size(GTK_WINDOW(library->window), 300, 450);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(library->list), TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
    gtk_menu_item_right_justify(GTK_MENU_ITEM(ayudaLabel));


    g_signal_connect(G_OBJECT(library->window), "delete_event", G_CALLBACK(delete_event_handler), library);
    g_signal_connect(G_OBJECT(library->window), "destroy", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonBuscar), "clicked", G_CALLBACK(searchUserAction), library);
    g_signal_connect(G_OBJECT(botonAgregarUsr), "clicked", G_CALLBACK(addUser), library);
    g_signal_connect(selection, "changed", G_CALLBACK(on_changed), label);
    g_signal_connect(G_OBJECT(acercaDeLabel), "activate", G_CALLBACK(ventanaAcercaDe), library);
    g_signal_connect(G_OBJECT(menuBarSalir), "activate", G_CALLBACK(changeWindowFromSubMenuQT), library);
    g_signal_connect(G_OBJECT(menuBarMenuPrincipal), "activate", G_CALLBACK(changeWindowFromSubMenuMP), library);
    g_signal_connect(G_OBJECT(menuBarRentarLibro), "activate", G_CALLBACK(changeWindowFromSubMenuRL), library);





    gtk_menu_shell_append(GTK_MENU_SHELL(opciones), menuBarMenuPrincipal);
    gtk_menu_shell_append(GTK_MENU_SHELL(opciones), menuBarRentarLibro);
    gtk_menu_shell_append(GTK_MENU_SHELL(opciones), menuBarSalir);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(opcionesLabel), opciones);
    gtk_menu_bar_append(GTK_MENU_BAR(menuBar), opcionesLabel);
    gtk_menu_shell_append(GTK_MENU_SHELL(ayudaMenu), acercaDeLabel);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ayudaLabel), ayudaMenu);
    gtk_menu_bar_append(GTK_MENU_BAR(menuBar), ayudaLabel);
    
    
    
    gtk_container_add(GTK_CONTAINER(sw), library->list);

    gtk_box_pack_start(GTK_BOX(hbox2), menuBar, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), library->searchEntry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), botonBuscar, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), botonAgregarUsr, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(library->window), vbox);

   

    
    gtk_widget_show_all(library->window);

    return ERROR_OK;
}


ERROR_CODE rentarLibro(Library library){
    

    GtkWidget *vbox, *hbox, *hbox2, *label, *botonBuscar, *botonRentar, *sw,
    *menuBar, *menuBarMenuPrincipal, *menuBarAgregarUsr, *menuBarSalir, *opciones, *opcionesLabel, *ayudaLabel, *ayudaMenu, *acercaDeLabel;
    
    GtkTreeSelection *selection; 

    
    getAllLibros(library->dataBase, library->data);

    

    library->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    library->list = gtk_tree_view_new();
    botonBuscar = gtk_button_new_with_mnemonic("_ENTER");
    botonRentar = gtk_button_new_with_mnemonic("_Rentar");
    vbox = gtk_vbox_new(FALSE, 0);
    hbox = gtk_hbox_new(FALSE, 0);
    hbox2 = gtk_hbox_new(FALSE, 0);
    label = gtk_label_new("SEARCH:");
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(library->list));
    library->searchEntry = gtk_entry_new();
    library->nextWindow = SALIR;
    sw = gtk_scrolled_window_new(NULL, NULL);
    opcionesLabel = gtk_menu_item_new_with_label("Menu");
    opciones = gtk_menu_new();
    menuBar = gtk_menu_bar_new();
    menuBarMenuPrincipal = gtk_image_menu_item_new_from_stock(GTK_STOCK_HOME, NULL);
    menuBarAgregarUsr = gtk_image_menu_item_new_with_label("Rentar Libro");
    menuBarSalir =  gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
    ayudaLabel = gtk_menu_item_new_with_label("Ayuda");
    ayudaMenu = gtk_menu_new(); 
    acercaDeLabel = gtk_menu_item_new_with_label("Acerca de");

    printf("USER TYPE : %d\n", getId(library->user));
    if(getTypeUser(library->user) == ADMIN){
        menuBarAgregarUsr = gtk_image_menu_item_new_with_label("Agregar Usuario");
        gtk_menu_shell_append(GTK_MENU_SHELL(opciones), menuBarAgregarUsr);
        g_signal_connect(G_OBJECT(menuBarAgregarUsr), "activate", G_CALLBACK(changeWindowFromSubMenuAU), library);
        
    }
    gtk_menu_shell_append(GTK_MENU_SHELL(opciones), menuBarMenuPrincipal);
    gtk_menu_shell_append(GTK_MENU_SHELL(opciones), menuBarSalir);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(opcionesLabel), opciones);
    gtk_menu_bar_append(GTK_MENU_BAR(menuBar), opcionesLabel);
    gtk_menu_shell_append(GTK_MENU_SHELL(ayudaMenu), acercaDeLabel);
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(ayudaLabel), ayudaMenu);
    gtk_menu_bar_append(GTK_MENU_BAR(menuBar), ayudaLabel);



    init_list_libros(library->list);
    for(uint8_t i = 0; i < getDataRows(library->data); i++)
        add_to_list_libros(library->list, getRowDataBd(library->data, i));


    gtk_window_set_title(GTK_WINDOW(library->window), "LIBRERIA JSS");
    gtk_window_set_position(GTK_WINDOW(library->window), GTK_WIN_POS_CENTER);
    gtk_container_set_border_width(GTK_CONTAINER(library->window), 10);
    gtk_window_set_default_size(GTK_WINDOW(library->window), 400, 450);
    gtk_tree_view_set_headers_visible(GTK_TREE_VIEW(library->list), TRUE);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(sw), GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(sw), GTK_SHADOW_ETCHED_IN);
    gtk_menu_item_right_justify(GTK_MENU_ITEM(ayudaLabel));
    

    g_signal_connect(G_OBJECT(library->window), "delete_event", G_CALLBACK(delete_event_handler), library);
    g_signal_connect(G_OBJECT(library->window), "destroy", G_CALLBACK(cerrar), library);
    g_signal_connect(G_OBJECT(botonBuscar), "clicked", G_CALLBACK(searchLibroAction), library);
    g_signal_connect(G_OBJECT(botonRentar), "clicked", G_CALLBACK(rentarLibroAction), library);
    g_signal_connect(selection, "changed", G_CALLBACK(on_changedRentar), library);
    g_signal_connect(G_OBJECT(acercaDeLabel), "activate", G_CALLBACK(ventanaAcercaDe), library);
    g_signal_connect(G_OBJECT(menuBarSalir), "activate", G_CALLBACK(changeWindowFromSubMenuQT), library);
    g_signal_connect(G_OBJECT(menuBarMenuPrincipal), "activate", G_CALLBACK(changeWindowFromSubMenuMP), library);
    

    
    gtk_container_add(GTK_CONTAINER(sw), library->list);
    gtk_box_pack_start(GTK_BOX(hbox2), menuBar, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox2, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), sw, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), library->searchEntry, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), botonBuscar, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(hbox), botonRentar, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(library->window), vbox);

   
    
    gtk_widget_show_all(library->window);

    return ERROR_OK;
}

/****************************************************************************************************/
/****************************************************************************************************/


// GETTERS
ERROR_CODE getNextWindow(Library library, WINDOWS nextWindow){
 
    if(!library){
        fprintf(stderr, "ERROR: %s %d %d", __FILE__, __LINE__, EMPTY_STRUCT);
        exit(EMPTY_STRUCT);
    } 

    if(nextWindow == SALIR)
        gtk_widget_destroy(library->window);

    else if(nextWindow != LOGIN){
        library->nextWindow = nextWindow;
        gtk_widget_destroy(library->window);
        library->ventanas[nextWindow](library);
    }
    else
        library->ventanas[nextWindow](library);

    return ERROR_OK;
}





// STATICS FUNCTIONS
static ERRORS_ENTRY validateLogin(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;

    setCorreo(library->user, gtk_entry_get_text(GTK_ENTRY(library->correo)));
    setPassword(library->user, gtk_entry_get_text(GTK_ENTRY(library->password)));

    printf("CORREO: %s\n", getCorreo(library->user));
    printf("PASSWORD: %s\n", getPassword(library->user));

 
    if(validateEmptyFileds(getCorreo(library->user)) == EMPTY_STRING || validateEmptyFileds(getPassword(library->user)) == EMPTY_STRING){
        dialogWarning(EMPTY_FIELDS);
        return EMPTY_STRING;
    }
    else if(validateEmail(getCorreo(library->user)) == INVALID_EMAIL){
        dialogWarning(INVALID_EMAIL);
        return INVALID_EMAIL;
    }
    else if(login(library->dataBase, library->user) == EMPTY_SET){
        dialogWarning(INVALID_MAIL_PASSWORD);
        return INVALID_MAIL_PASSWORD;
    }
        
    getNextWindow(library, MENU_PRINCIPAL);
    return OK;
}





static ERROR_CODE changeWindow(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;
    
    if(strcmp("Rentar Libros", gtk_button_get_label(boton)) == 0)
        getNextWindow(library, RENTA_LIBROS);
    else if(strcmp("Libors Prestados", gtk_button_get_label(boton)) == 0)
        getNextWindow(library, PRESATAMOS);
    else if(strcmp("Agregar Usuarios", gtk_button_get_label(boton)) == 0)
        getNextWindow(library, EDIT_USERS);

    return ERROR_OK;
}





static ERROR_CODE dialogWarning(int8_t error){

    GtkWidget *windowError, *errorDialog;
    gint respuesta;
    const char* errorMessage[] = {"SE INGRESO MAL EL CORREO O LA CONTRASEÑA\n", "SE DEJO ALGUN CAMPO VACIO\n", "EL CORREO INGRESADO NO ES VALIDO", "NO SE ENCONTRO NINGUN USURIO", 
    "NO SE ENCONTRO NINGUN LIBRO", "SE DEJO VACIO EL CAMPO DE NOMBRE", "SE DEJO VACIO EL CAMPO DE APELLIDO PATERNO", "SE DEJO VACIO EL CAMPO DE CORREO", "SE DEJO VACIO EL CAMPO DE PASSWORD", 
    "SE DEJO VACIO EL CAMPO DE FECHA DE NACIMIENTO", "ALGO SALIO MAL AL INTENAT AGREGAR EL USUARIO", "NO HAY NINGUN EJEMPLAR DISPONIBLE\n", "ALGO SALIO MAL AL REALIZAR LA RENTA\n"};

    windowError = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    errorDialog = gtk_message_dialog_new(GTK_WINDOW(windowError), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, errorMessage[error], NULL);

    gtk_widget_show_all(errorDialog);
    respuesta = gtk_dialog_run(GTK_DIALOG(errorDialog));

    if(respuesta == GTK_RESPONSE_OK)
        gtk_widget_destroy(errorDialog);
    
    return ERROR_OK;
}

static ERROR_CODE dialogSucces(uint8_t succes){
    
    GtkWidget *windowSucces, *succesDialog;
    guint respuesta;
    const char* succesMessage[] = {"SE AGREGO CRRECTAMENTE AL USUARIO", "SE REALIZO CORRECTAMENTA EL PRESTAMO", "SE REALIZO CORRECTAMENTE LA DEVOLUCION"};

    windowSucces = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    succesDialog = gtk_message_dialog_new(GTK_WINDOW(windowSucces), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, succesMessage[succes], NULL);

    gtk_widget_show_all(succesDialog);
    respuesta = gtk_dialog_run(GTK_DIALOG(succesDialog));

    if(respuesta == GTK_RESPONSE_OK)
        gtk_widget_destroy(succesDialog);
    
    return ERROR_OK;

}


static ERROR_CODE addUser(GtkWidget *widget, gpointer userData){

    Library library = (Library) userData;
    GtkWidget *aceptar, *cancelar, *cajaV, *cajaH, *cajaH2, *cajaH3, *cajaH4, *cajaH5, *cajaH6, *cajaH7, *cajaH8, *cajaH9, *cajaH10, *cajaH11, *etiqueta2, *userTypeEntryAdmin, *userTypeEntryClient,
    *etiqueta, *etiquetaTypeUser, *etiquetaNombre, *etiquetaApellidoPat, *etiquetaApellidoMat, *etiquetaCarrera, *etiquetaSemestre, *etiquetaCorreo, *etiquetaPassword, *etiquetaFechaNacimiento;

    GdkColor important;
    gdk_color_parse("red", &important); 
    
    userTypeEntryAdmin = gtk_radio_button_new_with_label(NULL, "Admin");
    userTypeEntryClient = gtk_radio_button_new_with_label(gtk_radio_button_get_group(GTK_RADIO_BUTTON(userTypeEntryAdmin)), "Client");

    
    library->nombreEntry = gtk_entry_new();
    library->apelidoPatEntry = gtk_entry_new();
    library->apellidoMatEntry = gtk_entry_new();
    library->carreraEntry = gtk_entry_new();
    library->semestreEntry = gtk_spin_button_new_with_range(0, 12, 1);
    library->correoEntry = gtk_entry_new();
    library->passwordEntry = gtk_entry_new();
    library->fechaNacimientoEntry = gtk_calendar_new();
    library->secundaryWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    library->nextWindow = SECUNDARY_WINDOW;

    aceptar = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    cancelar = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
    cajaV = gtk_vbox_new(FALSE, 5);
    cajaH = gtk_hbox_new(FALSE, 5);
    cajaH2 = gtk_hbox_new(FALSE, 5);
    cajaH3 = gtk_hbox_new(FALSE, 5);
    cajaH4 = gtk_hbox_new(FALSE, 5);
    cajaH5 = gtk_hbox_new(FALSE, 5);
    cajaH6 = gtk_hbox_new(FALSE, 5);
    cajaH7 = gtk_hbox_new(FALSE, 5);
    cajaH8 = gtk_hbox_new(FALSE, 5);
    cajaH9 = gtk_hbox_new(FALSE, 5);
    cajaH10 = gtk_hbox_new(FALSE, 5);
    cajaH11 = gtk_hbox_new(FALSE, 5);  
    etiqueta = gtk_label_new("\tIngresa los datos para agregar al usuario\n");
    etiqueta2 = gtk_label_new(" Es obligatorio que ingrese los datos con un '*'\n");
    etiquetaTypeUser = gtk_label_new("*TypeUser:");
    etiquetaNombre = gtk_label_new("*Nombre:");
    etiquetaApellidoPat = gtk_label_new("*Apellido Pat:");
    etiquetaApellidoMat = gtk_label_new("Apellido Mat:");
    etiquetaCarrera = gtk_label_new("Carrera:");
    etiquetaSemestre = gtk_label_new("Semestre:");
    etiquetaCorreo = gtk_label_new("*Correo:");
    etiquetaPassword = gtk_label_new("*Password:");
    etiquetaFechaNacimiento = gtk_label_new("\n\t\t\t\t*Fecha De Nacimiento:");
   



    //Atributos
    gtk_window_set_title(GTK_WINDOW(library->secundaryWindow), "ADD USER");
    gtk_window_set_default_size(GTK_WINDOW(library->secundaryWindow), 400, 400);
    gtk_widget_modify_fg(etiquetaTypeUser, GTK_STATE_NORMAL, &important);
    gtk_widget_modify_fg(etiquetaNombre, GTK_STATE_NORMAL, &important);
    gtk_widget_modify_fg(etiquetaApellidoPat, GTK_STATE_NORMAL, &important);
    gtk_widget_modify_fg(etiquetaCorreo, GTK_STATE_NORMAL, &important);
    gtk_widget_modify_fg(etiquetaPassword, GTK_STATE_NORMAL, &important);
    gtk_widget_modify_fg(etiqueta2, GTK_STATE_NORMAL, &important);
    gtk_widget_modify_fg(etiquetaFechaNacimiento, GTK_STATE_NORMAL, &important);
 

    
    g_signal_connect(G_OBJECT(aceptar), "clicked", G_CALLBACK(saveNewUser), library);
    g_signal_connect(G_OBJECT(cancelar), "clicked", G_CALLBACK(cancelSaveNewUser), library);
    g_signal_connect(G_OBJECT(userTypeEntryAdmin), "toggled", G_CALLBACK(setTypeAdmin), library);
    g_signal_connect(G_OBJECT(userTypeEntryClient), "toggled", G_CALLBACK(setTypeClient), library);
    g_signal_connect(G_OBJECT(library->fechaNacimientoEntry), "day-selected-double-click", G_CALLBACK(setFechaNacimientoFromCalendar), library);


    gtk_box_pack_start(GTK_BOX(cajaV), etiqueta, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(cajaH), etiquetaTypeUser, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH), userTypeEntryAdmin, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH), userTypeEntryClient, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cajaH2), etiquetaNombre, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH2), library->nombreEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH2, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cajaH3), etiquetaApellidoPat, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH3), library->apelidoPatEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH3, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cajaH4), etiquetaApellidoMat, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH4), library->apellidoMatEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH4, FALSE, FALSE, 0);
    
    
    gtk_box_pack_start(GTK_BOX(cajaH5), etiquetaCarrera, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH5), library->carreraEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH5, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cajaH6), etiquetaSemestre, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH6), library->semestreEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH6, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cajaH7), etiquetaCorreo, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH7), library->correoEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH7, FALSE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(cajaH8), etiquetaPassword, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH8), library->passwordEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH8, FALSE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(cajaH9), etiquetaFechaNacimiento, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH9, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(cajaH10), library->fechaNacimientoEntry, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH10, FALSE, FALSE, 0);


    gtk_box_pack_start(GTK_BOX(cajaH11), aceptar, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(cajaH11), cancelar, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(cajaV), cajaH11, FALSE, FALSE, 5);
    
    gtk_box_pack_start(GTK_BOX(cajaV), etiqueta2, FALSE, FALSE, 5);
    gtk_container_add(GTK_CONTAINER(library->secundaryWindow), cajaV);
    gtk_widget_show_all(library->secundaryWindow); 

    return ERROR_OK;

}

static ERROR_CODE setTypeAdmin(GtkWidget *widget, gpointer userData){
    
    Library library = (Library) userData;  
    library->typeUser = ADMIN;
    printf("\nTYPE USER ADMIN: %d", library->typeUser);

    return ERROR_OK;
}


static ERROR_CODE setTypeClient(GtkWidget *widget, gpointer userData){
    
    Library library = (Library) userData;  
    library->typeUser = CLIENT;

    printf("\nTYPE USER CLIENT: %d", library->typeUser);

    return ERROR_OK;
}

static ERROR_CODE setFechaNacimientoFromCalendar(GtkWidget *widget, gpointer userData){

    Library library = (Library) userData;
    guint day, month, year;
    char date[20];

    gtk_calendar_get_date(GTK_CALENDAR(library->fechaNacimientoEntry), &year, &month, &day);
    printf("\nDAY: %d | MONTH: %d | YEAR: %d\n", day, month, year);


    sprintf(date, "%d/%d/%d", year, month, day);

    setArgumentInsert(library->data, date, 8);
    printf("\nDATE: %s\n", getArgumentInsert(library->data, 8));

    return ERROR_OK;    
}

static ERRORS_ENTRY saveNewUser(GtkWidget *widget, gpointer userData){


    Library library = (Library) userData; 
    
    printf("TYPE USER %d\n", library->typeUser);
    if(library->typeUser == ADMIN)
        setArgumentInsert(library->data, "TRUE", 0);
    else
        setArgumentInsert(library->data, "FALSE", 0);
    

    printf("\n%s\n", getArgumentInsert(library->data, 0));

    if(validateNameField(gtk_entry_get_text(GTK_ENTRY(library->nombreEntry))) == OK){
        setArgumentInsert(library->data, gtk_entry_get_text(GTK_ENTRY(library->nombreEntry)), 1);
        printf("\n%s\n",getArgumentInsert(library->data, 1));
    }
    else{
        clearDataInsert(library->data);
        return ERROR_FIELDS;
    }
       


    if(validateApellidoPatField(gtk_entry_get_text(GTK_ENTRY(library->apelidoPatEntry))) == OK){
        setArgumentInsert(library->data, gtk_entry_get_text(GTK_ENTRY(library->apelidoPatEntry)), 2);
        printf("\n%s\n",getArgumentInsert(library->data, 2));
    }
    else{
        clearDataInsert(library->data);
        return ERROR_FIELDS;
    }


    
    
    
    if(validateApellidoMatField(gtk_entry_get_text(GTK_ENTRY(library->apellidoMatEntry))) == OK){
        setArgumentInsert(library->data, gtk_entry_get_text(GTK_ENTRY(library->apellidoMatEntry)), 3);  
        printf("\n%s\n",getArgumentInsert(library->data, 3));
    }
    else{
        setArgumentInsert(library->data, "NULL", 3);
        printf("\n%s\n",getArgumentInsert(library->data, 3));
    }
    

    
    if(validateCarreraField(gtk_entry_get_text(GTK_ENTRY(library->carreraEntry))) == OK){
        setArgumentInsert(library->data, gtk_entry_get_text(GTK_ENTRY(library->carreraEntry)), 4);  
        printf("\n%s\n",getArgumentInsert(library->data, 4));
    }
    else{
        setArgumentInsert(library->data, "NULL", 4);
        printf("\n%s\n",getArgumentInsert(library->data, 4));
    }

    
    if(validateSemestreField(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(library->semestreEntry))) == OK){
        setArgumentInsert(library->data, convertSemestreToString(gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(library->semestreEntry))), 5);
        printf("\n%s\n",getArgumentInsert(library->data, 5));
    }
    else{
        setArgumentInsert(library->data, "NULL", 5);
        printf("\n%s\n",getArgumentInsert(library->data, 5));
    }
        

    if(validateCorreoField(gtk_entry_get_text(GTK_ENTRY(library->correoEntry))) == OK){
        setArgumentInsert(library->data, gtk_entry_get_text(GTK_ENTRY(library->correoEntry)), 6);
        printf("\n%s\n",getArgumentInsert(library->data, 6));
    }else{
        clearDataInsert(library->data); 
        return ERROR_FIELDS;
    }


    if(validatePasswordField(gtk_entry_get_text(GTK_ENTRY(library->passwordEntry))) == OK){
        setArgumentInsert(library->data, gtk_entry_get_text(GTK_ENTRY(library->passwordEntry)), 7);
        printf("\n%s\n",getArgumentInsert(library->data, 7));
    }else{
        clearDataInsert(library->data); 
        return ERROR_FIELDS;
    }
    

    if(!getArgumentInsert(library->data, 8)){
        dialogWarning(F_NAC_FIELD_EMPTY);
        clearDataInsert(library->data);
        return ERROR_FIELDS;
    }


    if(addUserToDB(library->dataBase, library->data) != ERROR_OK){
        clearDataInsert(library->data);  
        dialogWarning(ADD_USER_ERROR);
        return ERROR_FIELDS;
    }
    
    clearData(library->data);
    getAllUsers(library->dataBase, library->data);
    updateList(library, USERS);
    clearDataInsert(library->data);
    dialogSucces(0);
    return OK;   
}    


static ERROR_CODE cancelSaveNewUser(GtkWidget *widget, gpointer userData){

    Library library = (Library) userData;

    clearDataInsert(library->data); 

    gtk_widget_destroy(library->secundaryWindow);
    library->nextWindow = SALIR;

    return ERROR_OK;
}


static ERRORS_ENTRY validateNameField(const char* entryToValidate){

    if(validateEmptyFileds(entryToValidate) == EMPTY_STRING){
        dialogWarning(NAME_FIELD_EMPTY);
        return NAME_FIELD_EMPTY;
    }
    
    return OK;
}

static ERRORS_ENTRY validateApellidoPatField(const char* entryToValidate){

    if(validateEmptyFileds(entryToValidate) == EMPTY_STRING){
        dialogWarning(AP_PAT_FIELD_EMPTY);
        return NAME_FIELD_EMPTY;
    }
    
    return OK;
}


static ERRORS_ENTRY validateApellidoMatField(const char* entryToValidate){

    if(validateEmptyFileds(entryToValidate) == EMPTY_STRING)
        return EMPTY_FIELD_OK;
    
    
    return OK;
}

static ERRORS_ENTRY validateCarreraField(const char* entryToValidate){

    if(validateEmptyFileds(entryToValidate) == EMPTY_STRING)
        return EMPTY_FIELD_OK;
    
    
    return OK;
}

static ERRORS_ENTRY validateSemestreField(uint8_t entryToValidate){

    
    if(entryToValidate == 0)
        return EMPTY_FIELD_OK; 

    return OK;
}


static ERRORS_ENTRY validateCorreoField(const char* entryToValidate){

    if(validateEmptyFileds(entryToValidate) == EMPTY_STRING){
        dialogWarning(EMAIL_FIEDL_EMPTY);
        return EMAIL_FIEDL_EMPTY;
    }
    
    if(validateEmail(entryToValidate) == OK){
        return OK;
    }

    dialogWarning(INVALID_EMAIL);
    return INVALID_EMAIL;
}

static ERRORS_ENTRY validatePasswordField(const char* entryToValidate){

    if(validateEmptyFileds(entryToValidate) == EMPTY_STRING){
        dialogWarning(PASSWORD_FIELD_EMPTY);
        return PASSWORD_FIELD_EMPTY;
    }
    
    return OK;
}






static ERROR_CODE validateEmptyFileds(const char* strintToValiidate){

    if(strlen(strintToValiidate) == 0){
        printf("ERROR EMPTY FIELDS: %d\n", EMPTY_STRING);
        return EMPTY_STRING;
    }

    return ERROR_OK;
}



static ERRORS_ENTRY validateEmail(char* correo){

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
            return OK;
    }

    printf("ERROR INVALID EMAIL: %d\n", INVALID_EMAIL);
    return INVALID_EMAIL;
}



static const char* convertSemestreToString(uint8_t semestre){

    switch (semestre){
        case 0:
            return "NULL";

        case 1:
            return "1";

        case 2:
            return "2";

        case 3:
            return "3";

        case 4:
            return "4";
        
        case 5:
            return "5";

        case 6:
            return "6";

        case 7:
            return "7";

        case 8:
            return "8";

        case 9:
            return "9";

        case 10:
            return "10";

        case 11:
            return "11";

        case 12:
            return "12";

        default:
            return "NULL";
    }

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




static void init_list_users(GtkWidget *list) {

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
    column = gtk_tree_view_column_new_with_attributes("Apellido Mat", renderer, "text", 4, NULL);
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



static void add_to_list_users(GtkWidget *list, gchar **str) {
    
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

static void init_list_libros(GtkWidget *list) {

    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    GtkListStore *store;

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Isbn", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nombre", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Editorial", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Numero Ejemplares", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list), column);

    

    store = gtk_list_store_new(4, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);

    gtk_tree_view_set_model(GTK_TREE_VIEW(list), GTK_TREE_MODEL(store));

    g_object_unref(store);
}

static void add_to_list_libros(GtkWidget *list, gchar **str) {
    
    GtkListStore *store;
    GtkTreeIter iter;

    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(list)));

    gtk_list_store_append(store, &iter);

    gtk_list_store_set(store, &iter, 
                    0, str[0],
                    1, str[1],
                    2, str[2],
                    3, str[3],
                    -1);
}

static void on_changed(GtkWidget *widget, gpointer label) {
    
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *value;

  if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

    gtk_tree_model_get(model, &iter, 0, &value,  -1);
    puts(value);
    gtk_label_set_text(GTK_LABEL(label), value);
    g_free(value);
  }

  
}


static void on_changedRentar(GtkWidget *widget, gpointer label) {
    
    GtkTreeIter iter;
    GtkTreeModel *model;
    gchar *isbn, *numEjemplares;

    Library library = (Library) label;

    if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(widget), &model, &iter)) {

        gtk_tree_model_get(model, &iter, 0, &isbn,  -1);
        gtk_tree_model_get(model, &iter, 3, &numEjemplares,  -1);
        setArgumentInsert(library->data, isbn, 0);
        setArgumentInsert(library->data, numEjemplares, 1);

        puts(isbn);
        puts(numEjemplares);
        g_free(isbn);
        g_free(numEjemplares);
        
    }

  
}


static ERROR_CODE searchUserAction(GtkButton *boton, gpointer userData){
    

    Library library = (Library) userData;
    
    setArgumentSearch(library->data,  gtk_entry_get_text(GTK_ENTRY(library->searchEntry)));
    if(searchUser(library->dataBase, library->data) == EMPTY_SET){
        dialogWarning(USER_NOT_FOUND);
        return EMPTY_SET;
    }


    updateList(library, USERS);
    return ERROR_OK;
}

static ERROR_CODE searchLibroAction(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;

    setArgumentSearch(library->data,  gtk_entry_get_text(GTK_ENTRY(library->searchEntry)));
    if(searchLibro(library->dataBase, library->data) == EMPTY_SET){
        dialogWarning(BOOK_NOT_FOUND);
        return EMPTY_SET;
    }


    updateList(library, BOOKS);
    return ERROR_OK;
}


static ERROR_CODE rentarLibroAction(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;
    uint8_t numEjemplares;

    printf("ISBN : -> %s\n", getArgumentInsert(library->data, 0));
    printf("NUM EJEMPLARES : -> %s\n", getArgumentInsert(library->data, 1));

    numEjemplares = atoi(getArgumentInsert(library->data, 1));
    if(numEjemplares == 0){
        dialogWarning(BOOK_STOCK_ERROR);
        clearDataInsert(library->data);
        return STOCK_ERROR;
    }
    

    if(addRenta(library->dataBase, library->data, library->user) != ERROR_OK)
        dialogWarning(RENTA_ERROR);

    clearData(library->data);
    getAllLibros(library->dataBase, library->data);
    updateList(library, BOOKS);
    clearDataInsert(library->data);
    dialogSucces(1);
    return ERROR_OK;
}


static ERROR_CODE updateList(Library library, UPDATES typeUpdate){

    GtkListStore *store;
    GtkTreeModel *model;
    GtkTreeIter  iter;


    store = GTK_LIST_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(library->list)));
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(library->list));




    if (gtk_tree_model_get_iter_first(model, &iter) == FALSE) {
        return MEM_ERROR;
    }
    
    gtk_list_store_clear(store);
    

    if(typeUpdate == USERS){

        for(uint8_t i = 0; i < getDataRows(library->data); i++)
            add_to_list_users(library->list, getRowDataBd(library->data, i));

    }else if(typeUpdate == BOOKS){

        for(uint8_t i = 0; i < getDataRows(library->data); i++)
            add_to_list_libros(library->list, getRowDataBd(library->data, i));
    }

    
    return ERROR_OK;
}



static void ventanaAcercaDe(GtkButton *boton, gpointer userData){
    
    Library library = (Library) userData;
    GtkWidget *window2;
    const char *author[] = {"Jaime Cojab", "Santiago Cuesta", "Santiago Zamora", "\0"};
    library->nextWindow = SALIR;
    window2 = gtk_about_dialog_new();
    
    gtk_window_set_default_size(GTK_WINDOW(window2), 300, 300);
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(window2), "Libreria JSS");
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(window2), "Version 3.1");
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(window2), author);
    gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(window2), author);
    gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(window2), author);
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(window2), "Copyright");
    
    
    g_signal_connect (GTK_DIALOG(window2), "response", G_CALLBACK (on_close), library);

    gtk_widget_show_all(window2);
    gtk_window_set_modal(GTK_WINDOW(window2), TRUE);
}

static void on_close(GtkDialog *dialog, gint response_id, gpointer userData){
  
  /* This will cause the dialog to be destroyed */
  gtk_widget_destroy(GTK_WIDGET(dialog));
}

static void changeWindowFromSubMenuRL(GtkButton *boton, gpointer userData){
    
    Library library = (Library) userData;
    clearData(library->data);
    clearDataInsert(library->data);
    getNextWindow(library, RENTA_LIBROS);
}

static void changeWindowFromSubMenuMP(GtkButton *boton, gpointer userData){
    
    Library library = (Library) userData;
    clearData(library->data);
    clearDataInsert(library->data);
    getNextWindow(library, MENU_PRINCIPAL);
}

static void changeWindowFromSubMenuQT(GtkButton *boton, gpointer userData){

    Library library = (Library) userData;
    getNextWindow(library, SALIR);
}

static void changeWindowFromSubMenuAU(GtkButton *boton, gpointer userData){
    Library library = (Library) userData;
    clearData(library->data);
    clearDataInsert(library->data);
    getNextWindow(library, EDIT_USERS);
}