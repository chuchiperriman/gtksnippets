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
#include <string.h>
#include <gtksourceview/gtksourcelanguagesmanager.h>
#include "gtk-snippets-loader.h"

//#define DEFAULT_SNIPPETS_DIR SNIPPETS_DIR

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
	//Hash <language,snippets_List>
	GHashTable *language_hash;
	gint snippets_count;
	gchar* default_path;
	
};

typedef struct _GtkSnippetsLoaderSignal GtkSnippetsLoaderSignal;
typedef enum _GtkSnippetsLoaderSignalType GtkSnippetsLoaderSignalType;

enum _GtkSnippetsLoaderSignalType {
	// Place Signal Types Here 
	SIGNAL_TYPE_SNIPPETS_CHANGED,
	LAST_SIGNAL
};

static guint gtk_snippets_loader_signals[LAST_SIGNAL] = { 0 };

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
gtk_snippets_loader_destroy_snippets_list(gpointer data)
{
	GtkSnippet *snippet;
	GList *next;
	GList *actual;
	if (data!=NULL)
	{
		actual = data;
		do
		{
			snippet = GTK_SNIPPET(actual->data);
			g_debug("Destroy en list destroy");
			g_object_unref(snippet);
			g_debug("fin Destroy en list destroy");
			next = g_list_next(actual);
		}while((actual = next) != NULL);
		
		g_list_free(data);
	}
}

static gchar*
gsl_get_default_path()
{
	gchar path[255];
	
	//TODO not portable
	sprintf(path,"%s/.gnome2/libgtksnippets/snippets",g_get_home_dir());
	
	if (!g_file_test(path , G_FILE_TEST_EXISTS | G_FILE_TEST_IS_DIR))
	{
		if (g_mkdir_with_parents(path,0755)==0)
			g_debug("Folder created: %s",path);
		else
			g_debug("Cannot create the home folder: %s",path);
	}
	
	return g_strdup(path);
}

static void
gtk_snippets_loader_class_init(GtkSnippetsLoaderClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gtk_snippets_loader_finalize;
	
 	//Señales
	gtk_snippets_loader_signals[SIGNAL_TYPE_SNIPPETS_CHANGED] = 
			g_signal_new("snippets-changed",
					G_TYPE_FROM_CLASS(klass),
					G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
					0,
					NULL,
					NULL,
					g_cclosure_marshal_VOID__VOID,
					G_TYPE_NONE,
					0);
}

static void
gtk_snippets_loader_init(GtkSnippetsLoader *obj)
{
	obj->priv = g_new0(GtkSnippetsLoaderPrivate, 1);
	/* Initialize private members, etc. */
	obj->priv->language_hash = g_hash_table_new_full(g_str_hash, g_str_equal , NULL, gtk_snippets_loader_destroy_snippets_list);
	obj->priv->snippets_count = 0;
	obj->priv->default_path = gsl_get_default_path();
	g_debug("Construido snippets loader");
}

static void
gtk_snippets_loader_finalize(GObject *object)
{
	GtkSnippetsLoader *cobj;
	cobj = GTK_SNIPPETSLOADER(object);
	
	/* Free private members, etc. */
	g_hash_table_destroy(cobj->priv->language_hash);
	g_free(cobj->priv->default_path);
	
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
			
	gtk_snippets_loader_add_snippet(loader, snippet);
		
	xmlFree(name);
	xmlFree(tag);
	xmlFree(description);
	xmlFree(text);
	
	g_debug("XML Snippet found");
	
	return res;
}

/**
 * gsl_parse_root:
 * @loader: The loader
 * @a_node: The root node
 * @language: snippets language. If NULL then get the language xml attribute
 * @Returns: TRUE if all works fine or FALSE if not.
 *
 * This function parse the root element, gets the language and call
 * to #gsl_parse_snippet to parse every snippet.
 */
