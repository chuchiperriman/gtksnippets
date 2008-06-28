/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8; coding: utf-8 -*-
 *  gtksnippets-inplaceparser.c
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
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
 
#include <gdk/gdkkeysyms.h>   
#include "gtksnippets-inplaceparser.h"
#include "../gsnippets/gsnippets-parser.h"

#define DELAY 300

typedef struct _SnippetVar SnippetVar;

struct _SnippetVar{
	gchar *name;
	GtkTextMark *start;
	GtkTextMark *end;
	GList *mirrors;
	GtkSnippetsInPlaceParser *parser;
};

/* Signals */
enum
{
	PARSER_START,
	PARSER_END,
	LAST_SIGNAL
};

static guint signals[LAST_SIGNAL] = { 0 };

struct _GtkSnippetsInPlaceParserPrivate
{
	GtkTextView *view;
	GList *vars;
	GtkTextTag *var_tag;
	gboolean active;
	GList *active_var_pos;
	guint timeout_id;
	gboolean updating;
	gboolean moving;
};
static GObjectClass* parent_class = NULL;

#define GTKSNIPPETS_INPLACEPARSER_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTKSNIPPETS_TYPE_INPLACEPARSER, GtkSnippetsInPlaceParserPrivate))

#define VAR_TAG_NAME "snippet_var"

/*###############SnippetVar functions##############*/

static SnippetVar*
snippet_new(GtkSnippetsInPlaceParser *parser)
{
	SnippetVar *var = g_new0(SnippetVar,1);
	var->mirrors=NULL;
	var->parser = parser;
	return var;
}

static void
snippetvar_free(GtkSnippetsInPlaceParser *self, SnippetVar *var)
{
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	gtk_text_buffer_delete_mark(buffer,var->start);
	gtk_text_buffer_delete_mark(buffer,var->end);
	g_free(var->name);
	GList *lista = var->mirrors;
	if (lista!=NULL)
	{
		do{
			snippetvar_free(self,(SnippetVar*)lista->data);
			lista = g_list_next(lista);
		}while(lista!=NULL);
		g_list_free(var->mirrors);
	}
	
	g_free(var);
}

static void
snippetvar_set_text(GtkTextBuffer *buffer, 
		SnippetVar *var,
		const gchar* text)
{
	/* Esto está por hacer todavía*/
	GtkTextIter start_var, end_var;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_var,var->start);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_var,var->end);
	gtk_text_buffer_delete(buffer,&start_var,&end_var);
	gtk_text_buffer_insert_with_tags_by_name(buffer,&start_var,text,-1,VAR_TAG_NAME,NULL);
}

static gchar*
snippetvar_get_text(GtkTextBuffer *buffer, 
		SnippetVar *var)
{
	GtkTextIter start_var, end_var;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_var,var->start);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_var,var->end);
	return gtk_text_buffer_get_text(buffer,&start_var,&end_var,FALSE);
}


/*#################################################*/
static void
gtksnippets_inplaceparser_init (GtkSnippetsInPlaceParser *self)
{
	g_debug("GtkSnippetsInPlaceParser init");
	self->priv = GTKSNIPPETS_INPLACEPARSER_GET_PRIVATE(self);
	self->priv->view = NULL;
	self->priv->vars = NULL;
	self->priv->var_tag = NULL;
	self->priv->active = FALSE;
	self->priv->active_var_pos = NULL;
	self->priv->timeout_id = 0;
	self->priv->moving = FALSE;
}

static void
gtksnippets_inplaceparser_finalize (GObject *object)
{
	g_debug("GtkSnippetsInPlaceParser finish");
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(object);
	
	gtksnippets_inplaceparser_deactivate(self);
	self->priv->active_var_pos = NULL;
	self->priv->timeout_id = 0;
	
	G_OBJECT_CLASS (parent_class)->finalize (object);
}

