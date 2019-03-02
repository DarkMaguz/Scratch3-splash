/*
 * MainWindow.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: peter
 */

#include "MainWindow.h"
#include <gtkmm-3.0/gtkmm/window.h>

void thread(MainWindow *mw)
{
	
	sleep(3);
	
	boost::asio::io_context::count_type count = -2;
	for (int i = 0; i < 10; ++i) 
	{
		try
		{
			boost::asio::io_service io_service;
			TcpClient c(io_service, "localhost", "/");
			count = io_service.run();
		} catch (std::exception& e)
		{
			std::cout << "Exception: " << e.what() << "\n";
			count = -1;
		}
		if (count >= 6 || count == -1)
			break;
		if (!mw)
		{
			std::cout << "Error: Terminating." << count << std::endl;
			exit(1);
		}
		sleep(1);
	}
	
	std::cout << "Done1!" << count << std::endl;
	mw->close();
}

MainWindow::MainWindow() :
		m_splashImage("scratch.jpg")
//	m_addrInfo(NULL),
//	m_sockFD(0)
//	m_curl(curl_easy_init())
{
	
	set_size_request(800, 600);
	add(m_splashImage);
	//fullscreen();
	set_keep_above(true);
	m_splashImage.show();
	
	//signalConnected().connect()

	//struct curl_slist *connect_to = NULL;
	//connect_to = curl_slist_append(NULL, "example.com::server1.example.com:");

	//curl_easy_setopt(m_curl, CURLOPT_URL, "http://localhost:8601");
	//curl_easy_perform(m_curl);
	//curl_multi_timeout

	/*addrinfo hints;
	 memset(&hints, 0, sizeof hints);
	 hints.ai_family = AF_UNSPEC;
	 hints.ai_socktype = SOCK_STREAM;*/
	
	std::cout << "Done2!" << std::endl;
	
}

MainWindow::~MainWindow()
{
	std::cout << "~MainWindow()" << std::endl;
}

/*MainWindow::siganelConnected_t MainWindow::signalConnected(void)
{
	return m_siganelConnected;
}*/
