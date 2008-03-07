/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gsnippets-item.c
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
 
  
#include "gsnippets-item.h"

struct _GSnippetsItemPrivate
{
	gint id;
	gint lang_id;
	gchar *name;
	gchar *content;
};

static GObjectClass* parent_class = NULL;

#define GSNIPPETS_ITEM_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GSNIPPETS_TYPE_ITEM, GSnippetsItemPrivate))


static void
gsnippets_item_init (GSnippetsItem *self)
{
	self->priv = GSNIPPETS_ITEM_GET_PRIVATE(self);
	self->priv->id = -1;
	self->priv->lang_id = -1;
	self->priv->name = NULL;
	self->priv->content = NULL;
}

static void
gsnippets_item_finalize (GObject *object)
{
	GSnippetsItem *self = GSNIPPETS_ITEM(object);
	
	g_free (self->priv->name);
	g_free (self->priv->content);
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
	
}

static void
gsnippets_item_class_init (GSnippetsItemClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gsnippets_item_finalize;

	g_type_class_add_private (object_class, sizeof(GSnippetsItemPrivate));
}

GType
gsnippets_item_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GSnippetsItemClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gsnippets_item_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GSnippetsItem), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gsnippets_item_init, /* instance_init */
			NULL /* value_table */  
		};

		our_type = g_type_register_static (G_TYPE_OBJECT, "GSnippetsItem",
		                                   &our_info, 0);
	}

	return our_type;
}

static GSnippetsItem*
gsnippets_item_new(void)
{
	GSnippetsItem *self = GSNIPPETS_ITEM(g_object_new (GSNIPPETS_TYPE_ITEM, NULL));
	return self;
}

GSnippetsItem*
gsnippets_item_new_full(gint id,
			const gchar* name,
			const gchar* content,
			gint lang_id)
{
	GSnippetsItem *self = gsnippets_item_new();
	self->priv->id = id;
	self->priv->name = g_strdup(name);
	self->priv->content = g_strdup(content);
	self->priv->lang_id = lang_id;
	return self;
}

gint
gsnippets_item_get_id(GSnippetsItem *self)
{
	return self->priv->id;
}

const gchar*
gsnippets_item_get_name(GSnippetsItem *self)
{
	return self->priv->name;
}

const gchar*
gsnippets_item_get_content(GSnippetsItem *self)
{
	return self->priv->content;
}

void
gsnippets_item_set_content(GSnippetsItem *self, const gchar* content)
{
	g_free(self->priv->content);
	self->priv->content = g_strdup(content);
}

void
gsnippets_item_set_id(GSnippetsItem *self,gint id)
{
	self->priv->id = id;
}

gint
gsnippets_item_get_lang_id(GSnippetsItem *self)
{
	return self->priv->lang_id;
}

