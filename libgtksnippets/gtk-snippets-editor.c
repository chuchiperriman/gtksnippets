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

#include "gtk-snippets-editor.h"

static void
gtk_snippets_editor_base_init (gpointer *g_class)
{
	static gboolean initialized = FALSE;
	if (!initialized)
	{
		initialized = TRUE;
	}
	/* TODO: Add initialization code here */
}

GType
gtk_snippets_editor_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsEditorClass), /* class_size */
			(GBaseInitFunc)gtk_snippets_editor_base_init, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) NULL, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL, /* class_data */
			0, /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) NULL /* instance_init */
		};

		our_type = g_type_register_static (G_TYPE_INTERFACE, "GtkSnippetsEditor",
		                                   &our_info, 0);
	}

	return our_type;
}

gchar*
gtk_snippets_editor_get_current_word (GtkSnippetsEditor* editor)
{
	return GTK_SNIPPETS_EDITOR_GET_CLASS (editor)->get_current_word(editor);
}

void
gtk_snippets_editor_get_cursor_position(GtkSnippetsEditor* editor,int* x, int* y)
{
	GTK_SNIPPETS_EDITOR_GET_CLASS (editor)->get_cursor_position(editor,x,y);
}

void
gtk_snippets_editor_insert_text_in_cursor(GtkSnippetsEditor* editor, const gchar* text)
{
	GTK_SNIPPETS_EDITOR_GET_CLASS (editor)->insert_text_in_cursor(editor, text);
}

GObject*
gtk_snippets_editor_get_widget(GtkSnippetsEditor* editor)
{
	return GTK_SNIPPETS_EDITOR_GET_CLASS (editor)->get_widget(editor);
}
