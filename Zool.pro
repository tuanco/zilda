CONFIG += qt opengl exceptions thread rtti debug_and_release
QT += opengl
TEMPLATE = app
TARGET = Zool

HEADERS += src/MainWindow.h \
			src/DarkStyle.h
SOURCES += src/main.cpp \
			src/MainWindow.cpp \
			src/DarkStyle.cpp
FORMS += src/MainWindow.ui
#RESOURCES += resources.qrc

TRANSLATIONS = Zool_de.ts

win32 {
    CONFIG += embed_manifest_exe
    CONFIG(debug, debug|release) {    
        OBJECTS_DIR = build/debug
        MOC_DIR = build/debug
        RCC_DIR = build/debug
        UI_DIR = build/debug
        DESTDIR = build/debug
    } else {
        OBJECTS_DIR = build/release
        MOC_DIR = build/release
        RCC_DIR = build/release
        UI_DIR = build/release
        DESTDIR = build/release
    }
}

macx {
	QMAKE_MAC_SDK=/Developer/SDKs/MacOSX10.5.sdk
	CONFIG += x86 x64
    CONFIG(debug, debug|release) {
        OBJECTS_DIR = build/debug
        MOC_DIR = build/debug
        RCC_DIR = build/debug
        DESTDIR = build/debug
        UI_DIR = build/debug
    } else {
        OBJECTS_DIR = build/release
        MOC_DIR = build/release
        RCC_DIR = build/release
        DESTDIR = build/release
        UI_DIR = build/release
    }
}

unix {
}

# install
target.path = bin
INSTALLS += target
