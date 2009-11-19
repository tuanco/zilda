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
#include "DarkStyle.h"

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
	
	Q_INIT_RESOURCE(zilda);
	
	QStyle *style = new DarkStyle();
	QApplication::setStyle(style);
	
	QPalette pal = app.palette();

	pal.setColor(QPalette::Window, QColor(71, 71, 71));
	pal.setColor(QPalette::Base, QColor(87, 87, 87));
	pal.setColor(QPalette::Text, QColor(228, 228, 228));
	pal.setColor(QPalette::HighlightedText, QColor(38, 38, 39));
	pal.setColor(QPalette::ButtonText, QColor(228, 228, 228));
	pal.setColor(QPalette::Highlight, QColor(160, 160, 160));
	pal.setColor(QPalette::WindowText, QColor(220, 220, 220));
	pal.setColor(QPalette::Mid, QColor(50, 50, 50));
	pal.setColor(QPalette::Midlight, QColor(85, 85, 85));
	pal.setColor(QPalette::Button, QColor(67, 67, 67));
	pal.setColor(QPalette::Dark, QColor(10, 10, 10));
	pal.setColor(QPalette::Light, QColor(122, 122, 122));
	pal.setColor(QPalette::Shadow, QColor(38, 38, 38));
	
	pal.setColor(QPalette::Disabled, QPalette::Text, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Highlight, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Shadow, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Light, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Midlight, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Dark, pal.color(QPalette::Active, QPalette::Text).dark());
	pal.setColor(QPalette::Disabled, QPalette::Mid, pal.color(QPalette::Active, QPalette::Text).dark());

	app.setPalette(pal);
	
	app.setStyleSheet("QWidget { color: #e4e4e4 }");
	
	QString locale(QLocale::system().name());
	QTranslator translator;
	translator.load(QString("zILDA_") + locale);
	app.installTranslator(&translator);
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();	

	return app.exec();
}

//=======================================================================================
