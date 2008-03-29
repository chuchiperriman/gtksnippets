bin_PROGRAMS = gtksnippets-manager

gladedir = $(datadir)/gtksnippets
icondir = $(datadir)/gtksnippets/pixmaps

INCLUDES=	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DGLADE_DIR=\""$(gladedir)"\" \
	-DICON_DIR=\""$(icondir)"\" \
	-I$(top_srcdir)/gsnippets	\
	-I$(top_srcdir)/gtksnippets	\
	`pkg-config --cflags gtksourceview-2.0`	\
	`pkg-config --cflags libglade-2.0` \
	$(PACKAGE_CFLAGS)

AM_CFLAGS = \
	-Wall	

gtksnippets_manager_SOURCES = 		\
	gtksnippets-manager.c	\
	gtksnippets-manager.h	\
	main.c

gtksnippets_manager_LDFLAGS =  \
	`pkg-config --libs libglade-2.0`	\
	`pkg-config --libs gtksourceview-2.0`	

gtksnippets_manager_LDADD = 	\
	$(top_builddir)/gsnippets/libgsnippets-1.0.la	\
	$(top_builddir)/gtksnippets/libgtksnippets-1.0.la	\
	$(PACKAGE_LIBS)

 