static void
gtksnippets_inplaceparser_class_init (GtkSnippetsInPlaceParserClass *klass)
{
	GObjectClass* object_class = G_OBJECT_CLASS (klass);	
	parent_class = G_OBJECT_CLASS (g_type_class_peek_parent (klass));

	object_class->finalize = gtksnippets_inplaceparser_finalize;

	g_type_class_add_private (object_class, sizeof(GtkSnippetsInPlaceParserPrivate));
	
	signals[PARSER_START] =
		g_signal_new ("parser-start",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			      0,
			      NULL, 
			      NULL,
			      g_cclosure_marshal_VOID__VOID, 
			      G_TYPE_NONE,
			      0);
	
	signals[PARSER_END] =
		g_signal_new ("parser-end",
			      G_TYPE_FROM_CLASS (klass),
			      G_SIGNAL_RUN_FIRST | G_SIGNAL_ACTION,
			      0,
			      NULL, 
			      NULL,
			      g_cclosure_marshal_VOID__VOID, 
			      G_TYPE_NONE,
			      0);
}

GType
gtksnippets_inplaceparser_get_type (void)
{
	static GType our_type = 0;

	if(our_type == 0)
	{
		static const GTypeInfo our_info =
		{
			sizeof (GtkSnippetsInPlaceParserClass), /* class_size */
			(GBaseInitFunc) NULL, /* base_init */
			(GBaseFinalizeFunc) NULL, /* base_finalize */
			(GClassInitFunc) gtksnippets_inplaceparser_class_init, /* class_init */
			(GClassFinalizeFunc) NULL, /* class_finalize */
			NULL /* class_data */,
			sizeof (GtkSnippetsInPlaceParser), /* instance_size */
			0, /* n_preallocs */
			(GInstanceInitFunc) gtksnippets_inplaceparser_init, /* instance_init */
			NULL /* value_table */  
		};
		our_type = g_type_register_static (G_TYPE_OBJECT, "GtkSnippetsInPlaceParser",
		                                   &our_info, 0);
	}

	return our_type;
}

GtkSnippetsInPlaceParser*
gtksnippets_inplaceparser_new(GtkTextView *view)
{
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(g_object_new (GTKSNIPPETS_TYPE_INPLACEPARSER, NULL));
	self->priv->view = view;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(view);
	self->priv->var_tag = gtk_text_buffer_create_tag (buffer, VAR_TAG_NAME,
	   		            "background", "yellow", NULL);
	
	return self;
}

static SnippetVar*
search_var(GtkSnippetsInPlaceParser *self, GtkTextBuffer *buffer,GtkTextIter *pos, GtkTextIter *limit)
{
	SnippetVar *var = NULL;
	GtkTextIter start, end, *temp_iter;
	gboolean found = gtk_text_iter_forward_search(pos,
				"${",
				GTK_TEXT_SEARCH_VISIBLE_ONLY,
				&start,
				NULL,
				limit);
	
	if (found)
	{
		temp_iter = gtk_text_iter_copy(&start);
		gtk_text_iter_forward_to_line_end(temp_iter);
		found = gtk_text_iter_forward_search(&start,
				"}",
				GTK_TEXT_SEARCH_VISIBLE_ONLY,
				&end,
				NULL,
				temp_iter);
		if (found)
		{
			gtk_text_iter_forward_char(&end);
			var = snippet_new(self);
			var->start = gtk_text_buffer_create_mark(buffer,
								NULL,
								&start,
								TRUE);
			var->end = gtk_text_buffer_create_mark(buffer,
								NULL,
								&end,
								FALSE);
			gtk_text_iter_forward_chars(&start,2);
			gtk_text_iter_forward_chars(&end,-1);
			var->name = gtk_text_buffer_get_text(buffer,
							&start,
							&end,
							FALSE);
		}
	}
	
	return var;
}

static void
set_active_var(GtkSnippetsInPlaceParser *self)
{
	self->priv->moving = TRUE;
	SnippetVar *var = (SnippetVar*)self->priv->active_var_pos->data;
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	GtkTextIter start,end;
	gtk_text_buffer_get_iter_at_mark(buffer,&start,var->start);
	gtk_text_buffer_get_iter_at_mark(buffer,&end,var->end);
	gtk_text_buffer_select_range(buffer,&start,&end);
}

