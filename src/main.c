#include <stdio.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <gtksourceview/gtksourceview.h>
#include <gtksourceview/gtksourcebuffer.h>
#include "gtksnippets-manager.h"
#include "gtksnippets-dialog.h"

#define GLADE_FILE GLADE_DIR"/gtksnippets.glade"

GtkWidget*
gtk_smngui_create_source_view()
{
	GtkWidget *source = gtk_source_view_new();
	/*gtk_source_buffer_set_highlight(
		GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(source))),
		TRUE);*/
	gtk_widget_show(source);
	return source;
}

static GtkWidget*
create_window()
{
	GtkWidget *w = gtksnippets_manager_new();
	return w;
}

static void
_window_destroy_cb(GtkWidget *w, gpointer user_data)
{
	gtk_main_quit();
}

static GtkWidget*
create_dialog()
{
	GtkWidget *w = gtksnippets_dialog_new();
	/* Signals */
	g_signal_connect(
		w,
		"destroy",
		G_CALLBACK(_window_destroy_cb),
		NULL);
	g_signal_connect(
		w,
		"hide",
		G_CALLBACK(_window_destroy_cb),
		NULL);
	return w;
}

int
main (int argc, char *argv[])
{
	GtkWidget *widget = NULL;
	printf("Hola mundo: %s\n",GLADE_FILE);
	gtk_init(&argc,&argv);
	/*GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	widget = glade_xml_get_widget (gxml, "smngui_dialog");
	*/
	//widget = create_window();
	widget = create_dialog();
	gtk_widget_show_all(widget);
	
	//g_object_unref(gxml);
        
	gtk_main();
}
