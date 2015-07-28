TEMPLATE = app
QT = gui core network
CONFIG += qt warn_on release
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
FORMS = ui/knoFirst.ui ui/aboutKNO.ui ui/knoGame.ui
HEADERS = src/knoFirstImpl.h src/aboutknodialogimpl.h src/knoGameimpl.h src/polewidget.h
SOURCES = src/knoFirstImpl.cpp \
 src/main.cpp \
 src/aboutknodialogimpl.cpp \
 src/knoGameimpl.cpp \
 src/polewidget.cpp
RESOURCES += ui/lbl.qrc
TRANSLATIONS += russian.ts
