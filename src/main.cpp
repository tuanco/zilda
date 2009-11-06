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
	app.setStyle(new DarkStyle());
	
	QString locale(QLocale::system().name());
	QTranslator translator;
	translator.load(QString("zILDA_") + locale);
	app.installTranslator(&translator);
	
	MainWindow *mainWindow = new MainWindow();
	mainWindow->show();

	return app.exec();
}

//=======================================================================================
