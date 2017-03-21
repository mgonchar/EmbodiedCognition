//#include "ExperimentBlock.h"
#include "ExperimentController.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
	QStringList paths = QCoreApplication::libraryPaths();
	paths.append(".");
	QCoreApplication::setLibraryPaths(paths);

	QApplication a(argc, argv);
	a.setOverrideCursor(Qt::BlankCursor);

	QCoreApplication::setApplicationName("EmbodiedCognitionExpirement");
	QCoreApplication::setApplicationVersion("1.0");

	QCommandLineParser parser;
	parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

	parser.setApplicationDescription(QCoreApplication::translate("main",
		"Provides series of text block to measure responce time under different conditions."));
	parser.addHelpOption();
	parser.addVersionOption();

	QCommandLineOption idOption(QStringList() << "id", QCoreApplication::translate("main", "Identificator of subject"), "subject id","");
	parser.addOption(idOption);

	QCommandLineOption distOption(QStringList() << "dist", QCoreApplication::translate("main", "Distance to the laptop screen in mm"), "distance in mm", "500");
	parser.addOption(distOption);

	QCommandLineOption colorOption(QStringList() << "color", QCoreApplication::translate("main", "Series with colored circles and text"));
	parser.addOption(colorOption);

	QCommandLineOption handOption(QStringList() << "hand", QCoreApplication::translate("main", "Series for hands"));
	parser.addOption(handOption);

	QCommandLineOption legOption(QStringList() << "leg", QCoreApplication::translate("main", "Series for legs"));
	parser.addOption(legOption);

	QCommandLineOption textAngleOption(QStringList() << "text_angle", QCoreApplication::translate("main", "View angle for text"), "angle in degrees", "5.0");
	parser.addOption(textAngleOption);

	QCommandLineOption circleAngleOption(QStringList() << "circle_angle", QCoreApplication::translate("main", "View angle for circle"), "angle in degrees", "3.2");
	parser.addOption(circleAngleOption);

	QCommandLineOption shiftAngleOption(QStringList() << "shift_angle", QCoreApplication::translate("main", "View angle shift"), "angle in degrees", "9.0");
	parser.addOption(shiftAngleOption);

	// Process the actual command line arguments given by the user
	parser.process(a);

	QString id = parser.value(idOption);
	bool colored_series = parser.isSet(colorOption);
	bool is_hand = !parser.isSet(legOption);
	int distance = parser.value(distOption).toInt();

	double text_angle   = parser.value(textAngleOption).toDouble(), 
		   circle_angle = parser.value(circleAngleOption).toDouble(), 
		   shift_angle  = parser.value(shiftAngleOption).toDouble();

	ExpirementController controller(&a, id, colored_series, distance, is_hand, circle_angle, text_angle, shift_angle);

	controller.EvaluateExperiment();

    return a.exec();
}
