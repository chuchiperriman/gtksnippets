/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gtksnippets
 * Copyright (C) chuchi 2007 <chuchiperriman@gmail.com>
 * 
 * gtksnippets is free software.
 * 
 * You may redistribute it and/or modify it under the terms of the
 * GNU General Public License, as published by the Free Software
 * Foundation; either version 2 of the License, or (at your option)
 * any later version.
 * 
 * gtksnippets is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with gtksnippets.  If not, write to:
 * 	The Free Software Foundation, Inc.,
 * 	51 Franklin Street, Fifth Floor
 * 	Boston, MA  02110-1301, USA.
 */

#include <glade/glade.h>
#include <gdk/gdkkeysyms.h>
#include "gtk-text-completion-popup.h"

#define GLADE_FILE GLADE_DIR"/gtk-text-completion-popup.glade"

#define COL_NAME 0
#define COL_PROVIDER 1

struct _GtkTextCompletionPopupPrivate
{
	GtkTextView *text_view;
	GtkTreeView *data_tree_view;
	GtkWidget *window;
	GList *events;
	GList *providers;
};

#define GTK_TEXT_COMPLETION_POPUP_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTK_TYPE_TEXT_COMPLETION_POPUP, GtkTextCompletionPopupPrivate))

enum
{
	POPULATE_COMPLETION,
	LAST_SIGNAL
};

static GObjectClass* parent_class = NULL;
static guint text_completion_popup_signals[LAST_SIGNAL] = { 0 };

/***************USER_REQUEST_EVENT********************/

static gboolean
view_key_press_event_cb(GtkWidget *view,GdkEventKey *event, gpointer user_data)
{
	GtkTextCompletionPopup *popup;
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkTreePath *path;
	GtkTreeViewColumn *column;
	
	popup = GTK_TEXT_COMPLETION_POPUP(user_data);
	
	if ( GTK_WIDGET_VISIBLE(popup->priv->window) )
	{
		//TODO Si presiona parriba o pabajo foco al popup
		if (event->keyval == GDK_Escape || event->keyval == GDK_space)
		{
			gtk_widget_hide(popup->priv->window);
		}
		else if (event->keyval == GDK_Up)
		{
			model = gtk_tree_view_get_model(popup->priv->data_tree_view);
			gtk_tree_view_get_cursor(popup->priv->data_tree_view,&path,NULL);
			if (path == NULL)
			{
				if (gtk_tree_model_get_iter_first(model,&iter))
				{
					path = gtk_tree_model_get_path(model,&iter);
					gtk_tree_view_set_cursor(popup->priv->data_tree_view,path,NULL,FALSE);
					gtk_tree_path_free(path);
				}
			}
			else
			{
				gtk_tree_path_prev(path);
				gtk_tree_view_set_cursor(popup->priv->data_tree_view,path,NULL,FALSE);
				gtk_tree_path_free(path);
			}
			gtk_widget_grab_focus(GTK_WIDGET(popup->priv->data_tree_view));
			return TRUE;
		}
		else if (event->keyval == GDK_Down)
		{
			model = gtk_tree_view_get_model(popup->priv->data_tree_view);
			gtk_tree_view_get_cursor(popup->priv->data_tree_view,&path,NULL);
			if (path == NULL)
			{
				if (gtk_tree_model_get_iter_first(model,&iter))
				{
					path = gtk_tree_model_get_path(model,&iter);
					gtk_tree_view_set_cursor(popup->priv->data_tree_view,path,NULL,FALSE);
					gtk_tree_path_free(path);
				}
			}
			else
			{
				gtk_tree_path_next(path);
				gtk_tree_view_set_cursor(popup->priv->data_tree_view,path,NULL,FALSE);
				gtk_tree_path_free(path);
			}
			gtk_widget_grab_focus(GTK_WIDGET(popup->priv->data_tree_view));
			return TRUE;
		}
		else if (event->keyval == GDK_Return)
		{
			gtk_tree_view_get_cursor(popup->priv->data_tree_view, &path, &column);
			if (path != NULL)
			{
				gtk_tree_view_row_activated(popup->priv->data_tree_view,path,column);
				return TRUE;
			}
			else
			{
				gtk_widget_hide(popup->priv->window);
			}
		}
	}
	else
	{
		if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_Return)
		{
			g_debug("lanzamos");
			gtk_text_completion_popup_raise_event(popup,USER_REQUEST_EVENT);
			return TRUE;
		}
	}

	return FALSE;
}

