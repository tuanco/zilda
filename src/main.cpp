/*
 *  MainWindow.h
 *  zILDA
 *
 *  Created by Andre Normann on 05.11.09.
 *  Copyright 2009 Andre Normann. All rights reserved.
 *
 */

//=======================================================================================
//  Headerfiles
//=======================================================================================

// Qt
#include <QtGui>

// Project
#include "MainWindow.h"
//#include "DarkStyle.h"

//=======================================================================================

#if defined(Q_WS_WIN)
	#if defined _M_IX86
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_IA64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#elif defined _M_X64
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#else
		#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
	#endif
#endif

//=======================================================================================

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
	//app.setStyle(new DarkStyle());
	
	Q_INIT_RESOURCE(zilda);
	
	QPlastiqueStyle *style = new QPlastiqueStyle();
	QApplication::setStyle(style);
	
	QPalette pal = app.palette();
	pal.setColor(QPalette::Window, QColor(92, 92, 92));
	pal.setColor(QPalette::WindowText, QColor(255, 255, 255));
	pal.setColor(QPalette::Base, QColor(80, 80, 80));
	pal.setColor(QPalette::AlternateBase, QColor(102, 102, 102));
	pal.setColor(QPalette::Text, QColor(255, 255, 255));
	pal.setColor(QPalette::Button, QColor(92, 92, 92));
	pal.setColor(QPalette::ButtonText, QColor(255, 255, 255));
	pal.setColor(QPalette::Light, QColor(122, 122, 122));
	pal.setColor(QPalette::Midlight, QColor(122, 122, 122));
	pal.setColor(QPalette::Mid, QColor(122, 122, 122));
	pal.setColor(QPalette::Highlight, QColor(162, 162, 162));
	app.setPalette(pal);
	
	QString locale(QLocale::system().name());
	QTranslator translator;
	translator.load(QString("zILDA_") + locale);
	app.installTranslator(&translator);
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	return app.exec();
}

//=======================================================================================
