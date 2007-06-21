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
#include <string.h>
#include "gtk-text-completion-popup.h"
#include "gtk-text-completion-data.h"
#include "gtk-snippets-gsv-utils.h"

#define GLADE_FILE GLADE_DIR"/gtk-text-completion-popup.glade"

#define COL_PIXBUF 0
#define COL_NAME 1
#define COL_PROVIDER 2
#define COL_DATA 3

struct _GtkTextCompletionPopupPrivate
{
	GtkTextView *text_view;
	GtkTreeView *data_tree_view;
	GtkWidget *window;
	GList *events;
	GList *providers;
	gulong internal_signal_ids[2];
	//user-request event
	gboolean ur_active;
	//word completion event
	gboolean wc_active;
	//Min. characters to show autocompletion
	guint min_char_word;
	
	//TODO borrar
	guint id;
};

#define GTK_TEXT_COMPLETION_POPUP_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTK_TYPE_TEXT_COMPLETION_POPUP, GtkTextCompletionPopupPrivate))

enum
{
	POPULATE_COMPLETION,
	LAST_SIGNAL
};

//Internal signals
enum
{
	IS_POPUP_ROW_ACTIVATE,
	IS_GTK_TEST_VIEW_KP,
	IS_GTK_TEST_VIEW_KR,
	IS_LAST_SIGNAL
};

static GObjectClass* parent_class = NULL;
static guint text_completion_popup_signals[LAST_SIGNAL] = { 0 };

//TODO pruebas
static guint contador=0;

static gboolean
gtcp_is_active(GtkTextCompletionPopup *popup)
{
	return GTK_WIDGET_VISIBLE(popup->priv->window);
}

//////////////////WORD COMPLETION FUNCTIONS///////////////////////
static gboolean
gtcp_wc_process_key_press(GtkTextCompletionPopup *popup, GdkEventKey* event)
{
	gchar* word;
	gboolean res = FALSE;
	guint keyval = event->keyval;
	//If not is a character key do nothing
	if  ((!(event->state & GDK_CONTROL_MASK))
		&& ( (GDK_A <= keyval && keyval <= GDK_Z)
		|| (GDK_a <= keyval && keyval <= GDK_z)
		|| (GDK_0 <= keyval && keyval <= GDK_9)		
		|| GDK_underscore == keyval))
	{
		word = gtk_snippets_gsv_get_last_word_and_iter(popup->priv->text_view, NULL, NULL);
		if (strlen(word)>=3)
		{
			gtk_text_completion_popup_raise_event(popup,WORD_COMPLETION_EVENT,event);
		}
		g_free(word);
	}
	else if (GDK_BackSpace == keyval)
	{
		word = gtk_snippets_gsv_get_last_word_and_iter(popup->priv->text_view, NULL, NULL);
		if (gtcp_is_active(popup))
		{
			//Deleted character is counted here
			if((strlen(word)>4))
			{
				gtk_text_completion_popup_raise_event(popup,WORD_COMPLETION_EVENT,event);
			}
			else
			{
				gtk_widget_hide(popup->priv->window);
			}
		}
		g_free(word);
	}
	
	return res;
}

//////////////////MOVING IN THE TREE//////////////////////////////
static gboolean
gtcp_tree_first(GtkTextCompletionPopup *popup)
{
	GtkTreeIter iter;
	GtkTreePath* path;
	GtkTreeModel* model;
	GtkTreeSelection* selection;
	if (!gtcp_is_active(popup))
		return FALSE;
	
	selection = gtk_tree_view_get_selection(popup->priv->data_tree_view);

	if (gtk_tree_selection_get_mode(selection) == GTK_SELECTION_NONE)
		return FALSE;

	model = gtk_tree_view_get_model(popup->priv->data_tree_view);
		
	gtk_tree_model_get_iter_first(model, &iter);
	gtk_tree_selection_select_iter(selection, &iter);
	path = gtk_tree_model_get_path(model, &iter);
	gtk_tree_view_scroll_to_cell(popup->priv->data_tree_view, path, NULL, FALSE, 0, 0);
	gtk_tree_path_free(path);
	return TRUE;
}

