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
#include <string.h>
#include "gtksnippets-inplaceparser.h"
#include "gtksnippets-gtv-var.h"
#include "../gsnippets/gsnippets-parser.h"


#define DELAY 300
#define SNIPPET_START_MARK "snippet_start"
#define SNIPPET_END_MARK "snippet_end"

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
	gboolean updating;
	gboolean moving;
	GtkTextMark *end_position_mark;
};
static GObjectClass* parent_class = NULL;

#define GTKSNIPPETS_INPLACEPARSER_GET_PRIVATE(object)(G_TYPE_INSTANCE_GET_PRIVATE ((object), GTKSNIPPETS_TYPE_INPLACEPARSER, GtkSnippetsInPlaceParserPrivate))

#define VAR_TAG_NAME "snippet_var"

/*###############indentation management############*/
static gchar *
_compute_line_indentation (GtkTextView *view,
		     GtkTextIter *cur)
{
	GtkTextIter start;
	GtkTextIter end;

	gunichar ch;
	gint line;

	line = gtk_text_iter_get_line (cur);

	gtk_text_buffer_get_iter_at_line (gtk_text_view_get_buffer (view),
					  &start,
					  line);

	end = start;

	ch = gtk_text_iter_get_char (&end);

	while (g_unichar_isspace (ch) &&
	       (ch != '\n') &&
	       (ch != '\r') &&
	       (gtk_text_iter_compare (&end, cur) < 0))
	{
		if (!gtk_text_iter_forward_char (&end))
			break;

		ch = gtk_text_iter_get_char (&end);
	}

	if (gtk_text_iter_equal (&start, &end))
		return NULL;

	return gtk_text_iter_get_slice (&start, &end);
}

static gchar*
_get_text_with_indent(const gchar* content,gchar *indent)
{
	if (indent==NULL)
		return g_strdup(content);
	GString *fin = NULL;
	gchar *ret = NULL;
	gint len = strlen(content);
	gint i;
	gint last_line = 0;
	for (i=0;i < len;i++)
	{
		if (content[i] == '\n' || content[i] =='\r')
		{
			if (fin==NULL)
				fin = g_string_new_len(content,i+1);
			else
			{
				fin = g_string_append_len(fin,&content[last_line+1],i - last_line);
			}
			fin = g_string_append(fin,indent);
			last_line = i;
		}
	}
	if (fin==NULL)
		ret = g_strdup(content);
	else
	{
		if (last_line < len -1)
		{
			fin = g_string_append_len(fin,&content[last_line+1],len - last_line);
		}
		ret = g_string_free(fin,FALSE);
	}
	return ret;
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
	self->priv->moving = FALSE;
	self->priv->end_position_mark = NULL;
	self->priv->updating = FALSE;
}

static void
gtksnippets_inplaceparser_finalize (GObject *object)
{
	g_debug("GtkSnippetsInPlaceParser finish");
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(object);
	
	gtksnippets_inplaceparser_deactivate(self);
	self->priv->active_var_pos = NULL;
	
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

static GtkSnippetsGtvVar*
search_var(GtkSnippetsInPlaceParser *self, GtkTextBuffer *buffer,GtkTextMark *init_mark, GtkTextMark *limit_mark)
{
	GtkSnippetsGtvVar *var = NULL;
	GtkTextMark *start_mark, *end_mark, *temp_mark;
	gchar *definition;
	const gchar *default_value;
	GtkTextIter start, end, *temp_iter;
	GtkTextIter pos, limit;
	gtk_text_buffer_get_iter_at_mark(buffer,&pos, init_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&limit, limit_mark);
	gboolean found = gtk_text_iter_forward_search(&pos,
				"${",
				GTK_TEXT_SEARCH_VISIBLE_ONLY,
				&start,
				NULL,
				&limit);
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
			start_mark = gtk_text_buffer_create_mark(buffer,
								NULL,
								&start,
								TRUE);
			end_mark = gtk_text_buffer_create_mark(buffer,
								NULL,
								&end,
								FALSE);
			gtk_text_iter_forward_chars(&start,2);
			gtk_text_iter_forward_chars(&end,-1);
			definition = gtk_text_buffer_get_text(buffer,
							&start,
							&end,
							FALSE);
			
			var = gtksnippets_gtv_var_new(definition,
							self->priv->view,
							start_mark,
							end_mark);
			g_free(definition);
			
			default_value = gtksnippets_variable_get_default_value(GTKSNIPPETS_VARIABLE(var));
			if (default_value==NULL)
				default_value = gtksnippets_variable_get_name(GTKSNIPPETS_VARIABLE(var));
				
			gtksnippets_gtv_var_set_text_with_tags_by_name(var,
									default_value,
									VAR_TAG_NAME);
		}
		else
		{
			/*
			If there is ${ but not } in the line, we must to search
			a new variable because if we return NULL, it is the end
			of the search
			*/
			gtk_text_iter_forward_chars(&start,2);
			temp_mark = gtk_text_buffer_create_mark(buffer,
								NULL,
								&start,
								TRUE);
			gtk_text_buffer_move_mark(buffer,temp_mark,&start);
			var = search_var(self,buffer,temp_mark,limit_mark);
			gtk_text_buffer_delete_mark(buffer,temp_mark);
		}
	}
	
	return var;
}

