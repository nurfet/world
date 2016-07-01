#ifndef JAVASCRIPTINTERPRETER_H
#define JAVASCRIPTINTERPRETER_H

#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include "CodeEditor.hpp"

#include <js/Initialization.h>
#include <jsapi.h>
#include <memory>
#include "Highlighter.hpp"
#include <string>
#include <QSettings>
#include <iostream>


static const JSClass global_class = {
	"global", JSCLASS_GLOBAL_FLAGS,
	nullptr, nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr,
	nullptr, nullptr, nullptr, nullptr,
	JS_GlobalObjectTraceHook
};


class QGridLayout;
class QPushButton;
class QSpacerItem;
class QSplitter;
class QCheckBox;

class JavaScriptInterpreter : public QMainWindow
{
	Q_OBJECT

public:
	JavaScriptInterpreter(QWidget *parent = 0);

	void setup();
	
	void InitializeJS();
	QString EvalAndPrint(const char16_t* bytes, size_t length, int lineno);
	void ReadEvalPrintLoop();

	~JavaScriptInterpreter();

public:
	void loadFile(const QString &fileName);

protected:
	void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

public slots:
	void Interpreted();

public slots:
	void about();
	void newFile();
	void openFile();
	bool save();
	bool saveAs();
	void documentWasModified();
	void commitData(QSessionManager &);
	void javaScriptVersion();

private:
	static void reportError(JSContext *cx, const char *message, JSErrorReport *report);
	bool maybeSave();
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	void readSettings();
	void writeSettings();
	void textEditEvent();

	void createStatusBar();

	Highlighter *highlighter;
	JSContext *cx;
	JSRuntime *rt;
	QString out;
	QString curFile;
	JSVersion v;

private:
	CodeEditor* codeEditor;
	QAction *actionNew;
	QAction *actionOpen;
	QAction *actionSave;
	QAction *actionSaveAs;
	QAction *actionExit;
	QAction *actionUndo;
	QAction *actionRedo;
	QAction *actionCut;
	QAction *actionCopy;
	QAction *actionPaste;
	QAction *actionAbout;
	QAction *actionRun;
	QAction *actionClear;
	QAction *actionJSVersion;
	QWidget *centralWidget;
	QGridLayout *gridLayout;
	QSplitter *splitter;
	QPlainTextEdit *TextEdit_input;
	QPlainTextEdit *TextEdit_output;
	QPushButton *Button__clear_out;
	QSpacerItem *horizontalSpacer;
	QMenu *fileMenu;
	QMenu *editMenu;
	QMenu *helpMenu;
	QToolBar *fileToolBar;
	QToolBar *editToolBar;
	QToolBar *undoRedoToolBar;
	QToolBar *runToolBar;
	QToolBar *helpToolBar;
	QSettings settings;
	QCheckBox *checkBox;
};

#endif // JAVASCRIPTINTERPRETER_H
