
gladedir = $(datadir)/libgtksnippets/glade
glade_DATA = *.glade

snippetsdir = $(datadir)/libgtksnippets/snippets
snippets_DATA = snippets/*.xml

icondir = $(datadir)/libgtksnippets/pixmaps
icon_DATA = pixmaps/*.png

INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DGLADE_DIR=\""$(gladedir)"\" \
	-DSNIPPETS_DIR=\""$(snippetsdir)"\" \
	-DICON_DIR=\""$(icondir)"\" \
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
	gtk-snippets-manager-gsv.h      \
	gtk-snippets-manager-gsv.c      \
	gtk-snippets-popup-dialog.h      \
	gtk-snippets-popup-dialog.c	\
	gtk-snippets-gsv-utils.h	\
	gtk-snippets-gsv-utils.c	\
	gtk-snippets-management-ui.h	\
	gtk-snippets-management-ui.c    \
	gtk-snippets-editor.c    \
	gtk-snippets-editor.h    \
	gtk-snippets-manager.c    \
	gtk-snippets-manager.h    \
	gtk-snippets-editor-gsv.c    \
	gtk-snippets-editor-gsv.h    \
	gtk-text-completion-data.c	\
	gtk-text-completion-data.h	\
	gtk-text-completion-popup.h    \
	gtk-text-completion-popup.c 	\
	gtk-text-completion-provider.c	\
	gtk-text-completion-provider.h	\
	gtc-provider-test.c	\
	gtc-provider-test.h	


libgtksnippets_la_LIBADD = \
	$(PACKAGE_LIBS) 
	
libgtksnippets_la_LDFLAGS = \
	-version-info 1:0:0 -module	\
	`pkg-config --libs gtksourceview-1.0`

INST_H_FILES = \
	gtk-snippet.h \
	gtk-snippets-gsv-utils.h  \
	gtk-snippets-loader.h  \
	gtk-snippets-management-ui.h  \
	gtk-snippets-manager-gsv.h  \
	gtk-snippets-editor.h    \
	gtk-snippets-manager.h    \
	gtk-snippets-editor-gsv.h \
	gtk-snippets-popup-dialog.h	\
	gtk-text-completion-provider.h	\
	gtc-provider-test.h	\
	gtk-text-completion-data.h

headerdir = $(prefix)/include/libgtksnippets-1

header_DATA =                           \
        $(INST_H_FILES) 

EXTRA_DIST = $(glade_DATA) $(snippets_DATA) $(icon_DATA)
	
## File created by the gnome-build tools






















