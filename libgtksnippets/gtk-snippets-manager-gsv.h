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

#ifndef _GTK_SNIPPETS_MANAGER_GSV_H_
#define _GTK_SNIPPETS_MANAGER_GSV_H_

#include <glib-object.h>
#include <gtksourceview/gtksourceview.h>
#include "gtk-snippets-loader.h"

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_MANAGER_GSV             (gtk_snippets_manager_gsv_get_type ())
#define GTK_SNIPPETS_MANAGER_GSV(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_MANAGER_GSV, GtkSnippetsManagerGsv))
#define GTK_SNIPPETS_MANAGER_GSV_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_MANAGER_GSV, GtkSnippetsManagerGsvClass))
#define GTK_IS_SNIPPETS_MANAGER_GSV(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_MANAGER_GSV))
#define GTK_IS_SNIPPETS_MANAGER_GSV_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_MANAGER_GSV))
#define GTK_SNIPPETS_MANAGER_GSV_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SNIPPETS_MANAGER_GSV, GtkSnippetsManagerGsvClass))

typedef struct _GtkSnippetsManagerGsvClass GtkSnippetsManagerGsvClass;
typedef struct _GtkSnippetsManagerGsvPrivate GtkSnippetsManagerGsvPrivate;
typedef struct _GtkSnippetsManagerGsv GtkSnippetsManagerGsv;

struct _GtkSnippetsManagerGsvClass
{
	GObjectClass parent_class;
};

struct _GtkSnippetsManagerGsv
{
	GObject parent_instance;
	GtkSnippetsManagerGsvPrivate *priv;
};

GType gtk_snippets_manager_gsv_get_type (void) G_GNUC_CONST;

GtkSnippetsManagerGsv* gtk_snippets_manager_gsv_new (GtkSnippetsLoader *loader);

/**
 * gtk_snippets_manager_gsv_add_support:
 * @manager: #GtkSnippetsManagerGsv 
 * @editor: An editor to add support
 * @language: ----Editor language. We copy this data
 *
 * Add snippets support to an Editor. Only calling this function we have an
 *	Editor ready to use with snippets (all signals, popup management are controlled here)
 *
 **/
void 
gtk_snippets_manager_gsv_add_support (GtkSnippetsManagerGsv *manager, const gpointer editor, const gchar* language);

/**
* gtk_snippets_manager_gsv_remove_support:
* @manager: The manager
* @editor: The editor to remove snippets support
*
* Removes the snippets support to this editor. We disconect all signals and 
* free all internal data
**/
void
gtk_snippets_manager_gsv_remove_support (GtkSnippetsManagerGsv *manager, gpointer editor);

/**
* gtk_snippets_manager_gsv_get_loader:
* @manager: The manager
* @Returns: The loader assined to this manager when we create it.
**/
GtkSnippetsLoader*
gtk_snippets_manager_gsv_get_loader (GtkSnippetsManagerGsv *manager);


G_END_DECLS

#endif /* _GTK_SNIPPETS_MANAGER_GSV_H_ */
