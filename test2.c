#include <gtk/gtk.h>



int main(int argc, char *argv[]) {
GtkWidget *window;
GtkWidget *grid;
GtkWidget *image;
GtkWidget *title;
GtkWidget *author;
GtkWidget *duration;

gtk_init(&argc,&argv);

window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_window_set_default_size(GTK_WINDOW(window), 1024,600);

grid = gtk_grid_new();
gtk_grid_insert_row(GTK_GRID(grid),0);
gtk_grid_set_row_spacing(GTK_GRID(grid),12);

image = gtk_image_new_from_file("./default.jpg");

title = gtk_label_new("this is some arbitrary title");

gtk_container_add(GTK_CONTAINER(window), grid);

//gtk_container_add(GTK_CONTAINER(grid), title);
gtk_container_add(GTK_CONTAINER(grid), image);




gtk_widget_show_all( window );

//if kill
g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
gtk_main();
return 0;

}