static gboolean 
gtcp_tree_last(GtkTextCompletionPopup *popup)
{
	GtkTreeIter iter;
	GtkTreeModel* model;
	GtkTreeSelection* selection;
	GtkTreePath* path;
	gint children;
	
	if (!gtcp_is_active(popup))
		return FALSE;
	
	selection = gtk_tree_view_get_selection(popup->priv->data_tree_view);
	model = gtk_tree_view_get_model(popup->priv->data_tree_view);
	
	if (gtk_tree_selection_get_mode(selection) == GTK_SELECTION_NONE)
		return FALSE;
	
	children = gtk_tree_model_iter_n_children(model, NULL);
	if (children > 0)
	{
		gtk_tree_model_iter_nth_child(model, &iter, NULL, children - 1);
	
		gtk_tree_selection_select_iter(selection, &iter);
		path = gtk_tree_model_get_path(model, &iter);
		gtk_tree_view_scroll_to_cell(popup->priv->data_tree_view, path, NULL, FALSE, 0, 0);
		gtk_tree_path_free(path);
		return TRUE;
	}
	return FALSE;
}

static gboolean
gtcp_tree_up(GtkTextCompletionPopup *popup, gint rows)
{
	GtkTreeIter iter;
	GtkTreePath* path;
	GtkTreeModel* model;
	GtkTreeSelection* selection;
	
	if (!gtcp_is_active(popup))
		return FALSE;
	
	selection = gtk_tree_view_get_selection(popup->priv->data_tree_view);
	
	if (gtk_tree_selection_get_mode(selection) == GTK_SELECTION_NONE)
		return FALSE;
	
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gint i;
		path = gtk_tree_model_get_path(model, &iter);
		for (i=0; i  < rows; i++)
			gtk_tree_path_prev(path);
		
		if (gtk_tree_model_get_iter(model, &iter, path))
		{
			gtk_tree_selection_select_iter(selection, &iter);
			gtk_tree_view_scroll_to_cell(popup->priv->data_tree_view, path, NULL, FALSE, 0, 0);
		}
		gtk_tree_path_free(path);
	}
	else
	{
		return gtcp_tree_first(popup);
	}
	
	return TRUE;
}

static gboolean
gtcp_tree_down(GtkTextCompletionPopup *popup, gint rows)
{
	GtkTreeIter iter;
	GtkTreeModel* model;
	GtkTreeSelection* selection;
	
	if (!gtcp_is_active(popup))
		return FALSE;
	
	selection = gtk_tree_view_get_selection(popup->priv->data_tree_view);
	if (gtk_tree_selection_get_mode(selection) == GTK_SELECTION_NONE)
		return FALSE;
	
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gint i;
		GtkTreePath* path;
		for (i = 0; i < rows; i++)
		{
			if (!gtk_tree_model_iter_next(model, &iter))
				return gtcp_tree_last(popup);
		}
		gtk_tree_selection_select_iter(selection, &iter);
		path = gtk_tree_model_get_path(model, &iter);
		gtk_tree_view_scroll_to_cell(popup->priv->data_tree_view, path, NULL, FALSE, 0, 0);
		gtk_tree_path_free(path);
	}
	else
	{
		return gtcp_tree_first(popup);
	}
	return TRUE;
}

