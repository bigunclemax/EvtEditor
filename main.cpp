#include "evteditor.h"
#include <QtWidgets/QApplication>

#include "RecordRemover.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	EvtEditor w;
	w.show();
	return a.exec();
}
