/***************************************************************************
 *            gtk-snippet.h
 *
 *  mié ene 10 10:15:00 2007
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

#ifndef GTK_SNIPPET_H
#define GTK_SNIPPET_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPET         (gtk_snippet_get_type ())
#define GTK_SNIPPET(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_SNIPPET, GtkSnippet))
#define GTK_SNIPPET_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GTK_TYPE_SNIPPET, GtkSnippetClass))
#define GTK_IS_SNIPPET(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_SNIPPET))
#define GTK_IS_SNIPPET_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GTK_TYPE_SNIPPET))
#define GTK_SNIPPET_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GTK_TYPE_SNIPPET, GtkSnippetClass))

typedef struct _GtkSnippet GtkSnippet;
typedef struct _GtkSnippetPrivate GtkSnippetPrivate;
typedef struct _GtkSnippetClass GtkSnippetClass;

struct _GtkSnippet {
	GObject parent;
	GtkSnippetPrivate *priv;
};

struct _GtkSnippetClass {
	GObjectClass parent_class;
	/* Add Signal Functions Here */
};

GType gtk_snippet_get_type();

GtkSnippet*
gtk_snippet_new(gchar *name,
				gchar *language,
				gchar *tag,
				gchar *description,
				gchar *text);
				
gchar*
gtk_snippet_get_language(GtkSnippet *snippet);

gchar*
gtk_snippet_get_name(GtkSnippet *snippet);


G_END_DECLS

#endif /* GTK_SNIPPET_H */
