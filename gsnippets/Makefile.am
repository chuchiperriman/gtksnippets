## @start 1
## Process this file with automake to produce Makefile.in

INCLUDES=	\
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	$(GTKSNIPPETS_CFLAGS)
	
AM_CFLAGS =\
	 -Wall\
	 -g

lib_LTLIBRARIES=libgsnippets-1.0.la

libgsnippets_headers = 	\
	gsnippets-common.h		\
	gsnippets-item.h	\
	gsnippets-db.h		\
	gsnippets-parser.h
	
libgsnippets_1_0_la_SOURCES	= \
	gsnippets-common.c		\
	gsnippets-item.c	\
	gsnippets-db.c		\
	gsnippets-parser.c		\
	$(libgsnippets_headers)

libgsnippets_1_0_la_LIBADD	= \
	$(PACKAGE_CFLAGS)

libgsnippets_1_0_la_LDFLAGS =	\
	-version-info 1:0:0 -module	\
	$(GTKSNIPPETS_LIBS)	
	
libgsnippets_1_0_includedir = $(includedir)/gsnippets-1.0/gsnippets

libgsnippets_1_0_include_HEADERS =          \
	$(libgsnippets_headers)             
	
## @end 1
