/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-inplaceparser.h
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

#ifndef _GTKSNIPPETS_INPLACEPARSER_H
#define _GTKSNIPPETS_INPLACEPARSER_H

#include <glib-object.h>
#include <glib.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define GTKSNIPPETS_TYPE_INPLACEPARSER             (gtksnippets_inplaceparser_get_type ())
#define GTKSNIPPETS_INPLACEPARSER(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTKSNIPPETS_TYPE_INPLACEPARSER, GtkSnippetsInPlaceParser))
#define GTKSNIPPETS_INPLACEPARSER_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTKSNIPPETS_TYPE_INPLACEPARSER, GtkSnippetsInPlaceParserClass)
#define GTKSNIPPETS_IS_INPLACEPARSER(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTKSNIPPETS_TYPE_INPLACEPARSER))
#define GTKSNIPPETS_IS_INPLACEPARSER_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTKSNIPPETS_TYPE_INPLACEPARSER))
#define GTKSNIPPETS_INPLACEPARSER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTKSNIPPETS_TYPE_INPLACEPARSER, GtkSnippetsInPlaceParserClass))

typedef struct _GtkSnippetsInPlaceParserPrivate GtkSnippetsInPlaceParserPrivate;
typedef struct _GtkSnippetsInPlaceParserClass GtkSnippetsInPlaceParserClass;
typedef struct _GtkSnippetsInPlaceParser GtkSnippetsInPlaceParser;

struct _GtkSnippetsInPlaceParserClass
{
	GObjectClass parent_class;
};

struct _GtkSnippetsInPlaceParser
{
	GObject parent_instance;
	GtkSnippetsInPlaceParserPrivate *priv;
};

GType 
gtksnippets_inplaceparser_get_type (void) G_GNUC_CONST;

GtkSnippetsInPlaceParser*
gtksnippets_inplaceparser_new(GtkTextView *view);

/*Return true if the content contains variables */
gboolean
gtksnippets_inplaceparser_activate(GtkSnippetsInPlaceParser *self, const gchar* content);

gboolean
gtksnippets_inplaceparser_deactivate(GtkSnippetsInPlaceParser *self);

G_END_DECLS

#endif
