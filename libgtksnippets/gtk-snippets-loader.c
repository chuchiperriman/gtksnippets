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
#include <glib/gstdio.h>
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
	GHashTable *snippets_hash;
	gint snippets_count;
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
	obj->priv->snippets_hash = g_hash_table_new_full(g_int_hash,g_int_equal, g_free, gtk_snippets_loader_destroy_snippet);
	obj->priv->snippets_count = 0;
	g_debug("Construido snippets loader");
}

static void
gtk_snippets_loader_finalize(GObject *object)
{
	GtkSnippetsLoader *cobj;
	cobj = GTK_SNIPPETSLOADER(object);
	
	/* Free private members, etc. */
	g_hash_table_destroy(cobj->priv->snippets_hash);
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
gsl_parse_snippet(GtkSnippetsLoader* loader, xmlNode *a_node, gchar* language)
{
	xmlChar *name = NULL;
	xmlChar *tag = NULL;
	xmlChar *description = NULL;
	xmlChar *text = NULL;
	xmlNode *cur_node = NULL;
	gboolean res = TRUE;
	gint* key;
	GtkSnippet *snippet;
	
	name = xmlGetProp(a_node, ATT_ID);
	
	for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
	{
		if (cur_node->type == XML_ELEMENT_NODE)
		{
			if (xmlStrcmp(cur_node->name, TAG_TAG)==0)
			{
				//Guardamos el dato en el objeto servidor actual
				tag = xmlNodeGetContent(cur_node);
			}else if (xmlStrcmp(cur_node->name, TAG_DESCRIPTION)==0)
			{
				description = xmlNodeGetContent(cur_node);
			}else if (xmlStrcmp(cur_node->name, TAG_TEXT)==0)
			{
				text = xmlNodeGetContent(cur_node);
			}
		}
	}
	
	//TODO Buscar una forma mejor de liberar
	if (name == NULL || tag == NULL || description == NULL || text == NULL)
	{
		g_warning("Snippet without some element");
		xmlFree(name);
		xmlFree(tag);
		xmlFree(description);
		xmlFree(text);
		return FALSE;
	}
	
	snippet = gtk_snippet_new(
			(gchar*)name,
			language,
			(gchar*)tag,
			(gchar*)description,
			(gchar*)text);
			
	//TODO liberar esto
	key = g_malloc0(sizeof(gint*));
	*key = loader->priv->snippets_count++;
	
	g_hash_table_insert(
			loader->priv->snippets_hash,
			key,
			(gpointer)snippet);
	
	xmlFree(name);
	xmlFree(tag);
	xmlFree(description);
	xmlFree(text);
	
	g_debug("XML Snippet found");
	
	return res;
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
					if (!gsl_parse_snippet(loader,cur_node, (gchar*)value))
					{
						res = FALSE;
						break;
					}
				}
			}
		}
		xmlFree(value);
	}
	else
	{
		g_warning("snippets xml have not a language attribute!");
		res = FALSE;
	}
	
	g_debug("Fin parse root");
	return res;
}

/**
* Devuelve FALSE si no puede cargarlos
* TODO: Ver cómo tratar mejor los errores
*/
gboolean
gtk_snippets_loader_load_from_file(GtkSnippetsLoader* loader, const gchar *file)
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
			
			g_debug("Fin limpieza xml");
		}
		else
		{
			g_debug("Cannot load the file: %s", file);
			res = FALSE;
		}
	}
	
	return res;
	
}

/**
* Lee todos los ficheros .xml del directorio
* Devuelve FALSE si no puede cargarlos
* TODO: Ver cómo tratar mejor los errores
*/
gboolean
gtk_snippets_loader_load_from_dir(GtkSnippetsLoader* loader,const gchar *path)
{
	GDir *dir;
	const gchar* file;
	const gchar* path_file;
	gboolean res;
	
	res = TRUE;
	
	dir = g_dir_open(path, 0, NULL);
	if (dir == NULL)
	{
		g_warning("Cannot open dir %s",path);
		res = FALSE;
	}
	
	if (res)
	{
		while ((file = g_dir_read_name(dir)) != NULL)
		{
			//TODO Ver cómo obtener el separador de directorio
			path_file = g_strconcat(path, "/", file, NULL);
			res = gtk_snippets_loader_load_from_file(loader, path_file); 
			if (!res)
			{
				g_warning("Error loading file %s",file); 
			}
		}
		g_dir_close(dir);
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
	/*gint num_snippets = 50;
	gchar temp[255];
	gchar temp2[255];
	gchar temp3[255];
	int i;
	for (i=0;i<num_snippets;i++){
		g_sprintf(temp,"nombre%d",i);
		g_sprintf(temp2,"descripcion%d",i);
		g_sprintf(temp3,"sentencia%d",i);
		g_hash_table_insert(
			loader->priv->snippets_hash,
			g_strdup(temp),
			(gpointer)gtk_snippet_new(g_strdup(temp),"C",g_strdup(temp),g_strdup(temp2),g_strdup(temp3)));
	}
	
	for (i=0;i<num_snippets;i++){
		g_sprintf(temp,"aaanombre%d",i);
		g_sprintf(temp2,"aaadescripcion%d",i);
		g_sprintf(temp3,"aaasentencia%d",i);
		g_hash_table_insert(
			loader->priv->snippets_hash,
			g_strdup(temp),
			(gpointer)gtk_snippet_new(g_strdup(temp),"C",g_strdup(temp),g_strdup(temp2),g_strdup(temp3)));
	}
	
	for (i=0;i<num_snippets;i++){
		g_sprintf(temp,"PYTHONnombre%d",i);
		g_sprintf(temp2,"PYTHONdescripcion%d",i);
		g_sprintf(temp3,"PYTHONsentencia%d",i);
		g_hash_table_insert(
			loader->priv->snippets_hash,
			g_strdup(temp),
			(gpointer)gtk_snippet_new(g_strdup(temp),"PYTHON",g_strdup(temp),g_strdup(temp2),g_strdup(temp3)));
	}
	
	*/
	gtk_snippets_loader_load_from_dir(loader,SNIPPETS_DIR);
	
}

GHashTable*
gtk_snippets_loader_get_snippets(GtkSnippetsLoader* loader)
{
	return loader->priv->snippets_hash;
}
