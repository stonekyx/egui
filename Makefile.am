maintainer-clean-local:
	-rm -vf library/dep.png library/dep.dot
	-rm -rvf autom4te.cache
	-rm -vf client.cflow server.cflow

pkglib_LTLIBRARIES=

nobase_pkginclude_HEADERS=

bin_PROGRAMS=

noinst_PROGRAMS=

noinst_HEADERS=

client_cflow_SRC=

server_cflow_SRC=

ACLOCAL_AMFLAGS= -I m4

AM_CPPFLAGS=@AM_CPPFLAGS@ -DPACKAGE_DATA_PATH="\"$(resourcedir)\"" \
			-DPACKAGE_SRC_PATH="\"$(abs_top_srcdir)\""

include $(top_srcdir)/resource/local.mk
include $(top_srcdir)/include/local.mk
include $(top_srcdir)/utils/local.mk
include $(top_srcdir)/comm/local.mk
include $(top_srcdir)/event/local.mk
include $(top_srcdir)/graph/local.mk
include $(top_srcdir)/widget/local.mk
include $(top_srcdir)/application/local.mk
include $(top_srcdir)/debug/local.mk
include $(top_srcdir)/window_manager/local.mk

cflow:
	-cflow -o client.cflow --omit-arguments -i s -T -d 3 $(addprefix $(top_srcdir)/, $(client_cflow_SRC))
	-cflow -o server.cflow --omit-arguments -i s -T -d 5 $(addprefix $(top_srcdir)/, $(server_cflow_SRC))
