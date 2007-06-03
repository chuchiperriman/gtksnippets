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

#include "gtk-snippets-editor-gsv.h"
#include "gtk-snippets-editor.h"
#include "gtk-snippets-gsv-utils.h"

static GObjectClass* parent_class = NULL;

static gchar*
gtk_snippets_editor_gsv_get_current_word(GtkSnippetsEditor* editor)
{
	return gtk_snippets_gsv_get_last_word(
										  GTK_TEXT_VIEW(GTK_SNIPPETS_EDITOR_GSV(editor)->source_view));	
	/*
	gchar*
gtk_snippets_gsv_get_last_word_and_iter(GtkTextView *text_view, GtkTextIter *start_word, GtkTextIter *end_word);

gchar*
gtk_snippets_gsv_get_last_word(GtkTextView *text_view);

void
gtk_snippets_gsv_get_screen_pos(GtkTextView *text_view, gint *x, gint *y);
*/
}

static void
gtk_snippets_editor_gsv_get_cursor_position(GtkSnippetsEditor* editor,int* x, int* y)
{
	gtk_snippets_gsv_get_screen_pos(
									GTK_TEXT_VIEW(GTK_SNIPPETS_EDITOR_GSV(editor)->source_view), 
									x,
									y);	
}

static void
gtk_snippets_editor_gsv_insert_text_in_cursor(GtkSnippetsEditor* editor, const gchar* text)
{
	GtkTextView *text_view;
	GtkTextBuffer *buffer;
	GtkTextIter word_start, word_end;
	
	text_view = GTK_TEXT_VIEW(GTK_SNIPPETS_EDITOR_GSV(editor)->source_view);
	//Hay que borrar la palabra escrita para el snippet
	buffer = gtk_text_view_get_buffer(text_view);

	gtk_snippets_gsv_get_last_word_and_iter(text_view,&word_start, &word_end);
									   
	gtk_text_buffer_delete(buffer,&word_start,&word_end);
	gtk_text_buffer_insert(buffer, &word_start, text,-1);
}

static GObject*
gtk_snippets_editor_gsv_get_widget(GtkSnippetsEditor* editor)
{
	return G_OBJECT(GTK_SNIPPETS_EDITOR_GSV(editor)->source_view);
}

static void
gtk_snippets_editor_gsv_interface_init (gpointer   g_iface,
																gpointer   iface_data)
{
	GtkSnippetsEditorClass *klass = (GtkSnippetsEditorClass *)g_iface;
	klass->get_current_word = ( gchar* (*)  (GtkSnippetsEditor* editor) ) gtk_snippets_editor_gsv_get_current_word;
	klass->get_cursor_position = ( void (*)  (GtkSnippetsEditor* editor,int* x, int* y) ) gtk_snippets_editor_gsv_get_cursor_position;
	klass->insert_text_in_cursor = ( void (*)  (GtkSnippetsEditor* editor, const gchar* text) ) gtk_snippets_editor_gsv_insert_text_in_cursor;
	klass->get_widget =  ( GObject* (*)  (GtkSnippetsEditor* editor) ) gtk_snippets_editor_gsv_get_widget;
	/*void (*get_cursor_position) (GtkSnippetsEditor* editor,int* x, int* y);
	void (*insert_text_in_cursor) (GtkSnippetsEditor* editor, const gchar* text);*/
	
}

static void
gtk_snippets_editor_gsv_init (GtkSnippetsEditorGsv *editor)
{
	/* TODO: Add initialization code here */
	editor->source_view = NULL;
}

static void
gtk_snippets_editor_gsv_finalize (GObject *editor)
{
	/* TODO: Add deinitalization code here */
	g_object_unref(GTK_SNIPPETS_EDITOR_GSV(editor)->source_view);
	G_OBJECT_CLASS (parent_class)->finalize (editor);
}

static void
gtk_snippets_editor_gsv_class_init (GtkSnippetsEditorGsvClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtk_snippets_editor_gsv_finalize;
}

GType
gtk_snippets_editor_gsv_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsEditorGsvClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtk_snippets_editor_gsv_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsEditorGsv), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtk_snippets_editor_gsv_init, /* instance_init */
			NULL /* value_table */
		};
		
		static const GInterfaceInfo interface_info = {
			(GInterfaceInitFunc) gtk_snippets_editor_gsv_interface_init,    /* interface_init */
			NULL,               /* interface_finalize */
			NULL                /* interface_data */
		};
		
		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsEditorGsv",
		                                   &our_info, 0);
		
		g_type_add_interface_static (our_type,
                                 GTK_TYPE_SNIPPETS_EDITOR,
                                 &interface_info);
	}

	return our_type;
}

GtkSnippetsEditorGsv* 
gtk_snippets_editor_gsv_new (GtkSourceView *view)
{
	GtkSnippetsEditorGsv *editor;

	editor = GTK_SNIPPETS_EDITOR_GSV(g_object_new(GTK_TYPE_SNIPPETS_EDITOR_GSV, NULL));

	g_object_ref(view);
	editor->source_view = view;
	
	return editor;	
}
