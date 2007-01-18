/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * gtksnippets
 * Copyright (C) chuchiperriman 2007 <chuchiperriman@gmail.com>
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

#include <gtk/gtk.h>
#include <glade/glade.h>
#include "gtk-snippets-popup-dialog.h"

#define GLADE_FILE GLADE_DIR"/libgtksnippets.glade"

struct _GtkSnippetsPopupDialogPrivate
{
	GtkWidget* window;
	GtkWidget* entry;
	GtkWidget* list;
	gint x;
	gint y;
};

#define GTK_SNIPPETS_POPUP_DIALOG_PRIVATE(o)  (G_TYPE_INSTANCE_GET_PRIVATE ((o), GTK_TYPE_SNIPPETS_POPUP_DIALOG, GtkSnippetsPopupDialogPrivate))

static GObjectClass* parent_class = NULL;

static void
gtk_snippets_popup_dialog_init (GtkSnippetsPopupDialog *popup_dialog)
{
	g_debug("Construido GtkSnippetsPopupDialog");
	popup_dialog->priv = g_new0(GtkSnippetsPopupDialogPrivate, 1);
	popup_dialog->priv->x = 0;
	popup_dialog->priv->y = 0;
}

static void
gtk_snippets_popup_dialog_finalize (GObject *object)
{
	/* TODO: Add deinitalization code here */
	GtkSnippetsPopupDialog *cobj;
	cobj = GTK_SNIPPETS_POPUP_DIALOG(object);

	//Si destruimos la ventana se destruyen los hijos
	gtk_widget_destroy(cobj->priv->window);
	
	g_free(cobj->priv);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
	g_debug("Destruido GtkSnippetsPopupDialog");
}

static void
gtk_snippets_popup_dialog_class_init (GtkSnippetsPopupDialogClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	g_type_class_add_private (klass, sizeof (GtkSnippetsPopupDialogPrivate));

	object_class->finalize = gtk_snippets_popup_dialog_finalize;
}

GType
gtk_snippets_popup_dialog_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsPopupDialogClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_snippets_popup_dialog_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsPopupDialog), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_snippets_popup_dialog_init, /* instance_init */
			NULL /* value_table */
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsPopupDialog",
		                                   &our_info, 0);
	}

	return our_type;
}


gboolean
on_snippets_popup_dialog_focus_out_event(GtkWidget *widget,
	GdkEventFocus *event,
	gpointer user_data)
{
	gtk_widget_hide(widget);
	return FALSE;
}

static void
gspd_load_glade(GtkSnippetsPopupDialog *obj)
{
	GladeXML *gxml = glade_xml_new (GLADE_FILE, NULL, NULL);
	
	glade_xml_signal_autoconnect (gxml);
	obj->priv->window = glade_xml_get_widget (gxml, "snippets_popup_dialog");
	obj->priv->entry = glade_xml_get_widget(gxml,"snippets_text_entry");
	obj->priv->list = glade_xml_get_widget(gxml,"snippets_list_view");
	
	g_object_unref(gxml);
}

GtkSnippetsPopupDialog*
gtk_snippets_popup_dialog_new (void)
{
	GtkSnippetsPopupDialog *obj;

	obj = GTK_SNIPPETS_POPUP_DIALOG(g_object_new(GTK_TYPE_SNIPPETS_POPUP_DIALOG, NULL));
	
	/* TODO: Add initialization code here */
	gspd_load_glade(obj);
	
	return obj;	
}

GtkWidget*
gtk_snippets_popup_dialog_get_window(GtkSnippetsPopupDialog* popup_dialog)
{
	return popup_dialog->priv->window;
}

void 
gtk_snippets_popup_dialog_set_pos(GtkSnippetsPopupDialog* popup_dialog, gint x, gint y)
{
	popup_dialog->priv->x = x;
	popup_dialog->priv->y = y;
}

void 
gtk_snippets_popup_dialog_set_pos_from_text_view(GtkSnippetsPopupDialog* popup_dialog, GtkTextView *text_view)
{
	GdkWindow *win;
	GtkTextMark* insert_mark;
	GtkTextBuffer* text_buffer;
	GtkTextIter start;
	GdkRectangle location;
	gint win_x, win_y;
	gint x, y;

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
	gdk_window_get_origin (win, &x, &y);
	
	gtk_snippets_popup_dialog_set_pos(popup_dialog,
									win_x + x,
									win_y + y + location.height);
	
	
}

void
gtk_snippets_popup_dialog_show(GtkSnippetsPopupDialog* popup_dialog,
								const gchar *word)
{

	gtk_window_move (GTK_WINDOW (popup_dialog->priv->window),popup_dialog->priv->x,popup_dialog->priv->y);
	gtk_entry_set_text (GTK_ENTRY(popup_dialog->priv->entry),word);
	gtk_widget_show (popup_dialog->priv->window);
	gtk_widget_grab_focus(popup_dialog->priv->entry);

}
void
gtk_snippets_popup_dialog_show_from_text_view(GtkSnippetsPopupDialog* popup_dialog, GtkTextView *text_view)
{
	GtkTextMark* insert_mark;
	GtkTextBuffer* text_buffer;
	GtkTextIter actual,ini_word;
	gchar* text;
	
	
	
	text_buffer = gtk_text_view_get_buffer(text_view);
	insert_mark = gtk_text_buffer_get_insert(text_buffer);
	gtk_text_buffer_get_iter_at_mark(text_buffer,&actual,insert_mark);
	
	ini_word = actual;
	
	//TODO Habría que comprobar si el caracter anterior es un blanco o una línea,
	//Entonces ponemos un blanco (la llamada a backward_word_start nos coge una palabra de la línea anterior)
	if (!gtk_text_iter_backward_word_start (&ini_word))
		text = "";
	else
	{
		text = gtk_text_iter_get_text (&ini_word, &actual);
		g_strstrip (text);
	}
	
	gtk_snippets_popup_dialog_set_pos_from_text_view(popup_dialog,text_view);
	gtk_snippets_popup_dialog_show(popup_dialog,text);
	//g_free(text);
}
