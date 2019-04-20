imagedatadir = $(datadir)/$(PACKAGE)/images
imagedata_DATA = scratch-hack.png
EXTRA_DIST = $(imagedata_DATA)

bin_PROGRAMS=s3splash

s3splash_SOURCES=main.cpp MainWindow.cpp
s3splash_CPPFLAGS=$(GTKMM_CFLAGS) -DIMAGE_DATA_DIR=\"$(imagedatadir)\" $(CURL_CFLAGS) $(JSONCPP_CFLAGS)
s3splash_LDADD=$(GTKMM_LIBS) -lboost_system -lpthread -lboost_thread $(CURL_LIBS) $(JSONCPP_LIBS)