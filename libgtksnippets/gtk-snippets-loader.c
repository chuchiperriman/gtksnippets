/***************************************************************************
 *            gtk-snippets-loader.c
 *
 *  mar ene  9 18:04:32 2007
 *  Copyright  2007  chuchiperriman
 *  chuchiperriman@gmail.com
 ***************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <glib/gprintf.h>
#include <libxml/xmlreader.h>
#include "gtk-snippets-loader.h"

#define DEFAULT_SNIPPETS_DIR SNIPPETS_DIR

#define TAG_SNIPPETS (const xmlChar *)"snippets"
#define ATT_LANGUAGE (const xmlChar *)"language"
#define ATT_ID (const xmlChar *)"id"
#define TAG_SNIPPET (const xmlChar *)"snippet"
#define TAG_TAG (const xmlChar *)"tag"
#define TAG_DESCRIPTION (const xmlChar *)"description"
#define TAG_TEXT (const xmlChar *)"text"
 
static void gtk_snippets_loader_class_init(GtkSnippetsLoaderClass *klass);
static void gtk_snippets_loader_init(GtkSnippetsLoader *sp);
static void gtk_snippets_loader_finalize(GObject *object);

struct _GtkSnippetsLoaderPrivate {
	/* Place Private Members Here */
	GHashTable *snippets_by_name;
};

typedef struct _GtkSnippetsLoaderSignal GtkSnippetsLoaderSignal;
typedef enum _GtkSnippetsLoaderSignalType GtkSnippetsLoaderSignalType;

/*enum _GtkSnippetsLoaderSignalType {
	// Place Signal Types Here 
	SIGNAL_TYPE_EXAMPLE,
	LAST_SIGNAL
};

struct _GtkSnippetsLoaderSignal {
	GtkSnippetsLoader *object;
};

static guint gtk_snippets_loader_signals[LAST_SIGNAL] = { 0 };*/
static GObjectClass *parent_class = NULL;

GType
gtk_snippets_loader_get_type()
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo our_info = {
			sizeof (GtkSnippetsLoaderClass),
			NULL,
			NULL,
			(GClassInitFunc)gtk_snippets_loader_class_init,
			NULL,
			NULL,
			sizeof (GtkSnippetsLoader),
			0,
			(GInstanceInitFunc)gtk_snippets_loader_init,
		};

		type = g_type_register_static(G_TYPE_OBJECT, 
			"GtkSnippetsLoader", &our_info, 0);
	}

	return type;
}

static void
gtk_snippets_loader_destroy_snippet(gpointer data)
{
	if (data!=NULL)
	{
		g_object_unref(data);
	}
}

static void
gtk_snippets_loader_class_init(GtkSnippetsLoaderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gtk_snippets_loader_finalize;
	
	/* Create signals here:
	   gtk_snippets_loader_signals[SIGNAL_TYPE_EXAMPLE] = g_signal_new(...)
 	*/
}

static void
gtk_snippets_loader_init(GtkSnippetsLoader *obj)
{
	obj->priv = g_new0(GtkSnippetsLoaderPrivate, 1);
	/* Initialize private members, etc. */
	obj->priv->snippets_by_name = g_hash_table_new_full(g_str_hash,g_str_equal,NULL,gtk_snippets_loader_destroy_snippet);
	g_debug("Construido snippets loader");
}

static void
gtk_snippets_loader_finalize(GObject *object)
{
	GtkSnippetsLoader *cobj;
	cobj = GTK_SNIPPETSLOADER(object);
	
	/* Free private members, etc. */
	g_hash_table_destroy(cobj->priv->snippets_by_name);
	g_free(cobj->priv);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	g_debug("Destruido snippets loader");
}

GtkSnippetsLoader *
gtk_snippets_loader_new()
{
	GtkSnippetsLoader *obj;
	
	obj = GTK_SNIPPETSLOADER(g_object_new(GTK_TYPE_SNIPPETSLOADER, NULL));
	
	return obj;
}

static gboolean
gsl_parse_snippet(GtkSnippetsLoader* loader, xmlNode *a_node)
{
	g_debug("XML Snippet found");
	return TRUE;
}

