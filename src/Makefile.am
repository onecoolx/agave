
SUBDIRS = javascript webcore webview

lib_LTLIBRARIES = libagave.la

libagave_la_SOURCES = template.cpp template.h

libagave_la_LIBADD = \
	javascript/libjscore.la \
	webcore/libwebcore.la \
	webview/libwebview.la 

libagave_la_LDFLAGS= -no-undefined -release $(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_MICRO)