static void
user_request_event_activate(GtkTextCompletionPopup *popup)
{
	//Catch control+return event
	GtkTextView *text_view = gtk_text_completion_popup_get_view(popup);
	if (text_view==NULL)
		g_debug("pues vaa ser nulo");
		
	g_debug("user_request_event activate");
	g_signal_connect(text_view, "key-press-event",
		G_CALLBACK(view_key_press_event_cb),(gpointer) popup);
	g_debug("FIN user_request_event activate");
	
}
/*****************************************************/
static void
gtcp_popup_row_activated_cb (GtkTreeView *tree_view,
										GtkTreePath *path,
										GtkTreeViewColumn *column,
										gpointer user_data)
{
	//gtcp_data_selected(popup);
	GtkTreeIter iter;
	GtkTreeModel *model;
	GtkTextCompletionProvider *provider;
	const gchar* data;
	GtkTextCompletionPopup *popup;
	GValue value_prov = {0,};
	GValue value_name = {0,};
	
	popup = GTK_TEXT_COMPLETION_POPUP(user_data);
	model = gtk_tree_view_get_model(tree_view);
	
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get_value(model,&iter,COL_NAME,&value_name);
	data = g_value_get_string(&value_name);
	gtk_tree_model_get_value(model,&iter,COL_PROVIDER,&value_prov);
	provider = GTK_TEXT_COMPLETION_PROVIDER(g_value_get_pointer(&value_prov));
	gtk_text_completion_provider_data_selected(provider,popup->priv->text_view,(gpointer)data);
	gtk_widget_hide(popup->priv->window);
}

static void
gtcp_gtv_get_screen_pos(GtkTextView *text_view, gint *x, gint *y)
{
	GdkWindow *win;
	GtkTextMark* insert_mark;
	GtkTextBuffer* text_buffer;
	GtkTextIter start;
	GdkRectangle location;
	gint win_x, win_y;
	gint xx, yy;

	text_buffer = gtk_text_view_get_buffer(text_view);
	insert_mark = gtk_text_buffer_get_insert(text_buffer);
	gtk_text_buffer_get_iter_at_mark(text_buffer,&start,insert_mark);
	gtk_text_view_get_iter_location(text_view,
														&start,
														&location );
	gtk_text_view_buffer_to_window_coords (text_view,
                                        GTK_TEXT_WINDOW_WIDGET,
                                        location.x, location.y,
                                        &win_x, &win_y);

	win = gtk_text_view_get_window (text_view, 
                                GTK_TEXT_WINDOW_WIDGET);
	gdk_window_get_origin (win, &xx, &yy);
	
	*x = win_x + xx;
	*y = win_y + yy + location.height;
	
}
static gboolean
gtcp_event_exists(GtkTextCompletionPopup *popup, const gchar *event_name)
{
	//Comprueba si existe el evento
	if (g_list_find_custom(popup->priv->events, event_name, (GCompareFunc)g_ascii_strcasecmp) != NULL)
		return TRUE;
	else
		return FALSE;
}

static void
gtcp_create_tree_model(GtkTextCompletionPopup *popup)
{
	GtkListStore *list_store;
	list_store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
		
	g_debug("Asignamos el modelo");
	gtk_tree_view_set_model(popup->priv->data_tree_view,GTK_TREE_MODEL(list_store));
}

static void
gtcp_add_data_to_tree(GtkTextCompletionPopup *popup, const gchar* name, GtkTextCompletionProvider *provider)
{
	GtkTreeIter iter;
	GtkListStore *store;
	
	store = GTK_LIST_STORE(gtk_tree_view_get_model(popup->priv->data_tree_view));
	
	gtk_list_store_append (store,&iter);
			
	gtk_list_store_set (store, 
						&iter,
						COL_NAME, name,
						COL_PROVIDER, (gpointer)provider,
						-1);
}

/**
* Crea el arbol pero no hace nada con el modelo
*/
static void
gtcp_load_tree(GtkTextCompletionPopup *popup)
{
	g_assert(popup!=NULL);
	
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	/* crea una columna */
	column = gtk_tree_view_column_new();
	/* coloca el nombre a la columna */
	//gtk_tree_view_column_set_title(column, "Snippet");
	/* crea un render tipo texto */
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"text",COL_NAME,NULL);

	/* agrega la columna al arbol */
	gtk_tree_view_append_column (GTK_TREE_VIEW(popup->priv->data_tree_view), column);
}

static void
gtcp_load_glade(GtkTextCompletionPopup *popup)
{
	g_debug("load glade");
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);

	g_assert(gxml != NULL);

	//glade_xml_signal_autoconnect (gxml);
	g_debug("despues conect load glade");
	popup->priv->window = glade_xml_get_widget (gxml, "window");
	popup->priv->data_tree_view = GTK_TREE_VIEW(glade_xml_get_widget (gxml, "data_tree_view"));
	g_debug("cogemos el window");
	
	/*g_signal_connect(GTK_WIDGET(obj->priv->tree_view), "row-activated",
		G_CALLBACK(snippets_tree_view_row_activated_cb),(gpointer) obj);*/
	
	//Load the GtkSourceView
	/*obj->priv->source_scroll = glade_xml_get_widget(gxml,"snippets_content_scroll");
	obj->priv->source = gtk_source_view_new();
	gtk_text_view_set_editable(GTK_TEXT_VIEW(obj->priv->source),FALSE);
	gtk_source_buffer_set_highlight(
		GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(obj->priv->source))),
		TRUE);
	
	gtk_container_add(GTK_CONTAINER(obj->priv->source_scroll), obj->priv->source);
	*/
	
	g_object_unref(gxml);
	g_debug("sacabo load glade");
}

