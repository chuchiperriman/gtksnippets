bin_PROGRAMS = gtksnippets

gladedir = $(datadir)/gtksnippets
icondir = $(datadir)/gtksnippets/pixmaps

INCLUDES=	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DGLADE_DIR=\""$(gladedir)"\" \
	-DICON_DIR=\""$(icondir)"\" \
	-I$(top_srcdir)/gtksnippets	\
	`pkg-config --cflags gtksourceview-2.0`	\
	`pkg-config --cflags libglade-2.0` \
	`pkg-config --cflags gsnippets-1.0` \
	$(PACKAGE_CFLAGS)

AM_CFLAGS = \
	-Wall	

gtksnippets_SOURCES = 		\
	gtksnippets-manager.c	\
	gtksnippets-manager.h	\
	main.c

gtksnippets_LDFLAGS =  \
	`pkg-config --libs libglade-2.0`	\
	`pkg-config --libs gtksourceview-2.0`	\
	`pkg-config --libs gsnippets-1.0`	

gtksnippets_LDADD = 	\
	$(top_srcdir)/gtksnippets/libgtksnippets-1.0.la	\
	$(PACKAGE_LIBS)

 