static void
set_active_var(GtkSnippetsInPlaceParser *self)
{
	self->priv->moving = TRUE;
	GtkSnippetsGtvVar *var = GTKSNIPPETS_GTV_VAR(self->priv->active_var_pos->data);
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	GtkTextMark *start_mark, *end_mark;
	GtkTextIter start,end;
	start_mark = gtksnippets_gtv_var_get_start_mark(var);
	end_mark = gtksnippets_gtv_var_get_end_mark(var);
	gtk_text_buffer_get_iter_at_mark(buffer,&start,start_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&end,end_mark);
	gtk_text_buffer_select_range(buffer,&start,&end);
	gtk_text_view_scroll_mark_onscreen(self->priv->view,start_mark);
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

static void
update_mirrors(GtkSnippetsInPlaceParser *self, GtkSnippetsGtvVar *actual_var)
{
	if (self->priv->updating)
		return;
	
	self->priv->updating = TRUE;
	
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	
	if (self->priv->active_var_pos==NULL)
	{
		self->priv->updating = FALSE;
		return;
	}
	gtk_text_buffer_begin_user_action(buffer);
	
	gchar* text = gtksnippets_gtv_var_get_text(actual_var);
	
	gtksnippets_gtv_var_set_text_with_tags_by_name(actual_var,
							text,
							VAR_TAG_NAME);
	g_free(text);
	gtk_text_buffer_end_user_action(buffer);
	self->priv->updating = FALSE;
}

void
view_insert_text_cb(GtkTextBuffer *buffer, 
		    GtkTextIter *location, 
		    gchar *text,
		    gint len,
		    gpointer user_data)
{
	GtkTextMark *start_mark, *end_mark;
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(user_data);
	
	if(self->priv->active_var_pos == NULL)
		return;
	/*Repaint the tag*/
	GtkSnippetsGtvVar *var = GTKSNIPPETS_GTV_VAR(self->priv->active_var_pos->data);
	start_mark = gtksnippets_gtv_var_get_start_mark(var);
	end_mark = gtksnippets_gtv_var_get_end_mark(var);
	GtkTextIter start_iter,end_iter;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,start_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_iter,end_mark);
	gtk_text_buffer_apply_tag_by_name (buffer, VAR_TAG_NAME, &start_iter, &end_iter);
	
	update_mirrors(self, var);
}

static void
buffer_delete_range_cb(GtkTextBuffer *textbuffer,
			GtkTextIter   *start,
			GtkTextIter   *end,
			gpointer       user_data)
{
	GtkSnippetsInPlaceParser *self = GTKSNIPPETS_INPLACEPARSER(user_data);
	
	if(self->priv->active_var_pos == NULL)
		return;
	
	GtkSnippetsGtvVar *var = GTKSNIPPETS_GTV_VAR(self->priv->active_var_pos->data);
	update_mirrors(self, var);
}

