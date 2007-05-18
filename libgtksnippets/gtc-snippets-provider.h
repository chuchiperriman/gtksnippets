/*
 * gtc-snippets-provider.h - Type here a short description of your plugin
 *
 * Copyright (C) 2007 - chuchi
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

#ifndef __SNIPPETS_PLUGIN_H__
#define __SNIPPETS_PLUGIN_H__

#include <glib.h>
#include <glib-object.h>
#include "gtk-text-completion-provider.h"
#include "gtk-text-completion-popup.h"
#include "gtk-snippets-loader.h"

G_BEGIN_DECLS


#define TYPE_GTC_SNIPPETS_PROVIDER (gtc_snippets_provider_get_type ())
#define GTC_SNIPPETS_PROVIDER(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), TYPE_GTC_SNIPPETS_PROVIDER, GtcSnippetsProvider))
#define GTC_SNIPPETS_PROVIDER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), TYPE_GTC_SNIPPETS_PROVIDER, GtcSnippetsProviderClass))
#define IS_GTC_SNIPPETS_PROVIDER(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TYPE_GTC_SNIPPETS_PROVIDER))
#define IS_GTC_SNIPPETS_PROVIDER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), TYPE_GTC_SNIPPETS_PROVIDER))
#define GTC_SNIPPETS_PROVIDER_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), TYPE_GTC_SNIPPETS_PROVIDER, GtcSnippetsProviderClass))

typedef struct _GtcSnippetsProvider GtcSnippetsProvider;
typedef struct _GtcSnippetsProviderClass GtcSnippetsProviderClass;
typedef struct _GtcSnippetsProviderPrivate GtcSnippetsProviderPrivate;

struct _GtcSnippetsProvider {
	GObject parent;
	GtcSnippetsProviderPrivate *priv;	
};

struct _GtcSnippetsProviderClass {
	GObjectClass parent;
};

GType gtc_snippets_provider_get_type ();

GtcSnippetsProvider* gtc_snippets_provider_new(GtkSnippetsLoader *loader);

G_END_DECLS

#endif
