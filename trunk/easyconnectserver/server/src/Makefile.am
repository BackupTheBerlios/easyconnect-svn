bin_PROGRAMS = easyconnect 
easyconnect_SOURCES = main.c config.c functionlist.c modulelist.c cmodule.c pythonmodule.c

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src/tcpcliserver -I$(top_srcdir)/src/iniparser

CFLAGS = -g -p
# the library search path.
easyconnect_LDFLAGS = $(all_libraries) -l pthread -l dl -l python2.4

SUBDIRS = tcpcliserver iniparser 
easyconnect_LDADD = $(top_builddir)/src/tcpcliserver/libtcpcliserver.a $(top_builddir)/src/tcpcliserver/cl/libcl.a $(top_builddir)/src/iniparser/libiniparser.a
