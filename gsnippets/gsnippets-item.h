/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-item.h
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

#ifndef _GSNIPPETS_ITEM_H
#define _GSNIPPETS_ITEM_H

#include <glib-object.h>
#include <glib.h>

G_BEGIN_DECLS

#define GSNIPPETS_TYPE_ITEM             (gsnippets_item_get_type ())
#define GSNIPPETS_ITEM(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GSNIPPETS_TYPE_ITEM, GSnippetsItem))
#define GSNIPPETS_ITEM_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GSNIPPETS_TYPE_ITEM, GSnippetsItemClass)
#define GSNIPPETS_IS_ITEM(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GSNIPPETS_TYPE_ITEM))
#define GSNIPPETS_IS_ITEM_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GSNIPPETS_TYPE_ITEM))
#define GSNIPPETS_ITEM_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GSNIPPETS_TYPE_ITEM, GSnippetsItemClass))

typedef struct _GSnippetsItemPrivate GSnippetsItemPrivate;
typedef struct _GSnippetsItemClass GSnippetsItemClass;
typedef struct _GSnippetsItem GSnippetsItem;

struct _GSnippetsItemClass
{
	GObjectClass parent_class;
};

struct _GSnippetsItem
{
	GObject parent_instance;
	GSnippetsItemPrivate *priv;
};

GType 
gsnippets_item_get_type (void) G_GNUC_CONST;

/**
 * gsnippets_item_new_full:
 * @id: The snippet id
 * @name: The snippet name
 * @content: The snippet content
 * @lang_id: The lang id of this snippet
 *
 * This function creates a new #GSnippetsItem object.
 *
 */
GSnippetsItem*
gsnippets_item_new_full(gint id,
			const gchar* name,
			const gchar* content,
			gint lang_id);

/**
 * gsnippets_item_get_id:
 * @self: The #GSnippetsItem
 *
 * Returns The snippet id into the database
 */
gint
gsnippets_item_get_id(GSnippetsItem *self);

/**
 * gsnippets_item_set_id:
 * @self: The #GSnippetsItem
 * @id: The snipept id
 *
 * Set the id for this snippet into the object
 *
 */
void
gsnippets_item_set_id(GSnippetsItem *self,gint id);

/**
 * gsnippets_item_get_name:
 * @self: The #GSnippetsItem
 *
 * Returns The snippet name
 */
const gchar*
gsnippets_item_get_name(GSnippetsItem *self);

/**
 * gsnippets_item_get_content:
 * @self: The #GSnippetsItem
 *
 * Returns The snippet content
 */
const gchar*
gsnippets_item_get_content(GSnippetsItem *self);

/**
 * gsnippets_item_set_content:
 * @self: The #GSnippetsItem
 * @content: The content to be inserted
 *
 * This function sets the snippet content. Internally copy the content.
 *
 */
void
gsnippets_item_set_content(GSnippetsItem *self, const gchar* content);

/**
 * gsnippets_item_get_lang_id:
 * @self: The #GSnippetsItem
 * 
 * Returns The lang id of this snippet
 *
 */
gint
gsnippets_item_get_lang_id(GSnippetsItem *self);

G_END_DECLS

#endif
