/***************************************************************************
 *            gtk-snippets-loader.h
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
 *
 * 
 *  Este objeto será el encargado de cargar los datos de los snippets de 
 *  la base de datos de snippets (una base de datos, un xml etc, dependerá
 *  de la implementación)
 *
 */

#ifndef GTK_SNIPPETS_LOADER_H
#define GTK_SNIPPETS_LOADER_H

#include <glib.h>
#include <glib-object.h>
#include "gtk-snippet.h"

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETSLOADER         (gtk_snippets_loader_get_type ())
#define GTK_SNIPPETSLOADER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_SNIPPETSLOADER, GtkSnippetsLoader))
#define GTK_SNIPPETSLOADER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GTK_TYPE_SNIPPETSLOADER, GtkSnippetsLoaderClass))
#define GTK_IS_SNIPPETSLOADER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_SNIPPETSLOADER))
#define GTK_IS_SNIPPETSLOADER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GTK_TYPE_SNIPPETSLOADER))
#define GTK_SNIPPETSLOADER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GTK_TYPE_SNIPPETSLOADER, GtkSnippetsLoaderClass))

typedef struct _GtkSnippetsLoader GtkSnippetsLoader;
typedef struct _GtkSnippetsLoaderPrivate GtkSnippetsLoaderPrivate;
typedef struct _GtkSnippetsLoaderClass GtkSnippetsLoaderClass;

struct _GtkSnippetsLoader {
	GObject parent;
	GtkSnippetsLoaderPrivate *priv;
};

struct _GtkSnippetsLoaderClass {
	GObjectClass parent_class;
	/* Add Signal Functions Here */
};

GType gtk_snippets_loader_get_type();
/**
* Crea un nuevo cargador de snippets pero no carga los snippets. 
*/
GtkSnippetsLoader *gtk_snippets_loader_new();

/**
* Carga los snippets por defecto del sistema
*/
void
gtk_snippets_loader_load_default(GtkSnippetsLoader* loader);


/**
* Devuelve FALSE si no puede cargarlos
* TODO: Ver cómo tratar mejor los errores
*/
gboolean
gtk_snippets_loader_load_from_file(GtkSnippetsLoader* loader, const gchar *file, gchar* language);


/**
* Lee todos los ficheros .xml del directorio
* Devuelve FALSE si no puede cargarlos
* TODO: Ver cómo tratar mejor los errores
*/
gboolean
gtk_snippets_loader_load_from_dir(GtkSnippetsLoader* loader, const gchar *path);

/**
* Devuelve todos los snippets. NULL si no hay o no se han cargado.
* El hash es <language,snippets GList >
* Returns: An internal GHashTable<language,snippets GList>. Not free it!!!
*/
GHashTable*
gtk_snippets_loader_get_snippets(GtkSnippetsLoader* loader);

/**
* Devuelve todos los snippets de un determinado lenguaje
**/
GList*
gtk_snippets_loader_get_snippets_by_language(GtkSnippetsLoader* loader, const gchar* language);

/**
* The snippet is unref
**/
void
gtk_snippets_loader_remove_snippet(GtkSnippetsLoader* loader, GtkSnippet* snippet);

G_END_DECLS

#endif /* GTK_SNIPPETS_LOADER_H */
