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
#include "gtk-snippets-loader.h"

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_MANAGEMENT_UI             (gtk_snippets_management_ui_get_type ())
#define GTK_SNIPPETS_MANAGEMENT_UI(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUI))
#define GTK_SNIPPETS_MANAGEMENT_UI_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUIClass))
#define GTK_IS_SNIPPETS_MANAGEMENT_UI(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_MANAGEMENT_UI))
#define GTK_IS_SNIPPETS_MANAGEMENT_UI_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_MANAGEMENT_UI))
#define GTK_SNIPPETS_MANAGEMENT_UI_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SNIPPETS_MANAGEMENT_UI, GtkSnippetsManagementUIClass))

/**
 * GtkSnippetsManagementUI:
 *	@parent: GObject Parent
 * @priv: Private data
 *
 * This object create a dialog to manage the snippets.
**/
typedef struct _GtkSnippetsManagementUI GtkSnippetsManagementUI;
typedef struct _GtkSnippetsManagementUIClass GtkSnippetsManagementUIClass;
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

/**
* gtk_smngui_create_source_view:
* @Returns: Creates the new sourceview to use in the management ui
*
* It is internal, do not use
**/
GtkWidget*
gtk_smngui_create_source_view();

/**
* gtk_snippets_management_ui_new:
* @Returns: The new snippets management ui
**/
GtkSnippetsManagementUI* 
gtk_snippets_management_ui_new (GtkSnippetsLoader *loader);

/**
* gtk_snippets_management_ui_show:
* @mng_ui: The ui.
*
* Show the management window
**/
void
gtk_snippets_management_ui_show(GtkSnippetsManagementUI* mng_ui);

G_END_DECLS

#endif /* _GTK_SNIPPETS_MANAGEMENT_UI_H_ */
