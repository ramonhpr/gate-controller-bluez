
ACLOCAL_AMFLAGS = -I m4

AM_MAKEFLAGS = --no-print-directory


bin_PROGRAMS = src/gatectl

src_gatectl_SOURCES = src/main.c \
		      src/dbus.h src/dbus.c \
		      src/bluez_client.h src/bluez_client.c \
		      src/option.h src/option.c \
		      src/manager.c

src_gatectl_LDADD = @ELL_LIBS@ @BLUEZ_LIBS@ -ldl -lbluetooth

src_iwpand_CFLAGS = @ELL_CFLAGS@ @BLUEZ_CFLAGS@

AM_CFLAGS = -fvisibility=hidden

MAINTAINERCLEANFILES = Makefile.in configure config.h.in aclocal.m4

DISTCLEANFILES =
