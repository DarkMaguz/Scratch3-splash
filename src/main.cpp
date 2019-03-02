/*
 ============================================================================
 Name        : Scratch3-splash.cpp
 Author      : Peter Balling
 Version     :
 Copyright   : MIT
 Description : Hello World in C++,
 ============================================================================
 */
#include "MainWindow.h"

#include <gtkmm/application.h>

int main(int argc, char **argv) {
	auto app = Gtk::Application::create(argc, argv, "dk.codingpirates.scratch3");
	
	MainWindow mw;

	boost::thread t(boost::function<void(void)>(boost::bind(thread, &mw)));
	//t.detach();
	
	int ret = app->run(mw);
	
	t.join();
	
	return ret;
}
