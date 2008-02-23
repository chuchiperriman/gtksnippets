/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-dialog.h
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

#ifndef _GTKSNIPPETS_DIALOG_H
#define _GTKSNIPPETS_DIALOG_H

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTKSNIPPETS_TYPE_DIALOG             (gtksnippets_dialog_get_type ())
#define GTKSNIPPETS_DIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTKSNIPPETS_TYPE_DIALOG, GtkSnippetsDialog))
#define GTKSNIPPETS_DIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTKSNIPPETS_TYPE_DIALOG, GtkSnippetsDialogClass)
#define GTKSNIPPETS_IS_DIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTKSNIPPETS_TYPE_DIALOG))
#define GTKSNIPPETS_IS_DIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTKSNIPPETS_TYPE_DIALOG))
#define GTKSNIPPETS_DIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTKSNIPPETS_TYPE_DIALOG, GtkSnippetsDialogClass))

typedef struct _GtkSnippetsDialogPrivate GtkSnippetsDialogPrivate;
typedef struct _GtkSnippetsDialogClass GtkSnippetsDialogClass;
typedef struct _GtkSnippetsDialog GtkSnippetsDialog;

struct _GtkSnippetsDialogClass
{
	GtkWindowClass parent_class;
};

struct _GtkSnippetsDialog
{
	GtkWindow parent;
	GtkSnippetsDialogPrivate *priv;
};

GType 
gtksnippets_dialog_get_type (void) G_GNUC_CONST;

GtkWidget*
gtksnippets_dialog_source_new(gchar *widget_name, gchar *string1, gchar
                       *string2, gint int1, gint int2);

GtkSnippetsDialog*
gtksnippets_dialog_new(void);

G_END_DECLS

#endif
