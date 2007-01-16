/***************************************************************************
 *            gtk-snippet-source-view-listener.c
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

#include <glade/glade.h>
#include <gdk/gdkkeysyms.h>
#include "gtk-snippet-source-view-listener.h"

#define GLADE_FILE "lib-gtk-source-view-snippets.glade"
 
static void gtk_snippet_source_view_listener_class_init(GtkSnippetSourceViewListenerClass *klass);
static void gtk_snippet_source_view_listener_init(GtkSnippetSourceViewListener *sp);
static void gtk_snippet_source_view_listener_finalize(GObject *object);

struct _GtkSnippetSourceViewListenerPrivate {
	/* Place Private Members Here */
	GHashTable *snippets;
	GtkWidget *popup;
};

static GObjectClass *parent_class = NULL;

GType
gtk_snippet_source_view_listener_get_type()
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo our_info = {
			sizeof (GtkSnippetSourceViewListenerClass),
			NULL,
			NULL,
			(GClassInitFunc)gtk_snippet_source_view_listener_class_init,
			NULL,
			NULL,
			sizeof (GtkSnippetSourceViewListener),
			0,
			(GInstanceInitFunc)gtk_snippet_source_view_listener_init,
		};

		type = g_type_register_static(G_TYPE_OBJECT, 
			"GtkSnippetSourceViewListener", &our_info, 0);
	}

	return type;
}

static void
gtk_snippet_source_view_listener_class_init(GtkSnippetSourceViewListenerClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gtk_snippet_source_view_listener_finalize;
	
	/* Create signals here:
	   gtk_snippet_source_view_listener_signals[SIGNAL_TYPE_EXAMPLE] = g_signal_new(...)
 	*/
}

static void
gtk_snippet_source_view_listener_init(GtkSnippetSourceViewListener *obj)
{
	obj->priv = g_new0(GtkSnippetSourceViewListenerPrivate, 1);
	/* Initialize private members, etc. */
	g_debug("Construido snippet source view listener");
}

static void
gtk_snippet_source_view_listener_finalize(GObject *object)
{
	GtkSnippetSourceViewListener *cobj;
	cobj = GTK_SNIPPET_SOURCE_VIEW_LISTENER(object);
	
	/* Free private members, etc. */
	
	g_free(cobj->priv);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	
	g_debug("Destruido snippet source view listener");
}

static void
gtk_snippets_source_view_listener_load_list_ui(GtkSnippetSourceViewListener *listener)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	glade_xml_signal_autoconnect (gxml);
	listener->priv->popup = glade_xml_get_widget (gxml, "snippets_popup_dialog");
}

static gboolean
gtk_snippet_source_view_listener_key_release_event(GtkWidget *widget,
	GdkEventKey *event,
   gpointer user_data)
{

	GtkSnippetSourceViewListener *listener = GTK_SNIPPET_SOURCE_VIEW_LISTENER(user_data);
	
	if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_space)
	{	
		g_print("hola");
		if (listener->priv->popup == NULL)
		{
			gtk_snippets_source_view_listener_load_list_ui(listener);
		}
	}
}

GtkSnippetSourceViewListener*
gtk_snippet_source_view_listener_new(GtkSnippetsLoader *loader, GtkSourceView *source_view)
{
	
	//Pedir al loader los snippets
	//Crear el model para mostrar
	//Crear el dialogo para mostrar los snippets.NO: Lo crearemos la primera vez que lo usen
	//conectar la señal para cazar el evento de teclado. NO: lo ponemos en una función a parte de momento
	
	g_assert(loader!=NULL);
	g_assert(source_view!=NULL);
	
	
	GtkSnippetSourceViewListener *obj;
	
	obj = GTK_SNIPPET_SOURCE_VIEW_LISTENER(g_object_new(GTK_TYPE_SNIPPET_SOURCE_VIEW_LISTENER, NULL));
	
	//Pedimos todos
	obj->priv->snippets = gtk_snippets_loader_get_snippets(loader);
	
	
	g_signal_connect(GTK_WIDGET(source_view), "key-release-event",
		G_CALLBACK(gtk_snippet_source_view_listener_key_release_event),(gpointer) obj);

	
	return obj;

	
}