static gboolean
gtcp_tree_selection(GtkTextCompletionPopup *popup)
{
	GtkTreeIter iter;
	GtkTreeModel* model;
	GtkTreeSelection* selection;
	GtkTreePath* path;
	
	if (!gtcp_is_active(popup))
		return FALSE;
	
	selection = gtk_tree_view_get_selection(popup->priv->data_tree_view);
	
	if (gtk_tree_selection_get_mode(selection) == GTK_SELECTION_NONE)
		return FALSE;	
		
	if (gtk_tree_selection_count_selected_rows(selection)==0)
	{
		gtk_widget_hide(popup->priv->window);	
		return FALSE;
	}

	
	if (gtk_tree_selection_get_selected(selection, &model, &iter))
	{
		gtk_tree_selection_select_iter(selection, &iter);
		path = gtk_tree_model_get_path(model, &iter);
		if (path!=NULL)
		{
			gtk_tree_view_row_activated(popup->priv->data_tree_view,path,gtk_tree_view_get_column(popup->priv->data_tree_view,COL_NAME));
		}
		gtk_tree_path_free(path);
		return TRUE;
	}
	else
	{	
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////
static gboolean
view_key_release_event_cb(GtkWidget *view,GdkEventKey *event, gpointer user_data)
{
	//Cazamos las teclas que no son de movimiento en el popup
	GtkTextCompletionPopup *popup;
	g_assert(GTK_IS_TEXT_COMPLETION_POPUP(user_data));
	popup = GTK_TEXT_COMPLETION_POPUP(user_data);
	
	if (popup->priv->wc_active)
	{
		return gtcp_wc_process_key_press(popup,event);
	}
	return FALSE;
}

static gboolean
view_key_press_event_cb(GtkWidget *view,GdkEventKey *event, gpointer user_data)
{
	//Cazamos solo las teclas especiales de movimiento en el popup
	GtkTextCompletionPopup *popup;
	g_assert(GTK_IS_TEXT_COMPLETION_POPUP(user_data));
	popup = GTK_TEXT_COMPLETION_POPUP(user_data);
	
	if (gtcp_is_active(popup))
	{
		switch (event->keyval)
	 	{
			case GDK_Escape:
			case GDK_space:
			{
				gtk_widget_hide(popup->priv->window);
				return FALSE;
			}
	 		case GDK_Down:
			{
				return gtcp_tree_down(popup, 1);
			}
			case GDK_Page_Down:
			{
				//TODO page_down 5, or 6 or 7 ...
				return gtcp_tree_down(popup, 5);
			}
			case GDK_Up:
			{
				if (gtcp_tree_up(popup, 1))
				{
					return TRUE;
				}
				else
				{
					return gtcp_tree_first(popup);
				}
			}
			case GDK_Page_Up:
			{
				return gtcp_tree_up(popup, 5);
			}
			case GDK_Home:
			{
				return gtcp_tree_first(popup);
			}
			case GDK_End:
			{
				return gtcp_tree_last(popup);
			}
			case GDK_Return:
			case GDK_Tab:
			{
				return gtcp_tree_selection(popup);
			}
		}
	}
	else
	{
		if (popup->priv->ur_active)
		{
			if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_Return)
			{
				gtk_text_completion_popup_raise_event(popup,USER_REQUEST_EVENT,NULL);
				return TRUE;
			}
		}
	}
	return FALSE;

}

static void
user_request_event_activate(GtkTextCompletionPopup *popup)
{
	if (popup->priv->ur_active == FALSE)
	{
		//Catch control+return event
		popup->priv->ur_active = TRUE;
		//Default events
		gtk_text_completion_popup_add_event(popup,USER_REQUEST_EVENT);
	}
}

static void
word_completion_event_activate(GtkTextCompletionPopup *popup)
{
	if (popup->priv->wc_active == FALSE)
	{
		//Catch control+return event
		popup->priv->wc_active = TRUE;
		gtk_text_completion_popup_add_event(popup,WORD_COMPLETION_EVENT);
	}
}

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
	GtkTextCompletionData *data;
	GtkTextCompletionPopup *popup;
	GValue value_prov = {0,};
	GValue value_name = {0,};
	
	popup = GTK_TEXT_COMPLETION_POPUP(user_data);
	model = gtk_tree_view_get_model(tree_view);
	
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get_value(model,&iter,COL_DATA,&value_name);
	data = GTK_TEXT_COMPLETION_DATA(g_value_get_pointer(&value_name));
	gtk_tree_model_get_value(model,&iter,COL_PROVIDER,&value_prov);
	provider = GTK_TEXT_COMPLETION_PROVIDER(g_value_get_pointer(&value_prov));
	gtk_text_completion_provider_data_selected(provider,popup->priv->text_view, data);
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
gtc_clean_data(GtkListStore *store)
{
	GtkTreeModel *model = GTK_TREE_MODEL(store);
	GtkTreeIter iter;
	GtkTextCompletionData *data;
	GtkTextCompletionProvider *prov;
	
	
	if (gtk_tree_model_get_iter_first(model,&iter))
	{
		do
		{
			GValue value_data = {0,};
			GValue value_prov = {0,};
			gtk_tree_model_get_value(model,&iter,COL_DATA,&value_data);
			data = GTK_TEXT_COMPLETION_DATA(g_value_get_pointer(&value_data));
			gtk_tree_model_get_value(model,&iter,COL_PROVIDER,&value_prov);
			prov = GTK_TEXT_COMPLETION_PROVIDER(g_value_get_pointer(&value_prov));
			gtk_text_completion_data_free(data);
		}while(gtk_tree_model_iter_next(model,&iter));
	}
	
	gtk_list_store_clear(store);
}

static void
gtcp_create_tree_model(GtkTextCompletionPopup *popup)
{
	GtkListStore *list_store;
	list_store = gtk_list_store_new (4,GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_POINTER, G_TYPE_POINTER);
		
	gtk_tree_view_set_model(popup->priv->data_tree_view,GTK_TREE_MODEL(list_store));
}

static void
gtcp_add_data_to_tree(GtkTextCompletionPopup *popup, GtkTextCompletionData* data, GtkTextCompletionProvider *provider)
{

	g_assert(data != NULL);
	g_assert(provider != NULL);
	
	GtkTreeIter iter;
	GtkListStore *store;
	
	store = GTK_LIST_STORE(gtk_tree_view_get_model(popup->priv->data_tree_view));
	
	gtk_list_store_append (store,&iter);
			
	gtk_list_store_set (store, 
						&iter,
						COL_PIXBUF, gtk_text_completion_data_get_icon(data),
						COL_NAME, gtk_text_completion_data_get_name(data),
						COL_PROVIDER, (gpointer)provider,
						COL_DATA, data,
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
	GtkCellRenderer* renderer_pixbuf;
	GtkTreeViewColumn* column_pixbuf;
	
	//Icon column
	renderer_pixbuf = gtk_cell_renderer_pixbuf_new();
   column_pixbuf = gtk_tree_view_column_new_with_attributes ("Pixbuf",
   		renderer_pixbuf, "pixbuf", COL_PIXBUF, NULL);
	
	//Name column
	column = gtk_tree_view_column_new();
	renderer = gtk_cell_renderer_text_new ();
	gtk_tree_view_column_pack_start (column, renderer, FALSE);
	gtk_tree_view_column_set_attributes (column,renderer,"text",COL_NAME,NULL);

	gtk_tree_view_append_column (popup->priv->data_tree_view, column_pixbuf);
	gtk_tree_view_append_column (popup->priv->data_tree_view, column);
}

static void
gtcp_load_glade(GtkTextCompletionPopup *popup)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);

	g_assert(gxml != NULL);

	//glade_xml_signal_autoconnect (gxml);
	popup->priv->window = glade_xml_get_widget (gxml, "window");
	popup->priv->data_tree_view = GTK_TREE_VIEW(glade_xml_get_widget (gxml, "data_tree_view"));
	
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
}

