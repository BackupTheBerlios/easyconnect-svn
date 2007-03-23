bin_PROGRAMS = easyconnect 
easyconnect_SOURCES = src/main.c src/config.c src/functionlist.c src/modulelist.c src/cmodule.c src/pythonmodule.c


# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src/tcpcliserver -I$(top_srcdir)/src/iniparser

AM_CFLAGS = -g
# the library search path.
easyconnect_LDFLAGS = $(all_libraries) 

SUBDIRS = src 
easyconnect_LDADD = $(top_builddir)/src/tcpcliserver/libtcpcliserver.a $(top_builddir)/src/tcpcliserver/cl/libcl.a $(top_builddir)/src/iniparser/libiniparser.a
