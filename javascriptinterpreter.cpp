#include "javascriptinterpreter.h"
#include <QtWidgets>

using namespace JS;
using namespace std;

JavaScriptInterpreter::JavaScriptInterpreter(QWidget *parent)
	: QMainWindow(parent), settings(QCoreApplication::organizationName(), QCoreApplication::applicationName())
{
	setup();
	createStatusBar();
	readSettings();
	
	TextEdit_input->setFocus();
	TextEdit_input->setFrameStyle(QFrame::NoFrame);

	splitter->setStretchFactor(0, 1);
	splitter->setStretchFactor(1, 0);

	QFont font;
	font.setFamily("Consolas");
	font.setFixedPitch(true);
	font.setPointSize(11);

	TextEdit_input->setFont(font);
	TextEdit_output->setFont(font);

	TextEdit_input->setTabStopWidth(30);
	TextEdit_output->setTabStopWidth(30);

	highlighter = new Highlighter(TextEdit_input->document());
	highlighter = new Highlighter(TextEdit_output->document());

	InitializeJS();
	textEditEvent();

	setCurrentFile(QString());
}

void JavaScriptInterpreter::InitializeJS()
{
	if (!JS_Init())
		return;
	

	rt = JS_NewRuntime(JS::DefaultHeapMaxBytes, JS::DefaultNurseryBytes); 
	if (!rt)
		return;

	cx = JS_NewContext(rt, 8192);
	if (!cx)
		return;

	JS_SetErrorReporter(rt, reportError);
}

QString JavaScriptInterpreter::EvalAndPrint(const char16_t* bytes, size_t length, int lineno)
{
	JS::CompileOptions options(cx, JSVERSION_DEFAULT);

	options.setIntroductionType("js shell interactive")
		.setUTF8(true)
		.setIsRunOnce(true)
		.setFileAndLine("UNHANDLED EXCEPTION: ", lineno);
	RootedScript script(cx);
	
	if (!JS_CompileUCScript(cx, bytes, length, options, &script))
		return "Empty";

	RootedValue result(cx);
	if (!JS_ExecuteScript(cx, script, &result))
		return "Empty";

	if (!result.isUndefined()) {
		
		RootedString str(cx);
		str = JS_ValueToSource(cx, result);
		if (!str)
			return false;

		RootedValue rtv(cx);
		char* utf8chars = JS_EncodeStringToUTF8(cx, str); 
		if (!utf8chars)
			return "Empty";

		out = QString(utf8chars);
		
		JS_free(cx, utf8chars);
	}
	
	return out;
}

void JavaScriptInterpreter::ReadEvalPrintLoop()
{
	JSAutoRequest ar(cx);
	JS::CompartmentOptions compartmentOptions;
	compartmentOptions.setVersion(JSVERSION_DEFAULT);
	JS::RootedObject global(cx, JS_NewGlobalObject(cx, &global_class, nullptr, JS::FireOnNewGlobalHook, compartmentOptions));
	if (!global)
		return;

	JS::RootedValue rval(cx);

	JSAutoCompartment ac(cx, global);

	if (!JS_InitStandardClasses(cx, global))
		return;

	v = JS_GetVersion(cx);
	actionJSVersion->setDisabled(false);

	/////////////////////////////////////////////////////////////

	int lineno = 1;
	int startline = lineno;
	errno = 1;

	u16string source = TextEdit_input->toPlainText().toStdU16String();
	
	if (source.empty()) {
		if (errno) {
			char buffer[80];
			strerror_s(buffer, 80, errno);
			JS_ReportError(cx, buffer);
			return;
		}
		return;
	}
	
	lineno++;

	QString output = EvalAndPrint(source.c_str(), source.length(), startline);
	if (output.isEmpty())
	{
		JS_ReportPendingException(cx);
	}
	
	if (checkBox->isChecked()) {
		TextEdit_output->setPlainText(output);
	}
	else {
		TextEdit_output->insertPlainText(output + "\n");
	}
	
}

void JavaScriptInterpreter::Interpreted()
{
	ReadEvalPrintLoop();
}

void JavaScriptInterpreter::reportError(JSContext *cx, const char *message, JSErrorReport *report)
{
	char* err = report->filename ? report->filename : "[no filename]";
	QString n;
	n.setNum(report->lineno);
	(new QErrorMessage())->showMessage(QString(err) + " " + QString(message) + ": Line number: " + n);
}


void JavaScriptInterpreter::newFile()
{
	if (maybeSave()) {
		TextEdit_input->clear();
		TextEdit_output->clear();
		setCurrentFile(QString());
	}
}

