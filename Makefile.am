
ACLOCAL_AMFLAGS = -I m4

AM_MAKEFLAGS = --no-print-directory

src_iwpand_LDADD = @ELL_LIBS@ $(ell_sources)

src_iwpand_CFLAGS = @ELL_CFLAGS@

SUBDIRS = src/bluetooth

bin_PROGRAMS = src/gatectl

src_gatectl_SOURCES = src/main.c \
		      src/dbus.h src/dbus.c \
		      src/option.h src/option.c \
		      src/manager.h src/manager.c

src_gatectl_LDADD = ell/libell-internal.la libs/libgatebluetooth.a -ldl

AM_CFLAGS = -fvisibility=hidden

MAINTAINERCLEANFILES = Makefile.in configure config.h.in aclocal.m4

DISTCLEANFILES =
