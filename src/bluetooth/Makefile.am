noinst_LTLIBRARIES = libgatebluetooth.la

AM_CFLAGS = $(WARNING_CFLAGS) $(BUILD_CFLAGS)

libgatebluetooth_la_SOURCES = adapter.c bluez_client.c $(ell_sources)
libgatebluetooth_la_DEPENDENCIES = adapter.h bluez_client.h
libgatebluetooth_la_LIBADD = $(abs_srcdir)/ell/libell-internal.la

clean-local:
	$(RM) -rf $(noinst_LTLIBRARIES)
all-local:
	$(MKDIR_P) $(top_srcdir)/libs && cp .libs/* $(top_srcdir)/libs
