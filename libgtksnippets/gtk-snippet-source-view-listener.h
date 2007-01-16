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

#ifndef GTK_SNIPPET_SOURCE_VIEW_LISTENER_H
#define GTK_SNIPPET_SOURCE_VIEW_LISTENER_H

#include <glib.h>
#include <glib-object.h>
#include <gtksourceview/gtksourceview.h>
#include "gtk-snippets-loader.h"

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER         (gtk_snippet_source_view_listener_get_type ())
#define GTK_SNIPPET_SOURCE_VIEW_LISTENER(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER, GtkSnippetSourceViewListener))
#define GTK_SNIPPETS_SOURCE_VIEW_LISTENER_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER, GtkSnippetSourceViewListenerClass))
#define GTK_IS_SNIPPETS_SOURCE_VIEW_LISTENER(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER))
#define GTK_IS_SNIPPETS_SOURCE_VIEW_LISTENER_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER))
#define GTK_SNIPPETS_SOURCE_VIEW_LISTENER_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER, GtkSnippetSourceViewListenerClass))

typedef struct _GtkSnippetSourceViewListener GtkSnippetSourceViewListener;
typedef struct _GtkSnippetSourceViewListenerPrivate GtkSnippetSourceViewListenerPrivate;
typedef struct _GtkSnippetSourceViewListenerClass GtkSnippetSourceViewListenerClass;

struct _GtkSnippetSourceViewListener {
	GObject parent;
	GtkSnippetSourceViewListenerPrivate *priv;
};

struct _GtkSnippetSourceViewListenerClass {
	GObjectClass parent_class;
	/* Add Signal Functions Here */
};

GType gtk_snippet_source_view_listener_get_type();

GtkSnippetSourceViewListener*
gtk_snippet_source_view_listener_new(GtkSnippetsLoader *loader, GtkSourceView *source_view);


G_END_DECLS

#endif /* GTK_SNIPPETS_SOURCE_VIEW_LISTENER_H */
