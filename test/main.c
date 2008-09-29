#include <stdio.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "../gsnippets/gsnippets-parser.h"
#include "../gsnippets/gsnippets-func-manager.h"
#include "../gtksnippets/gtksnippets-varsdialog.h"
#include "../gtksnippets/gtksnippets-inplaceparser.h"

//const gchar* EXAMPLE_TEXT = "${name::Nombre} is very beautiful, \n${name::Nombre222222} is the best.\n${--------\n${\n Lest's go \nto ${city}, ${number}!!!!!\nPosition:>${0}<---\n${";
const gchar* EXAMPLE_TEXT = "dos${dos:2:Dos}\nuno mays: ${uno:1:Uno}\nsinindex${sinindex}\ntres${tres:3:Tres}\ndos${dos:2:Dos}\nuno up: ${uno:6:Uno#upper}\ntres${tres:3:Tres}\nuno low: ${uno:6:Uno#lower}\nuno low: ${uno:6:Uno#camel}";

static GtkTextView *view;
static GtkSnippetsInPlaceParser *parser = NULL;

static gchar* miupper_func (GList *args,
				const gchar *value,
                                GError **error)
{

	return g_strdup_printf("Trans: %s",value);
}

static gchar* micamel_func (GList *args,
				const gchar *value,
                                GError **error)
{
	gchar *temp = g_strdup(value);
	gchar *orig = temp;
	gboolean found = TRUE;
	while(temp[0] != NULL && temp[0] != '\0' )
	{
		g_debug("letra: [%s]",temp);
		if (found)
		{
			g_debug("--");
			found = FALSE;
			g_debug("antes up");
			temp[0] = g_ascii_toupper(temp[0]);
			g_debug("des up");
		}
		g_debug("antes 0 ");
		if (temp[0] == ' ' || temp[0] == '_')
			found = TRUE;
		g_debug("antes ++");
		temp++;
		g_debug("des ++");
	}
	return orig;
}


static void
start_cb(GtkSnippetsInPlaceParser *parser, gpointer user_data)
{
	g_debug("start parsing");
}

static void
end_cb(GtkSnippetsInPlaceParser *parser, gpointer user_data)
{
	g_debug("end parsing");
}

static void
destroy_cb(GtkObject *object, gpointer user_data)
{
	gtk_main_quit();
}
static void
activate_cb(GtkWidget action,gpointer user_data)
{
	if (parser==NULL)
	{
		parser = gtksnippets_inplaceparser_new(view);
		g_signal_connect(parser,"parser-start",start_cb,NULL);
		g_signal_connect(parser,"parser-end",end_cb,NULL);
	}

	gtksnippets_inplaceparser_deactivate(parser);
	g_debug("boton activado");
	
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

	/*Func test*/
	gchar *res = gsnippets_func_manager_parse_text("upper",
					  NULL,
					  "un texto",
					  NULL);
	g_debug("Res func upper: %s",res);
	g_free(res);
	
	res = gsnippets_func_manager_parse_text("lower",
					  NULL,
					  "MAYÚSCULAS",
					  NULL);
	g_debug("Res func lower: %s",res);
	g_free(res);

	res = gsnippets_func_manager_parse_text("camel",
					  NULL,
					  " poner    a caaaaaamel",
					  NULL);
	g_debug("Res func lower: %s",res);
	g_free(res);
	
	gsnippets_func_manager_register_func("miupper",miupper_func);
	res = gsnippets_func_manager_parse_text("miupper",
					  NULL,
					  "un texto",
					  NULL);
	g_debug("Res func miupper: %s",res);
	g_free(res);
	gsnippets_func_manager_register_func("micamel",micamel_func);
	res = gsnippets_func_manager_parse_text("micamel",
					  NULL,
					  " un    texto en camel_aaa_bbbb",
					  NULL);
	g_debug("Res func micamel: %s",res);
	g_free(res);
	GError *error = NULL;
	res = gsnippets_func_manager_parse_text("sssss",
					  NULL,
					  "un texto",
					  &error);
	g_free(res);
	g_debug("un error %s",error->message);
	g_error_free(error);
	
	/*Main test*/
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

