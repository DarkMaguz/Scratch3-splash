/*
 * MainWindow.h
 *
 *  Created on: Mar 1, 2019
 *      Author: peter
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <gtkmm-3.0/gtkmm.h>
#include <gtkmm-3.0/gtkmm/image.h>


#include <boost/thread.hpp>

//#include <curl/curl.h>
#include "TcpClient.h"

class MainWindow;

void thread(MainWindow *mw);

class MainWindow : public Gtk::Window {
public:
	MainWindow();
	virtual ~MainWindow();
	
	/*typedef sigc::signal<void, bool, int> siganelConnected_t;
	siganelConnected_t signalConnected(void);
	
protected:
	siganelConnected_t m_siganelConnected;*/
	
private:
	Gtk::Image m_splashImage;
	//CURL *m_curl;
	//addrinfo *m_addrInfo;
	//int m_sockFD;
	
	boost::thread m_thread;
	
};

#endif /* MAINWINDOW_H_ */
