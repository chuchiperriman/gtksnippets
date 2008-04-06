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

/**
 * gsnippets_db_new:
 *
 * Returns A new #GSnippetsDb object without connect.
 */
GSnippetsDb*
gsnippets_db_new(void);

/**
 * gsnippets_db_connect:
 * @self: The #GSnippetsDb object
 * 
 * Open the connection to the snippets database
 * 
 */
void
gsnippets_db_connect(GSnippetsDb *self);

/**
 * gsnippets_db_disconnect:
 * @self: The #GSnippetsDb object
 * 
 * Closes the connection to the snippets database
 */
void
gsnippets_db_disconnect (GSnippetsDb *self);

/**
 * gsnippets_db_load:
 * @self: The #GSnippetsDb object
 * @id: The snippet id to be loaded
 *
 * Load a snippet by id.
 *
 * Returns A new allocated #GSnippetsItem
 */
GSnippetsItem*
gsnippets_db_load(GSnippetsDb *self, gint id);

/**
 * gsnippets_db_save:
 * @self: The #GSnippetsDb object
 * @snippet: The snippet tobe saved
 *
 * This function saves the snippet. It insert or update the snippet if not exists
 *
 */
void
gsnippets_db_save(GSnippetsDb *self, GSnippetsItem* snippet);

/**
 * gsnippets_db_delete:
 * @self: The #GSnippetsDb object
 * @id: The snippet id to be deleted
 *
 * This function delete a snippet from the database
 *
 */
void
gsnippets_db_delete(GSnippetsDb *self, gint id);

/**
 * gsnippets_db_get_all:
 * @self: The #GSnippetsDb object
 * 
 * Returns All the database snippets or NULL
 */
GSList*
gsnippets_db_get_all(GSnippetsDb *self);

/**
 * gsnippets_db_get_by_lang_name:
 * @self: The #GSnippetsDb object
 * @lang_name: The lang name.
 *
 * This search into the database all snippets of this language
 *
 * Returns A new allocated list of #GSnippetsItem objects or NULL
 */
GSList*
gsnippets_db_get_by_lang_name(GSnippetsDb *self, const gchar* language_name);

/**
 * gsnippets_db_lang_get_id:
 * @self: The #GSnippetsDb object
 * @lang_id: The database lang id
 *
 * This function search into the database all snippets of this language
 *
 * Returns A new allocated list of #GSnippetsItem objects or NULL
 */
GSList*
gsnippets_db_get_by_lang_id(GSnippetsDb *self, gint lang_id);

/**
 * gsnippets_db_lang_get_id:
 * @self: The #GSnippetsDb object
 * @lang_name: The language name
 *
 *
 * Returns The id of this language or -1 if it cannot find the language.
 */
gint
gsnippets_db_lang_get_id(GSnippetsDb *self, const gchar* lang_name);

/**
 * gsnippets_db_lang_insert:
 * @self: The #GSnippetsDb object
 * @lang_name: The language name to be inserted
 *
 * Insert a new language into the database
 *
 * Returns The inserted language id.
 */
gint
gsnippets_db_lang_insert(GSnippetsDb *self, const gchar* lang_name);

G_END_DECLS

#endif