static gboolean
active_prev_var(GtkSnippetsInPlaceParser *self)
{
	if(self->priv->active_var_pos == NULL)
		self->priv->active_var_pos = g_list_last(self->priv->vars);
	else
		self->priv->active_var_pos = g_list_previous(self->priv->active_var_pos);
		
	if (self->priv->active_var_pos==NULL)
		return FALSE;
	
	set_active_var(self);
	return TRUE;
}

static gboolean
active_next_var(GtkSnippetsInPlaceParser *self)
{
	if(self->priv->active_var_pos == NULL)
		self->priv->active_var_pos = self->priv->vars;
	else
		self->priv->active_var_pos = g_list_next(self->priv->active_var_pos);
		
	if (self->priv->active_var_pos==NULL)
		return FALSE;
	
	set_active_var(self);
	
	return TRUE;
}

static gboolean
update_mirrors_cb(gpointer user_data)
{
	SnippetVar *actual_var = (SnippetVar*)user_data;
	GtkSnippetsInPlaceParser *self = actual_var->parser;
	self->priv->updating = TRUE;
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	
	if (self->priv->active_var_pos==NULL)
	{
		self->priv->updating = FALSE;
		self->priv->timeout_id = 0;
		return FALSE;
	}
	
	gtk_text_buffer_begin_user_action(buffer);
	
	GList *list = actual_var->mirrors;
	gchar* text = snippetvar_get_text(buffer,actual_var);
	
	if (list!=NULL)
	{
		g_debug("update");
		do{
			self->priv->updating = TRUE;
			snippetvar_set_text(buffer, 
				list->data,
				text);
		}while((list=g_list_next(list))!=NULL);
	}
	g_free(text);
	self->priv->timeout_id = 0;
	gtk_text_buffer_end_user_action(buffer);
	self->priv->updating = FALSE;
	return FALSE;
}

void
view_insert_text_cb(GtkTextBuffer *buffer, 
		    GtkTextIter *location, 
		    gchar *text,
		    gint len,
		    gpointer user_data)
{
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(user_data);
	
	if(self->priv->active_var_pos == NULL)
		return;
	//Repaint the tag
	SnippetVar *var = self->priv->active_var_pos->data;
	GtkTextIter start_iter,end_iter;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,var->start);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_iter,var->end);
	gtk_text_buffer_apply_tag_by_name (buffer, VAR_TAG_NAME, &start_iter, &end_iter);
	
	//Update mirror vars
	if (self->priv->timeout_id==0 && !self->priv->updating)
	{
		self->priv->timeout_id = g_timeout_add(DELAY,update_mirrors_cb,var);
	}
	else
	{
		self->priv->updating = FALSE;
	}
}

static gboolean
mark_in_current_var(GtkSnippetsInPlaceParser *self, GtkTextMark *mark,gint move_offset)
{
	GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->priv->view);
	GtkTextIter location;
	gtk_text_buffer_get_iter_at_mark(buffer,&location,mark);
	gtk_text_iter_forward_chars(&location,move_offset);
	SnippetVar *var = self->priv->active_var_pos->data;
	gint current_offset = gtk_text_iter_get_offset(&location);
	GtkTextIter start_iter,end_iter;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,var->start);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_iter,var->end);
	gint start_offset = gtk_text_iter_get_offset(&start_iter);
	gint end_offset = gtk_text_iter_get_offset(&end_iter);
	/*Test if the cursor is out of the current placeholder*/
	if (current_offset<start_offset || current_offset > end_offset)
		return FALSE;
	return TRUE;
}

