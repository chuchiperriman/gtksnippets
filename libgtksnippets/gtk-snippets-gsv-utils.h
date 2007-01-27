/***************************************************************************
 *            gtk-snippets-gsv-utils.h
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

#ifndef GTK_SNIPPETS_GSV_UTILS_H
#define GTK_SNIPPETS_GSV_UTILS_H

#include <gtk/gtk.h>

gchar*
gtk_snippets_gsv_get_last_word_and_iter(GtkTextView *text_view, GtkTextIter *start_word, GtkTextIter *end_word);

gchar*
gtk_snippets_gsv_get_last_word(GtkTextView *text_view);

void
gtk_snippets_gsv_get_screen_pos(GtkTextView *text_view, gint *x, gint *y);

#endif /* GTK_SNIPPETS_GSV_UTILS_H */