static gboolean
gsl_parse_root(GtkSnippetsLoader* loader, xmlNode * a_node, gchar *language)
{
	xmlNode *cur_node = NULL;
	xmlChar *value=NULL;
	gchar *lang_fin;
	gboolean res = TRUE;
	
	if (language == NULL)
	{
		value = xmlGetProp(a_node, ATT_LANGUAGE);
		lang_fin = (gchar*)value;
	}
	else
	{
		lang_fin = language;
	}
	
	if (language != NULL)
	{
		g_debug("attribute language: %s",language);
		for (cur_node = a_node->children; cur_node; cur_node = cur_node->next)
		{
			if (cur_node->type == XML_ELEMENT_NODE)
			{
				if (xmlStrcmp(cur_node->name, TAG_SNIPPET)==0)
				{
					if (!gsl_parse_snippet(loader,cur_node, lang_fin))
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

gboolean
gtk_snippets_loader_load_from_file(GtkSnippetsLoader* loader, const gchar *file, gchar* language)
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
			
			res = gsl_parse_root(loader, root_element, language);
			
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

gboolean
gtk_snippets_loader_load_from_dir(GtkSnippetsLoader* loader,const gchar *path)
{
	GDir *dir;
	gchar* language;
	const gchar* path_file;
	gboolean res;
	GtkSourceLanguagesManager* lang_manager;
	GtkSourceLanguage *lang;
	const GSList *lang_list;
	
	lang_manager = gtk_source_languages_manager_new();
	lang_list = gtk_source_languages_manager_get_available_languages(lang_manager);
	
	res = TRUE;
	
	dir = g_dir_open(path, 0, NULL);
	if (dir == NULL)
	{
		g_warning("Cannot open dir %s",path);
		res = FALSE;
	}
	
	
	if (res)
	{
		while ((lang_list = g_slist_next(lang_list)) != NULL)
		{
			lang = GTK_SOURCE_LANGUAGE(lang_list->data);
			language = gtk_source_language_get_name(lang);
			path_file = g_strconcat(path, "/", language, ".xml", NULL);
			res = gtk_snippets_loader_load_from_file(loader, path_file, language); 
			if (!res)
			{
				g_warning("Error loading file %s",path_file); 
			}
		}
		
		g_dir_close(dir);
	}
	
	return res;
	
}

void
gtk_snippets_loader_load_default(GtkSnippetsLoader* loader)
{
	gtk_snippets_loader_load_from_dir(loader,loader->priv->default_path);
	g_debug("Load default end");
}

GHashTable*
gtk_snippets_loader_get_snippets(GtkSnippetsLoader* loader)
{
	return loader->priv->language_hash;
}

GList*
gtk_snippets_loader_get_snippets_by_language(GtkSnippetsLoader* loader, const gchar* language)
{
	return (GList*)g_hash_table_lookup(loader->priv->language_hash,language);
}

//Se llama por cada snippet que hay en la lista cuando grabamos
static void
gsl_hash_for_each_generate_snippet_xml (gpointer key,
		gpointer value,
		gpointer user_data)
{
	GList* lista;
	xmlNodePtr root_node = NULL, snippet_node = NULL, text_node = NULL, cdata_node = NULL;
	xmlDocPtr doc = NULL;
	GtkSnippet* snippet;
	const gchar* content;
	gchar fich[255];
	gchar* language;
	GtkSnippetsLoader* loader;
	
	loader = GTK_SNIPPETSLOADER(user_data);
	
	if (value != NULL)
	{
		lista = (GList*)value;
		
		language = (gchar*)key;
		
		doc = xmlNewDoc(BAD_CAST "1.0");
		root_node = xmlNewNode(NULL, BAD_CAST "snippets");
		xmlNewProp(root_node, BAD_CAST "language", BAD_CAST language);
		xmlDocSetRootElement(doc, root_node);
		
		do
		{
			g_assert(lista->data != NULL);
			
			
    
			snippet = GTK_SNIPPET(lista->data);
			
			snippet_node = xmlNewChild(root_node, NULL, BAD_CAST "snippet",NULL);
			xmlNewProp(snippet_node, BAD_CAST "id", BAD_CAST gtk_snippet_get_name(snippet) );
		
			xmlNewChild(snippet_node, NULL, BAD_CAST "tag", BAD_CAST gtk_snippet_get_tag(snippet));
			//TODO falta la descripcion
			xmlNewChild(snippet_node, NULL, BAD_CAST "description", BAD_CAST "Snippet description" );
			
			text_node = xmlNewChild(snippet_node, NULL, BAD_CAST "text", NULL);
			
			content = gtk_snippet_get_text(snippet);
			cdata_node = xmlNewCDataBlock(doc, BAD_CAST content, strlen(content));
			xmlAddChild(text_node,cdata_node);
			
		}while((lista = g_list_next(lista))!=NULL);

		sprintf(fich,"%s/%s.xml", loader->priv->default_path, language );
		
		xmlSaveFormatFileEnc(fich, doc, "UTF-8", 1);

		/*free the document */
		xmlFreeDoc(doc);
		
	}
	
	
	/*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();

    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();

}


gboolean
gtk_snippets_loader_save_default(GtkSnippetsLoader* loader)
{

	//TODO Ñapa de función

	/*
	* 1.- Graba los snippets en disco
	* 2.- Señal indicando que los snippets han cambiado
	*/
	
	
	GHashTable *snippets = gtk_snippets_loader_get_snippets(loader);
	
	if (snippets != NULL)
	{
		//TODO Now we save all the documents and all the snippets!!!!!!!
		//We must detect what snippets have been changed and only write these on disk
		g_hash_table_foreach (snippets, gsl_hash_for_each_generate_snippet_xml, loader);
		
		g_signal_emit(
				loader,
				gtk_snippets_loader_signals[SIGNAL_TYPE_SNIPPETS_CHANGED],
				0);
				
	}
	else
	{
		g_debug("No hay snippets para grabar???");
	}
	
	
	return TRUE;
    
}

void
gtk_snippets_loader_remove_snippet(GtkSnippetsLoader* loader, GtkSnippet* snippet)
{
	gchar* language;
	GList* snippets_list;
	GList* new_start;
	
	g_debug("Liberamos el snippet: %s", gtk_snippet_get_tag(snippet));
	
	language = g_strdup(gtk_snippet_get_language(snippet));
	snippets_list = (GList*)g_hash_table_lookup(loader->priv->language_hash,language);
	
	g_assert(snippets_list != NULL);
	
	new_start = g_list_remove(snippets_list,snippet);
	g_debug("Se quitó de la lista");
	if (new_start != snippets_list)
	{
		g_hash_table_steal(loader->priv->language_hash,language);
		g_debug("ositas: %s",language);
		g_hash_table_insert(loader->priv->language_hash,language,new_start);
	}
	g_debug("Destroy en remove loader");
	
	g_object_unref(snippet);
	g_debug("fin Destroy en remove loader");
	
	//No lo liberamos porque se queda en el hash_table
	//g_free(language);
	
	//TODO raise signal
	
}

void
gtk_snippets_loader_add_snippet(GtkSnippetsLoader* loader, GtkSnippet* snippet)
{
	const gchar* language;
	GList* snippets_list;
	GList* snippet_added;
	
	g_assert(snippet!=NULL);

	language = gtk_snippet_get_language(snippet);

	g_return_if_fail(language != NULL);	
	
	snippets_list = (GList*) g_hash_table_lookup(loader->priv->language_hash,language);
	snippet_added = g_list_append(snippets_list, snippet);
	//Si no tenía lista, la añadimos al hash
	if (snippets_list == NULL)
		g_hash_table_insert(loader->priv->language_hash, (gchar*)language, snippet_added);	
	
	
}

