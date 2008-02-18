/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-manager.h
 *
 *  Copyright (C) 2008 - ChuchiPerriman <chuchiperriman@gmail.com>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.

 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef _GTKSNIPPETS_MANAGER_H
#define _GTKSNIPPETS_MANAGER_H

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTKSNIPPETS_TYPE_MANAGER             (gtksnippets_manager_get_type ())
#define GTKSNIPPETS_MANAGER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTKSNIPPETS_TYPE_MANAGER, GtkSnippetsManager))
#define GTKSNIPPETS_MANAGER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTKSNIPPETS_TYPE_MANAGER, GtkSnippetsManagerClass)
#define GTKSNIPPETS_IS_MANAGER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTKSNIPPETS_TYPE_MANAGER))
#define GTKSNIPPETS_IS_MANAGER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTKSNIPPETS_TYPE_MANAGER))
#define GTKSNIPPETS_MANAGER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTKSNIPPETS_TYPE_MANAGER, GtkSnippetsManagerClass))

typedef struct _GtkSnippetsManagerPrivate GtkSnippetsManagerPrivate;
typedef struct _GtkSnippetsManagerClass GtkSnippetsManagerClass;
typedef struct _GtkSnippetsManager GtkSnippetsManager;

struct _GtkSnippetsManagerClass
{
	GtkWindowClass parent_class;
};

struct _GtkSnippetsManager
{
	GtkWindow parent;
	GtkSnippetsManagerPrivate *priv;
};

GType 
gtksnippets_manager_get_type (void) G_GNUC_CONST;

GtkSnippetsManager*
gtksnippets_manager_new(void);

G_END_DECLS

#endif
