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

#include "gtk-snippets-loader.h"
 
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

void
gtk_snippets_loader_load_default(GtkSnippetsLoader* loader)
{
	//Leer ficheros .xml
	//Parsear cada uno
	//Generar las listas de snippets
	GtkSnippet *snippets_temp[] = {
		gtk_snippet_new("nombre","C","for","bucle for","for(int i=0;i<10;i++){}"),
		gtk_snippet_new("nombre2","C","if","sentencia if","if .... (int i=0;i<10;i++){}"),
		gtk_snippet_new("nombre3","C","ifero","otra sentencia if","ifero ....(int i=0;i<10;i++){}"),
		gtk_snippet_new("nombre4","C","while","bucle while","while(int i=0;i<10;i++){}")
	};
	int i;
	for (i=0;i<4;i++)
		g_hash_table_insert(loader->priv->snippets_by_name, gtk_snippet_get_mime_type(snippets_temp[i]), snippets_temp[i]);
}

GHashTable*
gtk_snippets_loader_get_snippets(GtkSnippetsLoader* loader)
{
	return NULL;
}

GList*
gtk_snippets_loader_get_snippets_by_name_type(GtkSnippetsLoader* loader, gchar *mime_type)
{
	return NULL;
}