static void
gtk_text_completion_popup_init (GtkTextCompletionPopup *popup)
{
	gint i;
	popup->priv = GTK_TEXT_COMPLETION_POPUP_GET_PRIVATE(popup);
	popup->priv->events = NULL;
	popup->priv->providers = NULL;
	popup->priv->ur_active = FALSE;
	popup->priv->wc_active = FALSE;
	popup->priv->min_char_word = 3;
	gtcp_load_glade(popup);
	gtcp_load_tree(popup);
	gtcp_create_tree_model(popup);
	
	for (i=0;i<IS_LAST_SIGNAL;i++)
	{
		popup->priv->internal_signal_ids[i] = 0;
	}
	
	popup->priv->internal_signal_ids[IS_POPUP_ROW_ACTIVATE] = g_signal_connect(popup->priv->data_tree_view, "row-activated",
			G_CALLBACK(gtcp_popup_row_activated_cb),(gpointer) popup);

	popup->priv->id=contador;
	contador++;
}

static void
gtk_text_completion_popup_finalize (GObject *object)
{
	
	GtkTextCompletionPopup *popup = GTK_TEXT_COMPLETION_POPUP(object);
	GList *actual;
	GtkListStore *store;
	gint i;
	
	actual = popup->priv->events;
	if (actual != NULL)
	{
		do
		{
			g_free(actual->data);
			
		}while((actual = g_list_next(actual)) != NULL);
	}
	
	store = GTK_LIST_STORE(gtk_tree_view_get_model(popup->priv->data_tree_view));
	gtc_clean_data(store);
	
	g_list_free(popup->priv->events);
	
	actual = popup->priv->providers;
	if (actual != NULL)
	{
		do
		{
			g_object_unref(actual->data);
			
		}while((actual = g_list_next(actual)) != NULL);
	}
	
	g_list_free(popup->priv->providers);

	g_signal_handler_disconnect (popup->priv->data_tree_view,
				popup->priv->internal_signal_ids[IS_POPUP_ROW_ACTIVATE]);
	for (i=IS_POPUP_ROW_ACTIVATE;i<IS_LAST_SIGNAL;i++)
	{
		if (g_signal_handler_is_connected(popup->priv->text_view, popup->priv->internal_signal_ids[i]))
		{
			g_signal_handler_disconnect (popup->priv->text_view,
				popup->priv->internal_signal_ids[i]);
		}
		popup->priv->internal_signal_ids[i] = 0;
	}

	//g_assert(popup->priv->window!=NULL);
	//g_object_unref(popup->priv->window);
	//G_OBJECT_CLASS (parent_class)->finalize (object);
	
}