gboolean
view_key_press_cb(GtkWidget *view, GdkEventKey *event, gpointer user_data)
{
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(user_data);
	if (event->keyval == GDK_Tab
		|| event->keyval == GDK_ISO_Left_Tab
		|| event->keyval == GDK_Return
		|| event->keyval == GDK_KP_Enter)
	{
		if ((event->state & GDK_SHIFT_MASK) == GDK_SHIFT_MASK)
		{
			if (!active_prev_var(self))
				gtksnippets_inplaceparser_deactivate(self);
			return TRUE;
		}
		else
		{
			if (!active_next_var(self))
				gtksnippets_inplaceparser_deactivate(self);
			return TRUE;
		}
	}
	else if (event->keyval == GDK_Escape)
	{
		gtksnippets_inplaceparser_deactivate(self);
		return TRUE;
	}
	else if (event->keyval == GDK_Home || event->keyval == GDK_KP_Home)
	{
		if (self->priv->active_var_pos!=NULL)
		{
			SnippetVar *var = (SnippetVar*)self->priv->active_var_pos->data;
			GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
			GtkTextIter start_iter;
			gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,var->start);
			self->priv->moving = TRUE;
			if ((event->state & GDK_SHIFT_MASK) == GDK_SHIFT_MASK)
			{
				GtkTextMark *imark = gtk_text_buffer_get_insert(buffer);
				GtkTextIter current_iter;
				gtk_text_buffer_get_iter_at_mark(buffer,&current_iter,imark);
				gtk_text_buffer_select_range(buffer,&start_iter,&current_iter);
			}
			else
			{
				gtk_text_buffer_place_cursor(buffer,&start_iter);
			}
			return TRUE;
		}
	}
	else if (event->keyval == GDK_End || event->keyval == GDK_KP_End)
	{
		if (self->priv->active_var_pos!=NULL)
		{
			SnippetVar *var = (SnippetVar*)self->priv->active_var_pos->data;
			GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
			GtkTextIter iter;
			gtk_text_buffer_get_iter_at_mark(buffer,&iter,var->end);
			self->priv->moving = TRUE;
			if ((event->state & GDK_SHIFT_MASK) == GDK_SHIFT_MASK)
			{
				GtkTextMark *imark = gtk_text_buffer_get_insert(buffer);
				GtkTextIter current_iter;
				gtk_text_buffer_get_iter_at_mark(buffer,&current_iter,imark);
				gtk_text_buffer_select_range(buffer,&iter,&current_iter);
			}
			else
			{
				gtk_text_buffer_place_cursor(buffer,&iter);
			}
			return TRUE;
		}
	}
	else if (event->keyval == GDK_Left)
	{
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
		GtkTextMark *imark = gtk_text_buffer_get_insert(buffer);
		if (!mark_in_current_var(self,imark,-1))
		{
			g_debug("no esta en mark");
			return TRUE;
		}
	}
	else if (event->keyval == GDK_Right)
	{
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
		GtkTextMark *imark = gtk_text_buffer_get_insert(buffer);
		if (!mark_in_current_var(self,imark,1))
		{
			g_debug("no esta en mark");
			return TRUE;
		}
	}
	return FALSE;
}

static void
store_var(GtkSnippetsInPlaceParser *self, SnippetVar *var)
{
	GList *vars = self->priv->vars;
	SnippetVar *parent = NULL, *temp;
	if (vars!=NULL)
	{
		do{
			temp = (SnippetVar*)vars->data;
			if (g_utf8_collate(var->name,temp->name)==0)
			{
				parent = temp;
				break;
			}
		}while(parent == NULL && (vars = g_list_next(vars))!=NULL);
		
		if (parent !=NULL)
			parent->mirrors = g_list_append(parent->mirrors,var);
		else
			self->priv->vars = g_list_append(self->priv->vars,var);
	}
	else
		self->priv->vars = g_list_append(self->priv->vars,var);
	//TODO buscamos una variable en el nivel general con el mismo nombre
}

static void
buffer_mark_set_cb(GtkTextBuffer *buffer,
		GtkTextIter *location,
		GtkTextMark *mark,
		gpointer user_data)
{
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(user_data);
	if (mark == gtk_text_buffer_get_insert (buffer))
	{
		if (self->priv->moving)
			self->priv->moving = FALSE;
		else if (!mark_in_current_var(self,mark,0))
	                gtksnippets_inplaceparser_deactivate(self);
	}
}

