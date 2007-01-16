
INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	$(PACKAGE_CFLAGS) \
	`pkg-config --cflags gtksourceview-1.0` \
	`pkg-config --cflags libglade-2.0`

AM_CFLAGS =\
	 -Wall\
	 -g
	 
lib_LTLIBRARIES = \
	libgtksnippets.la

libgtksnippets_la_SOURCES = \
	gtk-snippets-loader.c     \
	gtk-snippets-loader.h     \
	gtk-snippet.c     \
	gtk-snippet.h 		\
	gtk-snippet-source-view-listener.h	\
	gtk-snippet-source-view-listener.c

libgtksnippets_la_LIBADD = \
	$(PACKAGE_LIBS) 
	
libgtksnippets_la_LDFLAGS = \
	-version-info 1:0:0 -module	\
	`pkg-config --libs gtksourceview-1.0`
	
## File created by the gnome-build tools


