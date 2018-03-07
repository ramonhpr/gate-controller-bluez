
ACLOCAL_AMFLAGS = -I m4

AM_MAKEFLAGS = --no-print-directory

SUBDIRS = src/bluetooth
bin_PROGRAMS = src/gatectl

src_gatectl_SOURCES = src/main.c \
		      src/dbus.h src/dbus.c \
		      src/option.h src/option.c \
		      src/manager.h src/manager.c

src_gatectl_CFLAGS = @ELL_CFLAGS@ -I$(top_srcdir)/src -I$(top_srcdir)/src/bluetooth
src_gatectl_LDADD = @ELL_LIBS@ libs/libgatebluetooth.a -ldl

AM_CFLAGS = -fvisibility=hidden

MAINTAINERCLEANFILES = Makefile.in configure config.h.in aclocal.m4

DISTCLEANFILES =
