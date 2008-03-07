/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-db.h
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

#ifndef _GSNIPPETS_DB_H
#define _GSNIPPETS_DB_H

#include <glib-object.h>
#include <glib.h>

#include "gsnippets-item.h"

G_BEGIN_DECLS

#define GSNIPPETS_TYPE_DB             (gsnippets_db_get_type ())
#define GSNIPPETS_DB(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSNIPPETS_TYPE_DB, GSnippetsDb))
#define GSNIPPETS_DB_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSNIPPETS_TYPE_DB, GSnippetsDbClass)
#define GSNIPPETS_IS_DB(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSNIPPETS_TYPE_DB))
#define GSNIPPETS_IS_DB_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSNIPPETS_TYPE_DB))
#define GSNIPPETS_DB_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GSNIPPETS_TYPE_DB, GSnippetsDbClass))

typedef struct _GSnippetsDbPrivate GSnippetsDbPrivate;
typedef struct _GSnippetsDbClass GSnippetsDbClass;
typedef struct _GSnippetsDb GSnippetsDb;

struct _GSnippetsDbClass
{
	GObjectClass parent_class;
};

struct _GSnippetsDb
{
	GObject parent_instance;
	GSnippetsDbPrivate *priv;
};

GType 
gsnippets_db_get_type (void) G_GNUC_CONST;

GSnippetsDb*
gsnippets_db_new(void);

void
gsnippets_db_connect(GSnippetsDb *self);

void
gsnippets_db_disconnect (GSnippetsDb *self);

GSnippetsItem*
gsnippets_db_load(GSnippetsDb *self, gint id);

void
gsnippets_db_save(GSnippetsDb *self, GSnippetsItem* snippet);

void
gsnippets_db_delete(GSnippetsDb *self, gint id);

GSList*
gsnippets_db_get_all(GSnippetsDb *self);

GSList*
gsnippets_db_get_by_lang_name(GSnippetsDb *self, const gchar* language_name);

GSList*
gsnippets_db_get_by_lang_id(GSnippetsDb *self, gint lang_id);

gint
gsnippets_db_lang_get_id(GSnippetsDb *self, const gchar* lang_name);

gint
gsnippets_db_lang_insert(GSnippetsDb *self, const gchar* lang_name);

G_END_DECLS

#endif
