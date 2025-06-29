#include "qtetksqlite3sample.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	qtETKSQLite3Sample w;
	w.show();
	return a.exec();
}