void JavaScriptInterpreter::openFile()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "JavaScript Files (*.js)");
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}

void JavaScriptInterpreter::about()
{
	QString about = tr("About");

	QString text("<p><FONT size=5 COLOR=#000000><i>Interpreter of JavaScript code ver. 2.5.2</i></FONT></p>"
		"<IMG SRC=\":/images/Resources/images/spidermonkey-logo.png\">"
		
		"<p><CENTER><FONT size=3 COLOR=#000000><i>Copyright&copy; 2016 Fetiev Nuri nurfet3@gmail.com</i></FONT></CENTER><p>"
		"<CENTER><FONT COLOR=#0046A1>"
		"    <I>version SpiderMonkey 45.0.2</I>"
		"</FONT></CENTER>"
		);

	QMessageBox::about(0, about, text);
}

bool JavaScriptInterpreter::maybeSave()
{
	if (!TextEdit_input->document()->isModified())
		return true;
	const QMessageBox::StandardButton ret
		= QMessageBox::warning(this, tr("JavaScript Interpreter"),
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	switch (ret) {
	case QMessageBox::Save:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}
	return true;
}

bool JavaScriptInterpreter::save()
{
	if (curFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}

bool JavaScriptInterpreter::saveAs()
{
	QFileDialog dialog(this, tr("Save File"), "", "JavaScript Files (*.js)");
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec() != QDialog::Accepted)
		return false;
	return saveFile(dialog.selectedFiles().first());
}

bool JavaScriptInterpreter::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName),
				file.errorString()));
		return false;
	}

	QTextStream out(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	out << TextEdit_input->toPlainText();
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void JavaScriptInterpreter::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	TextEdit_input->document()->setModified(false);
	setWindowModified(false);

	QString shownName = curFile;
	if (curFile.isEmpty())
		shownName = "untitled.js";
	setWindowFilePath(shownName);
}

void JavaScriptInterpreter::documentWasModified()
{
	setWindowModified(TextEdit_input->document()->isModified());
}

void JavaScriptInterpreter::commitData(QSessionManager &manager)
{
	if (manager.allowsInteraction()) {
		if (!maybeSave())
			manager.cancel();
	}
	else {
		if (TextEdit_input->document()->isModified())
			save();
	}
}

void JavaScriptInterpreter::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}

void JavaScriptInterpreter::readSettings()
{
	settings.beginGroup("/JavaScriptInterpreter");
	const QByteArray geometry = settings.value("/geometry", QByteArray()).toByteArray();
	QByteArray split = settings.value("/splitter").toByteArray();
	QByteArray fToolBar = settings.value("/fileToolBar").toByteArray();
	bool bEdit = settings.value("/edit", false).toBool();

	restoreGeometry(geometry);
	splitter->restoreState(split);
	restoreState(fToolBar);
	checkBox->setChecked(bEdit);

	settings.endGroup();
}

void JavaScriptInterpreter::writeSettings()
{
	settings.beginGroup("/JavaScriptInterpreter");

	settings.setValue("/geometry", saveGeometry());
	settings.setValue("/splitter", splitter->saveState());
	settings.setValue("/fileToolBar", saveState());
	settings.setValue("/edit", checkBox->isChecked());

	settings.endGroup();
}

void JavaScriptInterpreter::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(QDir::toNativeSeparators(fileName), file.errorString()));
		return;
	}

	QTextStream in(&file);
#ifndef QT_NO_CURSOR
	QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
	TextEdit_input->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
	QApplication::restoreOverrideCursor();
#endif

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}

void JavaScriptInterpreter::textEditEvent()
{

	connect(Button__clear_out, SIGNAL(clicked(bool)), TextEdit_output, SLOT(clear()));

	
	connect(TextEdit_input->document(), &QTextDocument::contentsChanged,
		this, &JavaScriptInterpreter::documentWasModified);


	QGuiApplication::setFallbackSessionManagementEnabled(false);
	connect(qApp, &QGuiApplication::commitDataRequest,
		this, &JavaScriptInterpreter::commitData);
}