static void
gtk_text_completion_popup_init (GtkTextCompletionPopup *popup)
{
	popup->priv = GTK_TEXT_COMPLETION_POPUP_GET_PRIVATE(popup);
	popup->priv->events = NULL;
	popup->priv->providers = NULL;
	gtcp_load_glade(popup);
	gtcp_load_tree(popup);
	gtcp_create_tree_model(popup);
	
	g_signal_connect(popup->priv->data_tree_view, "row-activated",
		G_CALLBACK(gtcp_popup_row_activated_cb),(gpointer) popup);
}

static void
gtk_text_completion_popup_finalize (GObject *object)
{
	GtkTextCompletionPopup *popup = GTK_TEXT_COMPLETION_POPUP(object);
	GList *actual;
	/* TODO: Add deinitalization code here */
	actual = popup->priv->events;
	if (actual != NULL)
	{
		do
		{
			g_free(actual->data);
			
		}while((actual = g_list_next(actual)) != NULL);
	}
	
	g_list_free(popup->priv->events);
	
	g_list_free(popup->priv->providers);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtk_text_completion_popup_populate_completion (GtkTextCompletionPopup* popup)
{
	/* TODO: Add default signal handler implementation here */
}

static void
gtk_text_completion_popup_class_init (GtkTextCompletionPopupClass *klass)
{
	g_debug("class init");
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtk_text_completion_popup_finalize;

	klass->populate_completion = gtk_text_completion_popup_populate_completion;

	text_completion_popup_signals[POPULATE_COMPLETION] =
		g_signal_new ("populate-completion",
		              G_OBJECT_CLASS_TYPE (klass),
		              0, 
		              G_STRUCT_OFFSET (GtkTextCompletionPopupClass, populate_completion),
		              NULL, NULL,NULL,
		              G_TYPE_NONE, 0,
		              NULL
		              );
	g_debug("casi fin class init");
	g_type_class_add_private (object_class, sizeof(GtkTextCompletionPopupPrivate));
	g_debug("fin class init");
}

GType
gtk_text_completion_popup_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkTextCompletionPopupClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_text_completion_popup_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkTextCompletionPopup), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_text_completion_popup_init, /* instance_init */
			NULL /* value_table */  
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkTextCompletionPopup",
		                                   &our_info, 0);
	}

	return our_type;
}

GtkTextCompletionPopup*
gtk_text_completion_popup_new (GtkTextView *view)
{
	GtkTextCompletionPopup *popup = GTK_TEXT_COMPLETION_POPUP (g_object_new (GTK_TYPE_TEXT_COMPLETION_POPUP, NULL));
	popup->priv->text_view = view;
	
	//Default events
	gtk_text_completion_popup_add_event(popup,USER_REQUEST_EVENT);
	user_request_event_activate(popup);
	
	return popup;
}

void
gtk_text_completion_popup_add_event(GtkTextCompletionPopup *popup, const gchar *event_name)
{
	if (!gtcp_event_exists(popup,event_name))
		popup->priv->events = g_list_append (popup->priv->events, g_strdup(event_name));
}

void
gtk_text_completion_popup_register_provider(GtkTextCompletionPopup *popup, GtkTextCompletionProvider *provider)
{
	g_object_ref(provider);
	popup->priv->providers = g_list_append(popup->priv->providers, provider);
}

GtkTextView*
gtk_text_completion_popup_get_view(GtkTextCompletionPopup *popup)
{
	return popup->priv->text_view;
}

void
gtk_text_completion_popup_raise_event(GtkTextCompletionPopup *popup, const gchar *event_name)
{
	GList* data_list;
	GList *providers_list;
	GtkTextCompletionProvider *provider;
	GtkListStore *store;
	gint x, y;
	
	//Raise populate
	/*g_signal_emit(
				popup,
				text_completion_popup_signals[POPULATE_COMPLETION],
				0);*/
	store = GTK_LIST_STORE(gtk_tree_view_get_model(popup->priv->data_tree_view));
	gtk_list_store_clear(store);
	if (popup->priv->providers != NULL)
	{
		providers_list = popup->priv->providers;
		do
		{
			provider =  GTK_TEXT_COMPLETION_PROVIDER(providers_list->data);
			data_list = gtk_text_completion_provider_get_data (provider, popup->priv->text_view, (gpointer)event_name);
			if (data_list != NULL)
			{
				do
				{
					gtcp_add_data_to_tree(popup, (gchar*)data_list->data, provider);
					
				}while((data_list = g_list_next(data_list)) != NULL);
			}
		}while((providers_list = g_list_next(popup->priv->providers)) != NULL);
		
				
		//Pedimos datos a los proveedores
	
		//If there are not items, we don't show the popup
		//Show popup
		gtcp_gtv_get_screen_pos(popup->priv->text_view,&x,&y);
		gtk_window_move(GTK_WINDOW(popup->priv->window), x, y);
		//TODO Poner el foco en el primer elemento
		gtk_widget_show(popup->priv->window);
	}
	
}