static void
gtk_text_completion_popup_populate_completion (GtkTextCompletionPopup* popup)
{
	/* TODO: Add default signal handler implementation here */
	
}

static void
gtk_text_completion_popup_class_init (GtkTextCompletionPopupClass *klass)
{
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

	g_type_class_add_private (object_class, sizeof(GtkTextCompletionPopupPrivate));
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
	
	popup->priv->internal_signal_ids[IS_GTK_TEST_VIEW_KP] = g_signal_connect(view, "key-press-event",
			G_CALLBACK(view_key_press_event_cb),(gpointer) popup);
	
	//TODO conectar after esta y utilizarla para las teclas normales. En el key press cazamos las
	//teclas de las flechas etc.
	popup->priv->internal_signal_ids[IS_GTK_TEST_VIEW_KR] = g_signal_connect_after(view, "key-release-event",
			G_CALLBACK(view_key_release_event_cb),(gpointer) popup);
			
	user_request_event_activate(popup);
	
	word_completion_event_activate(popup);
	
	
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
gtk_text_completion_popup_raise_event(GtkTextCompletionPopup *popup, const gchar *event_name, gpointer event_data)
{
	GList* data_list;
	GList *providers_list;
	GtkTextCompletionProvider *provider;
	GtkListStore *store;
	GtkTreeIter iter;
	gint x, y;
	
	//Raise populate
	/*g_signal_emit(
				popup,
				text_completion_popup_signals[POPULATE_COMPLETION],
				0);*/
	store = GTK_LIST_STORE(gtk_tree_view_get_model(popup->priv->data_tree_view));
	gtc_clean_data(store);
	
	if (popup->priv->providers != NULL)
	{
		providers_list = popup->priv->providers;
		do
		{
			provider =  GTK_TEXT_COMPLETION_PROVIDER(providers_list->data);
			data_list = gtk_text_completion_provider_get_data (
							provider, popup->priv->text_view, event_name, event_data);
			if (data_list != NULL)
			{
				do
				{
					gtcp_add_data_to_tree(popup, GTK_TEXT_COMPLETION_DATA(data_list->data), provider);
					//gtk_text_completion_data_free(GTK_TEXT_COMPLETION_DATA(data_list->data));
					
				}while((data_list = g_list_next(data_list)) != NULL);
				g_list_free(data_list);
			}
			
		}while((providers_list = g_list_next(providers_list)) != NULL);
		
		//If there are not items, we don't show the popup
		if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(store),&iter))
		{
			//Show popup
			gtcp_gtv_get_screen_pos(popup->priv->text_view,&x,&y);
			gtk_window_move(GTK_WINDOW(popup->priv->window), x, y);
			gtk_widget_show(popup->priv->window);
			gtk_tree_view_scroll_to_point(popup->priv->data_tree_view,0,0);
			//gtcp_tree_first(popup);
		}
		else
		{
			if (gtcp_is_active(popup))
			{
				gtk_widget_hide(popup->priv->window);
			}
		}
	}
	
}