gboolean
gtksnippets_inplaceparser_activate(GtkSnippetsInPlaceParser *self, const gchar* content)
{
	gtksnippets_inplaceparser_deactivate(self);
	
	GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->priv->view);
	
	GtkTextMark *insert = gtk_text_buffer_get_insert(buffer);
	if (gsnippets_parser_count_vars(content) <= 0)
	{
		gtk_text_buffer_insert_at_cursor(buffer,content,-1);
		gtk_text_view_scroll_mark_onscreen(self->priv->view,insert);
		return FALSE;
	}
	
	
	GtkTextIter start_iter, end_iter;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,insert);
	GtkTextMark *start_mark = gtk_text_buffer_create_mark(buffer,
							      "snippet_start",
							      &start_iter,
							      TRUE);
	GtkTextMark *end_mark = gtk_text_buffer_create_mark(buffer,
							      "snippet_end",
							      &start_iter,
							      FALSE);

	gtk_text_buffer_insert_at_cursor(buffer,content,-1);
	gtk_text_view_scroll_mark_onscreen(self->priv->view,end_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,start_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_iter,end_mark);
	   		            
	/* Searching variables */
	SnippetVar *var;
	var = search_var(self,buffer,&start_iter,&end_iter);
	while(var!=NULL)
	{
		GtkTextIter start_var, end_var;
		gtk_text_buffer_get_iter_at_mark(buffer,&start_var,var->start);
		gtk_text_buffer_get_iter_at_mark(buffer,&end_var,var->end);
		gtk_text_buffer_apply_tag_by_name (buffer, VAR_TAG_NAME, &start_var, &end_var);
		g_debug("Var found: %s",var->name);
		store_var(self,var);
		var = search_var(self,buffer,&end_var,&end_iter);
	}
	self->priv->active = TRUE;
	active_next_var(self);
	/*The first var is not handled by cursor-changed. We must set moving to FALSE*/
	self->priv->moving = FALSE;
	g_signal_connect(self->priv->view,"key-press-event",G_CALLBACK(view_key_press_cb),self);
	g_signal_connect_after(buffer,"insert-text",G_CALLBACK(view_insert_text_cb),self);
	g_signal_connect_after(buffer,"mark-set",G_CALLBACK(buffer_mark_set_cb),self);
	
	g_signal_emit (G_OBJECT (self), signals[PARSER_START], 0);
	
	return TRUE;
}

gboolean
gtksnippets_inplaceparser_deactivate(GtkSnippetsInPlaceParser *self)
{
	if (!self->priv->active)
		return FALSE;
	
	if (self->priv->timeout_id!=0)
	{
		/*Wait a moment while finish the vars update*/
		g_usleep((DELAY*1000)+100000);
	}
	
	g_debug("Deactivating inplace parser...");
	
	//TODO Desconectar señales
	g_signal_handlers_disconnect_by_func(self->priv->view,view_key_press_cb,self);
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	g_signal_handlers_disconnect_by_func(buffer,view_insert_text_cb,self);
	g_signal_handlers_disconnect_by_func(buffer,buffer_mark_set_cb,self);
	if (self->priv->var_tag!=NULL)
	{
		GtkTextIter start,end;
		gtk_text_buffer_get_start_iter(buffer,&start);
		gtk_text_buffer_get_end_iter(buffer,&end);
		gtk_text_buffer_remove_tag_by_name(buffer,
						VAR_TAG_NAME,
						&start,
						&end);
		/*
		GtkTextTagTable *table = gtk_text_buffer_get_tag_table(buffer);
		gtk_text_tag_table_remove(table,self->priv->var_tag);
		*/
	}
		
	GList *lista = self->priv->vars;
	if (lista!=NULL)
	{
		do{
			snippetvar_free(self,(SnippetVar*)lista->data);
			lista = g_list_next(lista);
		}while(lista!=NULL);
	}
	
	g_list_free(self->priv->vars);
	self->priv->vars = NULL;
	self->priv->active = FALSE;
	self->priv->active_var_pos = NULL;
	self->priv->moving = FALSE;
	
	g_signal_emit (G_OBJECT (self), signals[PARSER_END], 0);
	
	return TRUE;
}


