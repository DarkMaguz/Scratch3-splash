bin_PROGRAMS=s3splash

s3splash_SOURCES=main.cpp MainWindow.cpp TcpClient.cpp
s3splash_CPPFLAGS=$(GTKMM_CFLAGS)
#$(CURL_CFLAGS)
s3splash_LDADD=$(GTKMM_LIBS) -lboost_system -lpthread -lboost_thread
#$(CURL_LIBS) 
