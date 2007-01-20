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

#include "gtk-snippets-gsv-utils.h"
 
gchar*
gtk_snippets_gsv_get_last_word(GtkTextView *text_view)
{
	GtkTextMark* insert_mark;
	GtkTextBuffer* text_buffer;
	GtkTextIter actual,ini_word;
	gchar* text;
	
	text_buffer = gtk_text_view_get_buffer(text_view);
	insert_mark = gtk_text_buffer_get_insert(text_buffer);
	gtk_text_buffer_get_iter_at_mark(text_buffer,&actual,insert_mark);
	
	ini_word = actual;
	
	//TODO Habría que comprobar si el caracter anterior es un blanco o una línea,
	//Entonces ponemos un blanco (la llamada a backward_word_start nos coge una palabra de la línea anterior)
	if (!gtk_text_iter_backward_word_start (&ini_word))
		text = "";
	else
	{
		text = gtk_text_iter_get_text (&ini_word, &actual);
		g_strstrip (text);
	}
	
	return text;
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
