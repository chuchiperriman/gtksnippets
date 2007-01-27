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

#ifndef _GTK_SNIPPETS_MANAGEMENT_UI_H_
#define _GTK_SNIPPETS_MANAGEMENT_UI_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_MANAGEMENT_UI             (gtk_snippets_management_ui_get_type ())
#define GTK_SNIPPETS_MANAGEMENT_UI(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUI))
#define GTK_SNIPPETS_MANAGEMENT_UI_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUIClass))
#define GTK_IS_SNIPPETS_MANAGEMENT_UI(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_MANAGEMENT_UI))
#define GTK_IS_SNIPPETS_MANAGEMENT_UI_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_MANAGEMENT_UI))
#define GTK_SNIPPETS_MANAGEMENT_UI_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUIClass))

typedef struct _GtkSnippetsManagementUIClass GtkSnippetsManagementUIClass;
typedef struct _GtkSnippetsManagementUI GtkSnippetsManagementUI;
typedef struct _GtkSnippetsManagementUIPrivate GtkSnippetsManagementUIPrivate;

struct _GtkSnippetsManagementUIClass
{
	GObjectClass parent_class;
};

struct _GtkSnippetsManagementUI
{
	GObject parent_instance;
	GtkSnippetsManagementUIPrivate *priv;
};

GType gtk_snippets_management_ui_get_type (void) G_GNUC_CONST;

GtkSnippetsManagementUI* gtk_snippets_management_ui_new (void);

G_END_DECLS

#endif /* _GTK_SNIPPETS_MANAGEMENT_UI_H_ */
