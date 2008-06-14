#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "../gsnippets/gsnippets-parser.h"
#include "../gtksnippets/gtksnippets-varsdialog.h"
#include "../gtksnippets/gtksnippets-inplaceparser.h"

const gchar* EXAMPLE_TEXT = "${name} is very beautiful, \n${name} is the best. Lest's go \nto ${city}, ${number}!!!!!";

static GtkTextView *view;
static GtkSnippetsInPlaceParser *parser = NULL;

static void
destroy_cb(GtkObject *object, gpointer user_data)
{
	gtk_main_quit();
}
static void
activate_cb(GtkWidget action,gpointer user_data)
{
	if (parser!=NULL)
		g_object_unref(parser);
	g_debug("boton activado");
	parser = gtksnippets_inplaceparser_new(view);
	gtksnippets_inplaceparser_activate(parser,EXAMPLE_TEXT);
	gtk_widget_grab_focus(view);
}

static void
test_inplaceparser()
{
	GtkWindow *win;
	GtkScrolledWindow *scroll;
	GtkWidget *box, *button;
	win = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size(win,800,600);
	box = gtk_vbox_new(FALSE,2);
	scroll = gtk_scrolled_window_new(NULL,NULL);
	view = gtk_text_view_new();
	button = gtk_button_new_with_label("Activa");
	gtk_container_add(GTK_CONTAINER(win),GTK_WIDGET(box));
	gtk_box_pack_start(GTK_BOX(box),scroll,TRUE,TRUE,1);
	gtk_box_pack_end(GTK_BOX(box),button,FALSE,FALSE,1);
	gtk_container_add(GTK_CONTAINER(scroll),GTK_WIDGET(view));
	gtk_widget_show_all(GTK_WIDGET(win));

	g_signal_connect(button,"clicked",activate_cb,NULL);
	g_signal_connect(win,"destroy",destroy_cb,NULL);

	gtk_main();
}

/*
 * 1.- Coger las variables
 * 2.- Pedir los valores al usuario
 * 3.- Sustituir las variables
 * 4.- Interpretar las instrucciones
 */
int main( int argc, const char* argv[] )
{
	
	gtk_init(&argc,&argv);
	test_inplaceparser();
	
	/* Vars dialog test
	gchar *content = gtksnippets_varsdialog_show_and_parse(EXAMPLE_TEXT);
	g_debug("Res: %s",content);
	g_free(content);
	*/

	/* Parser test:w
	gchar *text1;
	GSList *vars;
	GSList *temp;
	g_debug("----------------------------");
	text1 = gsnippets_parser_clean_vars(EXAMPLE_TEXT);
	g_debug("Original phrase: %s",EXAMPLE_TEXT);
	g_debug("Clean: %s",text1);
	g_free(text1);
	vars = gsnippets_parser_get_vars(EXAMPLE_TEXT);
	temp = vars;
	GSnippetsVar *var;
	gchar input[254];
	while(temp!=NULL)
	{
		var = (GSnippetsVar*)temp->data;
		printf("\nValue for %s: ",var->name);
		scanf("%s",input);
		var->value = g_strdup(input);
		g_debug("Var name: %s",var->name);
		g_debug("Var value: %s",var->value);
		temp = g_slist_next(temp);
	}
	
	text1 = gsnippets_parser_replace_vars(EXAMPLE_TEXT, vars);
	
	g_debug("Result: %s",text1);
	
	g_free(text1);
	
	gsnippets_parser_free_var_list(vars);
	*/
	
	return 0;	
	
}

