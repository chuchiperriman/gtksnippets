/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-varsdialog.h
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

#ifndef _GTKSNIPPETS_VARSDIALOG_H
#define _GTKSNIPPETS_VARSDIALOG_H

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTKSNIPPETS_TYPE_VARSDIALOG             (gtksnippets_varsdialog_get_type ())
#define GTKSNIPPETS_VARSDIALOG(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTKSNIPPETS_TYPE_VARSDIALOG, GtkSnippetsVarsDialog))
#define GTKSNIPPETS_VARSDIALOG_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTKSNIPPETS_TYPE_VARSDIALOG, GtkSnippetsVarsDialogClass)
#define GTKSNIPPETS_IS_VARSDIALOG(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTKSNIPPETS_TYPE_VARSDIALOG))
#define GTKSNIPPETS_IS_VARSDIALOG_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTKSNIPPETS_TYPE_VARSDIALOG))
#define GTKSNIPPETS_VARSDIALOG_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTKSNIPPETS_TYPE_VARSDIALOG, GtkSnippetsVarsDialogClass))

typedef struct _GtkSnippetsVarsDialogPrivate GtkSnippetsVarsDialogPrivate;
typedef struct _GtkSnippetsVarsDialogClass GtkSnippetsVarsDialogClass;
typedef struct _GtkSnippetsVarsDialog GtkSnippetsVarsDialog;

struct _GtkSnippetsVarsDialogClass
{
	GtkDialogClass parent_class;
};

struct _GtkSnippetsVarsDialog
{
	GtkDialog parent;
	GtkSnippetsVarsDialogPrivate *priv;
};

GType 
gtksnippets_varsdialog_get_type (void) G_GNUC_CONST;

GtkSnippetsVarsDialog*
gtksnippets_varsdialog_new(const gchar *snippet_content);

gchar*
gtksnippets_varsdialog_get_content(GtkSnippetsVarsDialog *self);

gchar*
gtksnippets_varsdialog_show_and_parse(const gchar *snippet_content);

G_END_DECLS

#endif
