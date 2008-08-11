#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <gtk/gtk.h>
#include "../gsnippets/gsnippets-parser.h"
#include "../gtksnippets/gtksnippets-varsdialog.h"
#include "../gtksnippets/gtksnippets-inplaceparser.h"


typedef struct{
	gint numero;
	gchar *nombre;
	gchar *func;
}Var;

static gboolean
procesar_variable(gchar* var_text, Var *var)
{
	gchar **parts = g_strsplit (var_text, ":", 3);
	gboolean ok = TRUE;
	if (parts[0]==NULL || g_strcmp0(parts[0],"")==0)
	{
		g_warning("Nombre de la variable vacio");
		ok = FALSE;
	}
	
	var->nombre = g_strdup(parts[0]);
	
	if (ok && parts[1]!=NULL && g_strcmp0(parts[1],"")!=0)
	{
		if (strlen(parts[1]) != 1 || !g_ascii_isdigit(*parts[1]))
		{
			g_warning("numero no es numero o es mayor de 1 digito");
			ok = FALSE;
		}
		var->numero = atoi(parts[1]);
	}
	
	if (ok && parts[2]!=NULL)
	{
		g_warning("Demasiados separadores...");
		ok = FALSE;
	}
	
	g_strfreev (parts);
	return ok;
}

static gboolean
procesar_funcion(gchar* func_text, Var *var)
{
	var->func = g_strdup(func_text);
	return TRUE;
}


static void
test_split(const gchar *text)
{
	Var var = {-1,NULL,NULL};
	g_debug("Nuevo split de %s", text);
	
	gchar **parts = g_strsplit (text, "#", 3);
	if (parts[0] == NULL || strlen(parts[0]) < 1)
	{
		g_warning("Variable vacia.");
		return;
	}
	procesar_variable(parts[0],&var);
	
	if (parts[1]!=NULL)
		procesar_funcion(parts[1],&var);
	
	g_debug("Var: numero=%i, nombre=%s, funcion=%s",var.numero,var.nombre,var.func);
	g_free(var.nombre);
	g_free(var.func);
	g_strfreev (parts);
}

/*
 * 1.- Coger las variables
 * 2.- Pedir los valores al usuario
 * 3.- Sustituir las variables
 * 4.- Interpretar las instrucciones
 */
int main( int argc, const char* argv[] )
{
	
	test_split("");
	test_split("asdfasd:1");
	test_split(":asdfasd");
	test_split("asdfasd");
	test_split("asdfasd:");
	test_split("asdfasd:1:2:3:4");
	test_split("asdfasd:1#funcion");
	test_split("asdfasd:1#funcion:arg1:arg2");
	test_split("asdfasd:1#funcion#dsfasd#safasdf");
	//test_inplaceparser();
	
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
