/***************************************************************************
 *            gtk-snippet.c
 *
 *  mié ene 10 10:15:00 2007
 *  Copyright  2007  chuchiperriman
 *  chuchiperriman@gmail.com
 ***************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "gtk-snippet.h"
 
static void gtk_snippet_class_init(GtkSnippetClass *klass);
static void gtk_snippet_init(GtkSnippet *sp);
static void gtk_snippet_finalize(GObject *object);

struct _GtkSnippetPrivate {
	/* Place Private Members Here */
	gchar *name;
	gchar *language;
	gchar *tag;
	gchar *description;
	gchar *text;
};

static GObjectClass *parent_class = NULL;

GType
gtk_snippet_get_type()
{
	static GType type = 0;

	if(type == 0) {
		static const GTypeInfo our_info = {
			sizeof (GtkSnippetClass),
			NULL,
			NULL,
			(GClassInitFunc)gtk_snippet_class_init,
			NULL,
			NULL,
			sizeof (GtkSnippet),
			0,
			(GInstanceInitFunc)gtk_snippet_init,
		};

		type = g_type_register_static(G_TYPE_OBJECT, 
			"GtkSnippet", &our_info, 0);
	}

	return type;
}

static void
gtk_snippet_class_init(GtkSnippetClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS(klass);

	parent_class = g_type_class_peek_parent(klass);
	object_class->finalize = gtk_snippet_finalize;
	
	/* Create signals here:
	   gtk_snippet_signals[SIGNAL_TYPE_EXAMPLE] = g_signal_new(...)
 	*/
}

static void
gtk_snippet_init(GtkSnippet *obj)
{
	obj->priv = g_new0(GtkSnippetPrivate, 1);
	/* Initialize private members, etc. */
	obj->priv->name = NULL;
	obj->priv->language = NULL;
	obj->priv->tag = NULL;
	obj->priv->description = NULL;
	obj->priv->text = NULL;
	g_debug("Construido snippet");
}

static void
gtk_snippet_finalize(GObject *object)
{
	GtkSnippet *cobj;
	cobj = GTK_SNIPPET(object);
	
	/* Free private members, etc. */
	g_free(cobj->priv->name);
	g_free(cobj->priv->language);
	g_free(cobj->priv->tag);
	g_free(cobj->priv->description);
	g_free(cobj->priv->text);
	
	g_free(cobj->priv);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	
	g_debug("Destruido snippet");
}

GtkSnippet*
gtk_snippet_new(gchar *name,
				gchar *language,
				gchar *tag,
				gchar *description,
				gchar *text)
{
	g_return_val_if_fail(name!=NULL,NULL);
	g_return_val_if_fail(text!=NULL,NULL);
	
	GtkSnippet *obj;

	obj = GTK_SNIPPET(g_object_new(GTK_TYPE_SNIPPET, NULL));
	
	obj->priv->name = g_strdup(name);
	obj->priv->language = g_strdup(language);
	obj->priv->tag = g_strdup(tag);
	obj->priv->description = g_strdup(description);
	obj->priv->text = g_strdup(text);
	
	return obj;
}

gchar*
gtk_snippet_get_language(GtkSnippet *snippet)
{
	return snippet->priv->language;
}

gchar*
gtk_snippet_get_name(GtkSnippet *snippet)
{
	return snippet->priv->name;
}

gchar*
gtk_snippet_get_tag(GtkSnippet *snippet)
{
	return snippet->priv->tag;
}

gchar*
gtk_snippet_get_text(GtkSnippet *snippet)
{
	return snippet->priv->text;
}