static gboolean
mark_in_current_var(GtkSnippetsInPlaceParser *self, GtkTextMark *mark,gint move_offset)
{
	GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->priv->view);
	GtkTextIter location;
	GtkTextIter start_iter,end_iter;
	GtkTextMark *start_mark, *end_mark;
	gtk_text_buffer_get_iter_at_mark(buffer,&location,mark);
	gtk_text_iter_forward_chars(&location,move_offset);
	GtkSnippetsGtvVar *var = GTKSNIPPETS_GTV_VAR(self->priv->active_var_pos->data);
	start_mark = gtksnippets_gtv_var_get_start_mark(var);
	end_mark = gtksnippets_gtv_var_get_end_mark(var);
	gint current_offset = gtk_text_iter_get_offset(&location);
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,start_mark);
	gtk_text_buffer_get_iter_at_mark(buffer,&end_iter,end_mark);
	gint start_offset = gtk_text_iter_get_offset(&start_iter);
	gint end_offset = gtk_text_iter_get_offset(&end_iter);
	/*Test if the cursor is out of the current placeholder*/
	if (current_offset<start_offset || current_offset > end_offset)
		return FALSE;
	return TRUE;
}

static void
_end_var_replacement(GtkSnippetsInPlaceParser *self)
{
	GtkTextIter iter;
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
	GtkTextMark *mark = NULL;
	if (self->priv->end_position_mark!=NULL)
		mark = self->priv->end_position_mark;
	else
		mark = gtk_text_buffer_get_mark(buffer,SNIPPET_END_MARK);

	gtk_text_buffer_get_iter_at_mark(buffer,&iter,mark);
	gtk_text_view_scroll_mark_onscreen(self->priv->view,mark);
	gtksnippets_inplaceparser_deactivate(self);
	gtk_text_buffer_place_cursor(buffer,&iter);
}

static gboolean
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
			{
				_end_var_replacement(self);
			}
			return TRUE;
		}
		else
		{
			if (!active_next_var(self))
			{
				_end_var_replacement(self);
			}
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
			GtkSnippetsGtvVar *var = GTKSNIPPETS_GTV_VAR(self->priv->active_var_pos->data);
			GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
			GtkTextIter start_iter;
			GtkTextMark *start_mark;
			start_mark = gtksnippets_gtv_var_get_start_mark(var);
			gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,start_mark);
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
			GtkSnippetsGtvVar *var = GTKSNIPPETS_GTV_VAR(self->priv->active_var_pos->data);
			GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
			GtkTextIter iter;
			GtkTextMark *end_mark;
			end_mark = gtksnippets_gtv_var_get_end_mark(var);
			gtk_text_buffer_get_iter_at_mark(buffer,&iter,end_mark);
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
			return TRUE;
	}
	else if (event->keyval == GDK_Right)
	{
		GtkTextBuffer *buffer = gtk_text_view_get_buffer(self->priv->view);
		GtkTextMark *imark = gtk_text_buffer_get_insert(buffer);
		if (!mark_in_current_var(self,imark,1))
			return TRUE;
	}
	return FALSE;
}

