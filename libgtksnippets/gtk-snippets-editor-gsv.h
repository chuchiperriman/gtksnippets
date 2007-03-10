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

#ifndef _GTK_SNIPPETS_EDITOR_GSV_H_
#define _GTK_SNIPPETS_EDITOR_GSV_H_

#include <glib-object.h>
#include <gtksourceview/gtksourceview.h>

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_EDITOR_GSV             (gtk_snippets_editor_gsv_get_type ())
#define GTK_SNIPPETS_EDITOR_GSV(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_EDITOR_GSV, GtkSnippetsEditorGsv))
#define GTK_SNIPPETS_EDITOR_GSV_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_EDITOR_GSV, GtkSnippetsEditorGsvClass))
#define GTK_IS_SNIPPETS_EDITOR_GSV(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_EDITOR_GSV))
#define GTK_IS_SNIPPETS_EDITOR_GSV_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_EDITOR_GSV))
#define GTK_SNIPPETS_EDITOR_GSV_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SNIPPETS_EDITOR_GSV, GtkSnippetsEditorGsvClass))

typedef struct _GtkSnippetsEditorGsvClass GtkSnippetsEditorGsvClass;
typedef struct _GtkSnippetsEditorGsv GtkSnippetsEditorGsv;

struct _GtkSnippetsEditorGsvClass
{
	GObjectClass parent_class;
};

struct _GtkSnippetsEditorGsv
{
	GObject parent_instance;
	GtkSourceView* source_view;
};

GType gtk_snippets_editor_gsv_get_type (void) G_GNUC_CONST;

G_END_DECLS

#endif /* _GTK_SNIPPETS_EDITOR_GSV_H_ */
