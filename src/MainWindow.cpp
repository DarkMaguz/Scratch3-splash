/*
 * MainWindow.cpp
 *
 *  Created on: Mar 1, 2019
 *      Author: peter
 */

#include "MainWindow.h"
#include <gtkmm-3.0/gtkmm/window.h>

#include <curl/curl.h>
#include <json/json.h>

#include <fstream>

size_t WriteCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	((std::string*)stream)->append((char*)ptr, size * nmemb);
	
	std::string output;
	output.append((char*)ptr, size * nmemb);
	
	return size * nmemb;
}

size_t PullImageWriteCallback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	MainWindow *mw = (MainWindow*)stream;
	
	std::string output;
	output.append((char*)ptr, size * nmemb);
	
	Json::Reader jsonReader;
	Json::Value root;
	jsonReader.parse(output, root);
	
	mw->UpdateStatus(1, root["status"].asString());
	if (root["status"].asString() == "Downloading" || root["status"].asString() == "Extracting")
	{
		Json::Value details = root["progressDetail"];
		double current = details["current"].asDouble();
		double total = details["total"].asDouble();
		mw->UpdateProgress(1, current / total);
	}
	
	return size * nmemb;
}

void pull_image(MainWindow *mw)
{
	CURL *curl;
	CURLcode res;
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http:/v1.30/images/create?fromImage=darkmagus/scratch3");
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, "/var/run/docker.sock");
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &PullImageWriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)mw);
	
	// Perform the request, res will get the return code.
	res = curl_easy_perform(curl);
	
	// Check for errors.
	if (res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	
	// Cleanup,
	curl_easy_cleanup(curl);
}

void create_container(void)
{
	CURLcode res;
	CURL *curl;
	struct curl_slist *slist1;
	std::string output;
	
	slist1 = NULL;
	slist1 = curl_slist_append(slist1, "Content-Type: application/json");
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http:/v1.30/containers/create?name=scratch3");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "{'Image': 'darkmagus/scratch3', 'PortBindings':{'80/tcp':[{'HostIp':'','HostPort':'8601'}]}}");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, "/var/run/docker.sock");
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&output);
	
	res = curl_easy_perform(curl);
	
	// Check for errors.
	if (res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
}

void start_container(void)
{
	CURLcode res;
	CURL *curl;
	std::string output;
	
	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, "http:/v1.30/containers/scratch3/start");
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(curl, CURLOPT_UNIX_SOCKET_PATH, "/var/run/docker.sock");
	
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&output);
	
	res = curl_easy_perform(curl);
	
	// Check for errors.
	if (res != CURLE_OK)
		fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
	
	curl_easy_cleanup(curl);
}


bool wait_for_connection(MainWindow *mw)
{
	CURLcode res = CURL_LAST;
	CURL *curl;
	
	curl = curl_easy_init();
	int trys = 0;
	while (res != CURLE_OK && trys++ < CON_ATTEMPT)
	{
		std::string output;
		curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8601");
		
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&output);
		
		res = curl_easy_perform(curl);
		
		if (res != CURLE_OK && trys < CON_ATTEMPT)
			sleep(1);
		
		mw->UpdateProgress(1, trys / CON_ATTEMPT);
	}
	
	curl_easy_cleanup(curl);
	
	return (res == CURLE_OK);
}

void thread(MainWindow *mw)
{
	
	mw->UpdateStatus(2, "Getting the latest image version");
	pull_image(mw);
	
	mw->UpdateProgress(2, 0.25);
	mw->UpdateStatus(2, "Creating container");
	create_container();
	
	mw->UpdateProgress(2, 0.50);
	mw->UpdateStatus(2, "Starting container");
	start_container();
	
	mw->UpdateProgress(2, 0.75);
	mw->UpdateStatus(2, "Connecting");
	wait_for_connection(mw);
	
	mw->close();
}

MainWindow::MainWindow() :
	m_SplashImage(std::string(IMAGE_DATA_DIR) + "/scratch-hack.png")
	//m_SplashImage("scratch-hack.png")
{
	
	add(m_Layout);
	
	m_Layout.put(m_SplashImage, 0, 0);
	
	Glib::RefPtr<Gdk::Pixbuf> pb = m_SplashImage.get_pixbuf();
	set_size_request(pb->get_width(), pb->get_height());
	set_decorated(false);
	set_keep_above(true);
	
	m_Layout.put(m_MainHBox, 0, 0);
	
	m_MainHBox.set_size_request(pb->get_width(), pb->get_height());
	
	m_MainHBox.add(m_MainVBox);
	
	m_MainVBox.set_margin_left(100);
	m_MainVBox.set_margin_right(100);
	m_MainVBox.set_margin_bottom(20);
	
	m_MainVBox.pack_end(m_SecondProgressBar, false, false, 10);
	m_MainVBox.pack_end(m_FirstProgressBar, false, false, 10);
	
	m_FirstProgressBar.set_fraction(0.0);
	m_SecondProgressBar.set_fraction(0.0);
	
	m_FirstProgressBar.set_size_request(pb->get_width() / 2, 20);
	m_SecondProgressBar.set_size_request(pb->get_width() / 2, 20);
	
	m_FirstProgressBar.set_text("");
	m_SecondProgressBar.set_text("");
	
	m_FirstProgressBar.set_show_text(true);
	m_SecondProgressBar.set_show_text(true);
	
	show_all();
	

}

MainWindow::~MainWindow()
{
}

void MainWindow::UpdateProgress(const int& bar, const double& fraction)
{
	if (bar == 1)
		m_FirstProgressBar.set_fraction(fraction);
	else if (bar == 2) 
		m_SecondProgressBar.set_fraction(fraction);
}

void MainWindow::UpdateStatus(const int& bar, const std::string& status)
{
	if (bar == 1)
		m_FirstProgressBar.set_text(status);
	else if (bar == 2) 
		m_SecondProgressBar.set_text(status);
}

