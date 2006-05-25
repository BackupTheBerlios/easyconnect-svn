bin_PROGRAMS = easyconnect 
easyconnect_SOURCES = main.c config.c functionlist.c devicelist.c generalfunctions.c devicefunctions.c

# set the include path found by configure
INCLUDES = -I$(top_srcdir)/src/tcpcliserver -I$(top_srcdir)/src/iniparser


# the library search path.
easyconnect_LDFLAGS = $(all_libraries) -l pthread -l dl

SUBDIRS = tcpcliserver iniparser 
easyconnect_LDADD = $(top_builddir)/src/tcpcliserver/libtcpcliserver.a $(top_builddir)/src/tcpcliserver/cl/libcl.a $(top_builddir)/src/iniparser/libiniparser.a
