noinst_LTLIBRARIES = libgatebluetooth.la

AM_CFLAGS = $(WARNING_CFLAGS) $(BUILD_CFLAGS)

libgatebluetooth_la_SOURCES = adapter.c bluez_client.c $(ell_sources)
libgatebluetooth_la_DEPENDENCIES = adapter.h bluez_client.h
libgatebluetooth_la_CFLAGS = @ELL_CFLAGS@ -I$(top_srcdir)/src -I$(top_srcdir)/src/bluetooth

clean-local:
	$(RM) -rf $(noinst_LTLIBRARIES)
all-local:
	$(MKDIR_P) $(top_srcdir)/libs && cp .libs/* $(top_srcdir)/libs
