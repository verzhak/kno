#include <QApplication>
#include "knoFirstImpl.h"
//
int main(int argc, char ** argv)
{
	QApplication app( argc, argv );
	QTranslator translator;
	translator.load(":/translations/russian");
	app.installTranslator(&translator);
	dFirstImpl win;
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	return app.exec();
}
