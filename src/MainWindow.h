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

#define CON_ATTEMPT 30

#include <boost/thread.hpp>

class MainWindow;

void thread(MainWindow *mw);

class MainWindow : public Gtk::Window {
public:
	MainWindow();
	virtual ~MainWindow();
	
	void UpdateProgress(const int& bar, const double& fraction);
	void UpdateStatus(const int& bar, const std::string& status);
	
private:
	Gtk::Layout m_Layout;
	Gtk::VBox m_MainVBox;
	Gtk::HBox m_MainHBox;
	Gtk::Image m_SplashImage;
	
	Gtk::ProgressBar m_FirstProgressBar;
	Gtk::ProgressBar m_SecondProgressBar;
	
	boost::thread m_Thread;
	
};

#endif /* MAINWINDOW_H_ */
