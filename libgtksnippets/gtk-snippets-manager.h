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

#ifndef _GTK_SNIPPETS_MANAGER_H_
#define _GTK_SNIPPETS_MANAGER_H_

#include <glib-object.h>
#include <gtksourceview/gtksourceview.h>
#include "gtk-snippets-loader.h"
#include "gtk-snippets-editor.h"

G_BEGIN_DECLS

#define GTK_TYPE_SNIPPETS_MANAGER             (gtk_snippets_manager_get_type ())
#define GTK_SNIPPETS_MANAGER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SNIPPETS_MANAGER, GtkSnippetsManager))
#define GTK_SNIPPETS_MANAGER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SNIPPETS_MANAGER, GtkSnippetsManagerClass))
#define GTK_IS_SNIPPETS_MANAGER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SNIPPETS_MANAGER))
#define GTK_IS_SNIPPETS_MANAGER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SNIPPETS_MANAGER))
#define GTK_SNIPPETS_MANAGER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SNIPPETS_MANAGER, GtkSnippetsManagerClass))

typedef struct _GtkSnippetsManagerClass GtkSnippetsManagerClass;
typedef struct _GtkSnippetsManagerPrivate GtkSnippetsManagerPrivate;
typedef struct _GtkSnippetsManager GtkSnippetsManager;

struct _GtkSnippetsManagerClass
{
	GObjectClass parent_class;
};

struct _GtkSnippetsManager
{
	GObject parent_instance;
	GtkSnippetsManagerPrivate *priv;
};

GType gtk_snippets_manager_get_type (void) G_GNUC_CONST;

GtkSnippetsManager* gtk_snippets_manager_new (GtkSnippetsLoader *loader);

/**
 * gtk_snippets_manager_add_support:
 * @manager: #GtkSnippetsManager 
 * @editor: An editor to add support
 * @language: ----Editor language. We copy this data
 *
 * Add snippets support to an Editor. Only calling this function we have an
 *	Editor ready to use with snippets (all signals, popup management are controlled here)
 *
 **/
void 
gtk_snippets_manager_add_support (GtkSnippetsManager *manager, GtkSnippetsEditor* editor, const gchar* language);

/**
* gtk_snippets_manager_remove_support:
* @manager: The manager
* @editor: The editor to remove snippets support
*
* Removes the snippets support to this editor. We disconect all signals and 
* free all internal data
**/
void
gtk_snippets_manager_remove_support (GtkSnippetsManager *manager, GtkSnippetsEditor* editor);

/**
* gtk_snippets_manager_get_loader:
* @manager: The manager
* @Returns: The loader assined to this manager when we create it.
**/
GtkSnippetsLoader*
gtk_snippets_manager_get_loader (GtkSnippetsManager *manager);


G_END_DECLS

#endif /* _GTK_SNIPPETS_MANAGER_H_ */
