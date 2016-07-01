#include "javascriptinterpreter.h"
#include <QtWidgets/QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTranslator>

int main(int argc, char *argv[])
{
	setlocale(LC_ALL, "");
	QApplication app(argc, argv);
	QCoreApplication::setApplicationName("JavaScript Interpreter");
	QCoreApplication::setOrganizationName("Sigma");
	QCoreApplication::setApplicationVersion(QT_VERSION_STR);
	QCommandLineParser parser;
	parser.setApplicationDescription(QCoreApplication::applicationName());
	parser.addHelpOption();
	parser.addVersionOption();
	parser.addPositionalArgument("file", "The file to open.");
	parser.process(app);

	/*QTranslator translator;
	translator.load(":/translations/Resources/translations/i18n_javascriptinterpreter_ru.qm"); 
	app.installTranslator(&translator);*/

	JavaScriptInterpreter mainWin;
	if (!parser.positionalArguments().isEmpty())
		mainWin.loadFile(parser.positionalArguments().first()); 
	mainWin.show();
	return app.exec();
}
