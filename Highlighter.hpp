#pragma once
#include <QSyntaxHighlighter>
#include <QTextCharFormat>

class Highlighter : public QSyntaxHighlighter {
	Q_OBJECT

public:
	Highlighter(QTextDocument *parent = Q_NULLPTR);
	~Highlighter();

protected:
	void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

private:
	struct HighlightingRule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};
	QVector<HighlightingRule> highlightingRules;

	QRegExp commentStartExpression;
	QRegExp commentEndExpression;

	QTextCharFormat keywordFormatMethods;
	QTextCharFormat keywordFormatIdentifiers;
	QTextCharFormat keywordFormatObjects;

	QTextCharFormat classFormat;
	QTextCharFormat singleLineCommentFormat;
	QTextCharFormat multiLineCommentFormat;
	QTextCharFormat quotationFormat;
	QTextCharFormat functionFormat;
};
