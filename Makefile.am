maintainer-clean-local:
	-rm -vf library/dep.png library/dep.dot
	-rm -rvf autom4te.cache

pkglib_LTLIBRARIES=

nobase_pkginclude_HEADERS=

bin_PROGRAMS=

noinst_PROGRAMS=

noinst_HEADERS=

nobase_pkgdata_DATA=

ACLOCAL_AMFLAGS= -I m4

AM_CPPFLAGS=@AM_CPPFLAGS@ -DPACKAGE_DATA_PATH="\"$(pkgdatadir)\""

include $(top_srcdir)/resource/local.mk
include $(top_srcdir)/include/local.mk
include $(top_srcdir)/utils/local.mk
include $(top_srcdir)/comm/local.mk
include $(top_srcdir)/event/local.mk
include $(top_srcdir)/client_lib/local.mk
include $(top_srcdir)/graph/local.mk
include $(top_srcdir)/server_lib/local.mk
include $(top_srcdir)/widget/local.mk
include $(top_srcdir)/application/local.mk
include $(top_srcdir)/debug/local.mk
include $(top_srcdir)/window_manager/local.mk
