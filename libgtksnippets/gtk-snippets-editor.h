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

#ifndef _GTK_SNIPPETS_EDITOR_H_
#define _GTK_SNIPPETS_EDITOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_EDITOR             (gtk_snippets_editor_get_type ())
#define GTK_SNIPPETS_EDITOR(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_EDITOR, GtkSnippetsEditor))
#define GTK_SNIPPETS_EDITOR_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_EDITOR, GtkSnippetsEditorClass))
#define GTK_IS_SNIPPETS_EDITOR(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_EDITOR))
#define GTK_IS_SNIPPETS_EDITOR_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_EDITOR))
#define GTK_SNIPPETS_EDITOR_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GTK_TYPE_SNIPPETS_EDITOR, GtkSnippetsEditorClass))

typedef struct _GtkSnippetsEditorClass GtkSnippetsEditorClass;
typedef struct _GtkSnippetsEditor GtkSnippetsEditor;

struct _GtkSnippetsEditorClass
{
	GTypeInterface parent;
	gchar* (*get_current_word) (GtkSnippetsEditor* editor);
	void (*get_cursor_position) (GtkSnippetsEditor* editor,int* x, int* y);
	void (*insert_text_in_cursor) (GtkSnippetsEditor* editor, const gchar* text);
	GObject* (*get_widget) (GtkSnippetsEditor* editor);
};

GType gtk_snippets_editor_get_type (void) G_GNUC_CONST;

gchar* gtk_snippets_editor_get_current_word (GtkSnippetsEditor* editor);
void gtk_snippets_editor_get_cursor_position(GtkSnippetsEditor* editor,int* x, int* y);
void gtk_snippets_editor_insert_text_in_cursor(GtkSnippetsEditor* editor, const gchar* text);
GObject* gtk_snippets_editor_get_widget(GtkSnippetsEditor* editor);

G_END_DECLS

#endif /* _GTK_SNIPPETS_EDITOR_H_ */
