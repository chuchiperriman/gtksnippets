/*
 * ##(FILENAME) - ##(DESCRIPTION)
 *
 * Copyright (C) ##(DATE_YEAR) - ##(AUTHOR_FULLNAME)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __##(PLUGIN_ID.upper)_PLUGIN_H__
#define __##(PLUGIN_ID.upper)_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include "gtk-text-completion-provider.h"
#include "gtk-text-completion-popup.h"

G_BEGIN_DECLS


#define TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER (gtc_##(PLUGIN_ID.lower)_provider_get_type ())
#define GTC_##(PLUGIN_ID.upper)_PROVIDER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER, Gtc##(PLUGIN_ID.camel)Provider))
#define GTC_##(PLUGIN_ID.upper)_PROVIDER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER, Gtc##(PLUGIN_ID.camel)ProviderClass))
#define IS_GTC_##(PLUGIN_ID.upper)_PROVIDER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER))
#define IS_GTC_##(PLUGIN_ID.upper)_PROVIDER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER))
#define GTC_##(PLUGIN_ID.upper)_PROVIDER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_GTC_##(PLUGIN_ID.upper)_PROVIDER, Gtc##(PLUGIN_ID.camel)ProviderClass))

typedef struct _Gtc##(PLUGIN_ID.camel)Provider Gtc##(PLUGIN_ID.camel)Provider;
typedef struct _Gtc##(PLUGIN_ID.camel)ProviderClass Gtc##(PLUGIN_ID.camel)ProviderClass;
typedef struct _Gtc##(PLUGIN_ID.camel)ProviderPrivate Gtc##(PLUGIN_ID.camel)ProviderPrivate;

struct _Gtc##(PLUGIN_ID.camel)Provider {
	GObject parent;
	Gtc##(PLUGIN_ID.camel)ProviderPrivate *priv;	
};

struct _Gtc##(PLUGIN_ID.camel)ProviderClass {
	GObjectClass parent;
};

GType gtc_##(PLUGIN_ID.lower)_provider_get_type ();

Gtc##(PLUGIN_ID.camel)Provider* gtc_##(PLUGIN_ID.lower)_provider_new();

G_END_DECLS

#endif
