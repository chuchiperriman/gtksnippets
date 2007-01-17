## Process this file with automake to produce Makefile.in

## Created by Anjuta

gladedir = $(datadir)/gtksnippets/glade
glade_DATA = gtksnippets.glade

INCLUDES = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DGLADE_DIR=\""$(gladedir)"\" \
	`pkg-config --cflags gtksourceview-1.0` \
	`pkg-config --cflags libglade-2.0` \
	$(PACKAGE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gtksnippets

gtksnippets_SOURCES = \
	callbacks.c \
	callbacks.h \
	main.c

gtksnippets_LDFLAGS =  `pkg-config --libs libglade-2.0`

gtksnippets_LDADD = ../libgtksnippets/libgtksnippets.la $(PACKAGE_LIBS) 

EXTRA_DIST = $(glade_DATA)