void JavaScriptInterpreter::setup()
{
	resize(1400, 860);
	centralWidget = new QWidget(this);

	gridLayout = new QGridLayout(centralWidget);
	gridLayout->setSpacing(6);
	gridLayout->setContentsMargins(11, 11, 11, 11);

	splitter = new QSplitter(centralWidget);
	splitter->setOrientation(Qt::Vertical);

	TextEdit_input = new CodeEditor(splitter);
	splitter->addWidget(TextEdit_input);

	TextEdit_output = new QPlainTextEdit(splitter);
	TextEdit_output->setReadOnly(true);
	splitter->addWidget(TextEdit_output);

	gridLayout->addWidget(splitter, 0, 0, 1, 2);

	Button__clear_out = new QPushButton(centralWidget);
	Button__clear_out->setText(tr("Clear"));

	gridLayout->addWidget(Button__clear_out, 1, 0, 1, 1);

	horizontalSpacer = new QSpacerItem(1298, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	gridLayout->addItem(horizontalSpacer, 1, 1, 1, 1);

	setCentralWidget(centralWidget);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////

	fileMenu = menuBar()->addMenu(tr("File"));
	fileToolBar = addToolBar(tr("File"));

	QIcon newIcon;
	newIcon.addFile(QStringLiteral(":/images/Resources/images/new.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionNew = new QAction(newIcon, tr("&New"), this);
	actionNew->setShortcuts(QKeySequence::New);
	actionNew->setStatusTip(tr("Create a new file"));
	connect(actionNew, &QAction::triggered, this, &JavaScriptInterpreter::newFile);
	fileMenu->addAction(actionNew);
	fileToolBar->addAction(actionNew);
	
	
	QIcon openIcon;
	openIcon.addFile(QStringLiteral(":/images/Resources/images/open.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionOpen = new QAction(openIcon, tr("&Open..."), this);
	actionOpen->setShortcuts(QKeySequence::Open);
	actionOpen->setStatusTip(tr("Open an existing file"));
	connect(actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
	fileMenu->addAction(actionOpen);
	fileToolBar->addAction(actionOpen);
	//

	QIcon saveIcon;
	saveIcon.addFile(QStringLiteral(":/images/Resources/images/save.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionSave = new QAction(saveIcon, tr("&Save"), this);
	actionSave->setShortcuts(QKeySequence::Save);
	actionSave->setStatusTip(tr("Save the document to disk"));
	connect(actionSave, &QAction::triggered, this, &JavaScriptInterpreter::save);
	fileMenu->addAction(actionSave);
	fileToolBar->addAction(actionSave);

	QIcon savceAsIcon = QIcon::fromTheme("document-save-as");
	actionSaveAs = fileMenu->addAction(savceAsIcon, tr("&Save &As..."), this, &JavaScriptInterpreter::saveAs);
	actionSaveAs->setShortcuts(QKeySequence::SaveAs);
	actionSaveAs->setStatusTip(tr("Save the document under a new name"));

	fileMenu->addSeparator();

	QIcon exitIcon;
	exitIcon.addFile(QStringLiteral(":/images/Resources/images/exit.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionExit = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
	actionExit->setShortcuts(QKeySequence::Quit);
	actionExit->setStatusTip(tr("Exit the application"));

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	editMenu = menuBar()->addMenu(tr("&Edit"));
	editToolBar = addToolBar(tr("Edit"));
	//
	QIcon undoIcon;
	undoIcon.addFile(QStringLiteral(":/images/Resources/images/Undo.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionUndo = new QAction(undoIcon, tr("&Undo"), this);
	actionUndo->setShortcuts(QKeySequence::Undo);
	actionUndo->setStatusTip(tr("Undo"));
	connect(actionUndo, &QAction::triggered, TextEdit_input, &QPlainTextEdit::undo);
	editMenu->addAction(actionUndo);
	
	
	QIcon redoIcon;
	redoIcon.addFile(QStringLiteral(":/images/Resources/images/Redo.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionRedo = new QAction(redoIcon, tr("&Redo"), this);
	actionRedo->setShortcuts(QKeySequence::Redo);
	actionRedo->setStatusTip(tr("Redo"));
	connect(actionRedo, &QAction::triggered, TextEdit_input, &QPlainTextEdit::redo);
	editMenu->addAction(actionRedo);

	QIcon cutIcon;
	cutIcon.addFile(QStringLiteral(":/images/Resources/images/cut.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionCut = new QAction(cutIcon, tr("Cu&t"), this);
	actionCut->setShortcuts(QKeySequence::Cut);
	actionCut->setStatusTip(tr("Cut the current selection's contents to the "
		                       "clipboard"));
	connect(actionCut, &QAction::triggered, TextEdit_input, &QPlainTextEdit::cut);
	editMenu->addAction(actionCut);
	editToolBar->addAction(actionCut);
	

	QIcon copyIcon;
	copyIcon.addFile(QStringLiteral(":/images/Resources/images/copy.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionCopy = new QAction(copyIcon, tr("&Copy"), this);
	actionCopy->setShortcuts(QKeySequence::Copy);
	actionCopy->setStatusTip(tr("Copy the current selection's contents to the "
		                        "clipboard"));
	connect(actionCopy, &QAction::triggered, TextEdit_input, &QPlainTextEdit::copy);
	editMenu->addAction(actionCopy);
	editToolBar->addAction(actionCopy);
	
	QIcon pasteIcon;
	pasteIcon.addFile(QStringLiteral(":/images/Resources/images/paste.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionPaste = new QAction(pasteIcon, tr("&Paste"), this);
	actionPaste->setShortcuts(QKeySequence::Paste);
	actionPaste->setStatusTip(tr("Paste the clipboard's contents into the current "
		                         "selection"));
	connect(actionPaste, &QAction::triggered, TextEdit_input, &QPlainTextEdit::paste);
	editMenu->addAction(actionPaste);
	editToolBar->addAction(actionPaste);

	///////////////////////////////////////////////////////////////////////////////////////////

	undoRedoToolBar = addToolBar(tr("Undo"));

	actionUndo->setStatusTip(tr("Undo"));
	undoRedoToolBar->addAction(actionUndo);

	actionRedo->setStatusTip(tr("Redo"));
	undoRedoToolBar->addAction(actionRedo);

	///////////////////////////////////////////////////////////////////////////////////////////
	
	helpMenu = menuBar()->addMenu(tr("&Help"));
	QIcon aboutIcon;
	aboutIcon.addFile(QStringLiteral(":/images/Resources/images/about.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionAbout = helpMenu->addAction(aboutIcon, tr("&About"), this, &JavaScriptInterpreter::about, Qt::CTRL + Qt::Key_Q);
	actionAbout->setStatusTip(tr("Show the application's About box"));
	helpMenu->addAction(actionAbout);

	QIcon JavaScriptIcon;
	JavaScriptIcon.addFile(QStringLiteral(":/images/Resources/images/javascript.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionJSVersion = helpMenu->addAction(JavaScriptIcon, tr("&About JavaScript"), this, &JavaScriptInterpreter::javaScriptVersion);
	actionJSVersion->setStatusTip(tr("Version Information will be available after running the script"));
	actionJSVersion->setDisabled(true);
	helpMenu->addAction(actionJSVersion); 

	/////////////////////////////////////////////////////////////////////////////////////////////

	runToolBar = addToolBar(tr("Edit"));
	QIcon runIcon;
	runIcon.addFile(QStringLiteral(":/images/Resources/images/Run.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionRun = new QAction(runIcon, tr("&Run"), this);
	actionRun->setStatusTip(tr("Run"));
	connect(actionRun, &QAction::triggered, this, &JavaScriptInterpreter::Interpreted);
	runToolBar->addAction(actionRun);
	actionRun->setShortcut(Qt::Key_F7);
    
	QIcon clearIcon;
	clearIcon.addFile(QStringLiteral(":/images/Resources/images/Clear.png"), QSize(), QIcon::Normal, QIcon::Off);
	actionClear = new QAction(clearIcon, tr("&Clear"), this);
	actionClear->setStatusTip(tr("Clear"));
	connect(actionClear, &QAction::triggered, TextEdit_input, &QPlainTextEdit::clear);
	runToolBar->addAction(actionClear);

	checkBox = new QCheckBox(tr("Overwrite output string"));
	checkBox->setStatusTip(tr("Overwrite output string"));
	checkBox->setToolTip(tr("Overwrite output string"));
	helpToolBar = addToolBar(tr("Help"));
	helpToolBar->addAction(actionAbout);
	helpToolBar->addSeparator();

	helpToolBar->addWidget(checkBox);

	helpToolBar->addSeparator();

	actionCut->setEnabled(false);
	actionCopy->setEnabled(false);
	connect(TextEdit_input, &QPlainTextEdit::copyAvailable, actionCut, &QAction::setEnabled);
	connect(TextEdit_input, &QPlainTextEdit::copyAvailable, actionCopy, &QAction::setEnabled);
}

void JavaScriptInterpreter::createStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}

void JavaScriptInterpreter::javaScriptVersion()
{

	QString about = tr("Information about JavaScript");
	QString text = tr("JavaScript version:") + " " + QString(JS_VersionToString(v));

	QMessageBox::about(0, about, text);
}

JavaScriptInterpreter::~JavaScriptInterpreter()
{
	JS_DestroyContext(cx);
	JS_DestroyRuntime(rt);
	JS_ShutDown();
}