static gboolean
gsl_parse_root(GtkSnippetsLoader* loader, xmlNode * a_node)
{
	xmlChar *value;
	xmlNode *cur_node = NULL;
	gboolean res = TRUE;
	
	value = xmlGetProp(a_node, ATT_LANGUAGE);
	
	if (value != NULL)
	{
		g_debug("attribute language: %s",value);
		for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
		{
			if (cur_node->type == XML_ELEMENT_NODE)
			{
				if (xmlStrcmp(cur_node->name, TAG_SNIPPET)==0)
				{
					if (!gsl_parse_snippet(loader,cur_node))
					{
						res = FALSE;
						break;
					}
				}
			}
			
		}
	}
	else
	{
		g_warning("snippets xml have not a language attribute!");
		res = FALSE;
	}
	
	return res;
}

/**
* Devuelve FALSE si no puede cargarlos
* TODO: Ver cómo tratar mejor los errores
*/
gboolean
gtk_snippets_loader_load_from_file(GtkSnippetsLoader* loader, gchar *file)
{
	xmlDoc *doc = NULL;
	xmlNode *root_element = NULL;
	gboolean res = TRUE;
	
	if (!g_file_test (file, G_FILE_TEST_EXISTS | G_FILE_TEST_IS_REGULAR)) 
	{
		g_debug("File not found: %s",file);
		res =  FALSE;
	}
	else
	{
		doc = xmlReadFile(file, NULL, 0);
		
		if (doc!=NULL)
		{
			root_element = xmlDocGetRootElement(doc);
			
			res = gsl_parse_root(loader,root_element);
			
			xmlFreeDoc(doc);
			xmlCleanupParser();
		}
		else
		{
			g_debug("Cannot load the file: %s", file);
			res = FALSE;
		}
	}	
	return res;
	
}

void
gtk_snippets_loader_load_default(GtkSnippetsLoader* loader)
{
	//Leer ficheros .xml
	//Parsear cada uno
	//Generar las listas de snippets
	
	
	//TODO AHORA ESTAN PUESTAS COCHINADAS PARA DEPURAR
	gint num_snippets = 50;
	gchar temp[255];
	gchar temp2[255];
	gchar temp3[255];
	int i;
	for (i=0;i<num_snippets;i++){
		g_sprintf(temp,"nombre%d",i);
		g_sprintf(temp2,"descripcion%d",i);
		g_sprintf(temp3,"sentencia%d",i);
		g_hash_table_insert(
			loader->priv->snippets_by_name,
			g_strdup(temp),
			(gpointer)gtk_snippet_new(g_strdup(temp),"C",g_strdup(temp),g_strdup(temp2),g_strdup(temp3)));
	}
	
	for (i=0;i<num_snippets;i++){
		g_sprintf(temp,"aaanombre%d",i);
		g_sprintf(temp2,"aaadescripcion%d",i);
		g_sprintf(temp3,"aaasentencia%d",i);
		g_hash_table_insert(
			loader->priv->snippets_by_name,
			g_strdup(temp),
			(gpointer)gtk_snippet_new(g_strdup(temp),"C",g_strdup(temp),g_strdup(temp2),g_strdup(temp3)));
	}
	
	for (i=0;i<num_snippets;i++){
		g_sprintf(temp,"PYTHONnombre%d",i);
		g_sprintf(temp2,"PYTHONdescripcion%d",i);
		g_sprintf(temp3,"PYTHONsentencia%d",i);
		g_hash_table_insert(
			loader->priv->snippets_by_name,
			g_strdup(temp),
			(gpointer)gtk_snippet_new(g_strdup(temp),"PYTHON",g_strdup(temp),g_strdup(temp2),g_strdup(temp3)));
	}
	
	
	gtk_snippets_loader_load_from_file(loader,SNIPPETS_DIR"/c.xml");
	
}

GHashTable*
gtk_snippets_loader_get_snippets(GtkSnippetsLoader* loader)
{
	return loader->priv->snippets_by_name;
}

GList*
gtk_snippets_loader_get_snippets_by_name_type(GtkSnippetsLoader* loader, gchar *mime_type)
{
	return NULL;
}
