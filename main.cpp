#include "mainwindow.h"
//#include "ExperimentBlock.h"
#include "ExperimentController.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QStringList paths = QCoreApplication::libraryPaths();
	paths.append(".");
	QCoreApplication::setLibraryPaths(paths);

    QApplication a(argc, argv);
	a.setOverrideCursor(Qt::BlankCursor);

	QString id = argc > 1 ? argv[1] : "";
	ExpirementController controller(&a, id);

	controller.EvaluateExperiment();

    return a.exec();
}
