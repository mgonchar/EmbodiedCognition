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
	bool colored_series = argc > 2 ? (QString(argv[2]) == "color") : false;
	ExpirementController controller(&a, id, colored_series);

	controller.EvaluateExperiment();

    return a.exec();
}
