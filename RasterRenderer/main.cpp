#include "RasterRenderer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	RasterRenderer w;
	w.show();
	return a.exec();
}
