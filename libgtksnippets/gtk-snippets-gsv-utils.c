/***************************************************************************
 *            gtk-snippet.c
 *
 *  mi� ene 10 10:15:00 2007
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

#include "gtk-snippets-gsv-utils.h"

/**
* gtk_snippets_gsv_get_last_word_and_iter:
* @text_view: editor
* @start_iter: if != NULL then assign it the start position of the word
* Returns: the last word founded or ""
*
**/
gchar*
gtk_snippets_gsv_get_last_word_and_iter(GtkTextView *text_view, GtkTextIter *start_word, GtkTextIter *end_word)
{
	GtkTextMark* insert_mark;
	GtkTextBuffer* text_buffer;
	GtkTextIter actual,temp;
	GtkTextIter *start_iter;
	gchar* text;
	gunichar ch;
	gboolean found, no_doc_start;
	
	if (start_word != NULL)
	{
		start_iter = start_word;
	}
	else
	{
		start_iter = &temp;
	}
	
	text_buffer = gtk_text_view_get_buffer(text_view);
	insert_mark = gtk_text_buffer_get_insert(text_buffer);
	gtk_text_buffer_get_iter_at_mark(text_buffer,&actual,insert_mark);
	
	*start_iter = actual;
	if (end_word!=NULL)
	{
		*end_word = actual;
	}
	
	found = FALSE;
	while ((no_doc_start = gtk_text_iter_backward_char(start_iter)) == TRUE)
	{
		ch = gtk_text_iter_get_char(start_iter);
		if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')
		{
			found = TRUE;
			break;
		}
	}
	
	//Si es el principio del doc, cogemos la palabra hasta el principio
	if (!no_doc_start)
	{
		gtk_text_buffer_get_start_iter(text_buffer,start_iter);
		text = gtk_text_iter_get_text (start_iter, &actual);
	}
	else
	{
	
		if (found)
		{
			gtk_text_iter_forward_char(start_iter);
			text = gtk_text_iter_get_text (start_iter, &actual);
		}
		else
		{
			*start_iter = actual;
			text = "";
		}
	}
	
	return text;
}
 
gchar*
gtk_snippets_gsv_get_last_word(GtkTextView *text_view)
{
	return gtk_snippets_gsv_get_last_word_and_iter(text_view, NULL, NULL);
}


void
gtk_snippets_gsv_get_screen_pos(GtkTextView *text_view, gint *x, gint *y)
{
	GdkWindow *win;
	GtkTextMark* insert_mark;
	GtkTextBuffer* text_buffer;
	GtkTextIter start;
	GdkRectangle location;
	gint win_x, win_y;
	gint xx, yy;

	text_buffer = gtk_text_view_get_buffer(text_view);
	insert_mark = gtk_text_buffer_get_insert(text_buffer);
	gtk_text_buffer_get_iter_at_mark(text_buffer,&start,insert_mark);
	gtk_text_view_get_iter_location(text_view,
														&start,
														&location );
	gtk_text_view_buffer_to_window_coords (text_view,
                                        GTK_TEXT_WINDOW_WIDGET,
                                        location.x, location.y,
                                        &win_x, &win_y);

	win = gtk_text_view_get_window (text_view, 
                                GTK_TEXT_WINDOW_WIDGET);
	gdk_window_get_origin (win, &xx, &yy);
	
	*x = win_x + xx;
	*y = win_y + yy + location.height;
}

gchar*
gtk_snippets_gsv_get_text(GtkTextView *text_view)
{
	GtkTextIter start, end;
	GtkTextBuffer *buffer;
	
	buffer = gtk_text_view_get_buffer(text_view);
	gtk_text_buffer_get_start_iter (buffer, &start);
	gtk_text_buffer_get_end_iter (buffer, &end);
	return gtk_text_buffer_get_text(buffer,&start,&end,FALSE);
	
}

void
gtk_snippets_gsv_replace_actual_word(GtkTextView *text_view, const gchar* text)
{
	GtkTextBuffer *buffer;
	GtkTextIter word_start, word_end;
	
	//Hay que borrar la palabra escrita para el snippet
	buffer = gtk_text_view_get_buffer(text_view);
	gtk_text_buffer_begin_user_action(buffer);
	
	gtk_snippets_gsv_get_last_word_and_iter(text_view,&word_start, &word_end);
									   
	gtk_text_buffer_delete(buffer,&word_start,&word_end);
	gtk_text_buffer_insert(buffer, &word_start, text,-1);
	
	gtk_text_buffer_end_user_action(buffer);
}

