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
    //MainWindow w;
    //w.show();
	
	//ExperimentBlock block;
	//block.show();
	ExpirementController controller(&a);

	controller.EvaluateExperiment();

    return a.exec();
}