static void
store_var(GtkSnippetsInPlaceParser *self, GtkSnippetsGtvVar *var)
{
	GList *vars = self->priv->vars;
	GtkSnippetsGtvVar *parent = NULL, *temp;
	const gchar *name, *temp_name;
	if (vars!=NULL)
	{
		do{
			temp = GTKSNIPPETS_GTV_VAR(vars->data);
			name = gtksnippets_variable_get_name(GTKSNIPPETS_VARIABLE(var));
			temp_name = gtksnippets_variable_get_name(GTKSNIPPETS_VARIABLE(temp));
			if (g_utf8_collate(name,temp_name)==0)
			{
				parent = temp;
				break;
			}
		}while(parent == NULL && (vars = g_list_next(vars))!=NULL);
		
		if (parent !=NULL)
		{
			gtksnippets_gtv_var_add_mirror(parent,var);
			g_object_unref(var);
		}
		else
		{
			self->priv->vars = g_list_append(self->priv->vars,var);
		}
	}
	else
		self->priv->vars = g_list_append(self->priv->vars,var);
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

static gint
sort_variables(gconstpointer a,
		gconstpointer b)
{
	GtkSnippetsVariable *vara, *varb;
	vara = GTKSNIPPETS_VARIABLE(a);
	varb = GTKSNIPPETS_VARIABLE(b);
	return gtksnippets_variable_get_index(vara) - gtksnippets_variable_get_index(varb);
}

gboolean
gtksnippets_inplaceparser_activate(GtkSnippetsInPlaceParser *self, const gchar* content)
{
	g_debug("inplace activate...");
	gtksnippets_inplaceparser_deactivate(self);
	
	GtkTextBuffer * buffer = gtk_text_view_get_buffer(self->priv->view);
	
	GtkTextMark *insert = gtk_text_buffer_get_insert(buffer);
	if (gsnippets_parser_count_vars(content) <= 0)
	{
		GtkTextIter cur;
		gtk_text_buffer_get_iter_at_mark(buffer,&cur,insert);
		gchar *indent = _compute_line_indentation(self->priv->view,&cur);
		gchar *indent_text = _get_text_with_indent(content, indent);
		g_free(indent);
		gtk_text_buffer_insert_at_cursor(buffer,indent_text,-1);
		g_free(indent_text);
		gtk_text_view_scroll_mark_onscreen(self->priv->view,insert);
		return FALSE;
	}
	
	GtkTextIter start_iter;
	gtk_text_buffer_get_iter_at_mark(buffer,&start_iter,insert);
	GtkTextMark *start_mark = gtk_text_buffer_create_mark(buffer,
							      SNIPPET_START_MARK,
							      &start_iter,
							      TRUE);
	GtkTextMark *end_text_mark = gtk_text_buffer_create_mark(buffer,
							      SNIPPET_END_MARK,
							      &start_iter,
							      FALSE);
	GtkTextMark *end_mark;
	
	gchar *indent = _compute_line_indentation(self->priv->view,&start_iter);
	gchar *indent_text = _get_text_with_indent(content, indent);
	g_free(indent);
	gtk_text_buffer_insert_at_cursor(buffer,indent_text,-1);
	g_free(indent_text);
	gtk_text_view_scroll_mark_onscreen(self->priv->view,end_text_mark);

	/* Searching variables */
	GtkSnippetsGtvVar *var;
	var = search_var(self,buffer,start_mark,end_text_mark);
	while(var!=NULL)
	{
		end_mark = gtksnippets_gtv_var_get_end_mark(var);
		store_var(self,var);
		var = search_var(self,buffer,end_mark,end_text_mark);
	}
	
	/*Sorting varables by index*/
	self->priv->vars = g_list_sort(self->priv->vars,
					sort_variables);
	
	self->priv->active = TRUE;
	active_next_var(self);
	/*The first var is not handled by cursor-changed. We must set moving to FALSE*/
	self->priv->moving = FALSE;
	g_signal_connect(self->priv->view,"key-press-event",G_CALLBACK(view_key_press_cb),self);
	g_signal_connect_after(buffer,"insert-text",G_CALLBACK(view_insert_text_cb),self);
	g_signal_connect_after(buffer,"delete-range",G_CALLBACK(buffer_delete_range_cb),self);
	g_signal_connect_after(buffer,"mark-set",G_CALLBACK(buffer_mark_set_cb),self);
	g_signal_emit (G_OBJECT (self), signals[PARSER_START], 0);
	
	return TRUE;
}

gboolean
gtksnippets_inplaceparser_deactivate(GtkSnippetsInPlaceParser *self)
{
	if (!self->priv->active)
		return FALSE;
	
	g_debug("Deactivating inplace parser...");
	
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
			g_object_unref(GTKSNIPPETS_GTV_VAR(lista->data));
			lista = g_list_next(lista);
		}while(lista!=NULL);
	}
	g_list_free(self->priv->vars);
	
	gtk_text_buffer_delete_mark_by_name(buffer,SNIPPET_START_MARK);
	gtk_text_buffer_delete_mark_by_name(buffer,SNIPPET_END_MARK);
	
	self->priv->vars = NULL;
	self->priv->active = FALSE;
	self->priv->active_var_pos = NULL;
	self->priv->moving = FALSE;
	if (self->priv->end_position_mark!=NULL)
	{
		gtk_text_buffer_delete_mark(buffer,self->priv->end_position_mark);
		self->priv->end_position_mark = NULL;
	}
	
	g_signal_emit (G_OBJECT (self), signals[PARSER_END], 0);
	
	return TRUE;
}

