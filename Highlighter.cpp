#include "Highlighter.hpp"

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent) 
{
	HighlightingRule rule;


	keywordFormatMethods.setForeground(Qt::darkRed);
	keywordFormatMethods.setFontItalic(true);

	keywordFormatIdentifiers.setForeground(Qt::blue);

	keywordFormatObjects.setForeground(Qt::darkCyan);
	keywordFormatObjects.setFontItalic(true);

	//
	QStringList keywordPatternsMethods;
	keywordPatternsMethods
		<< "\\bcharAt\\b" << "\\bisFinite\\b" << "\\bsin\\b" << "\\bgetMonth\\b" << "\\bsetUTCMilliseconds\\b" << "\\blastIndexOf\\b"
		<< "\\bcharCodeAt\\b" << "\\bisInteger\\b" << "\\bsqrt\\b" << "\\bgetSeconds\\b" << "\\bsetUTCMinutes\\b" << "\\bmap\\b"
		<< "\\bendsWith\\b" << "\\bisNaN\\b" << "\\btan\\b" << "\\bgetTime\\b" << "\\bsetUTCMonth\\b" << "\\bpop\\b"
		<< "\\bfromCharCode\\b" << "\\bisSafeInteger\\b" << "\\bimul\\b" << "\\bgetTimezoneOffset\\b" << "\\bsetUTCSeconds\\b" << "\\bpush\\b"
		<< "\\bincludes\\b" << "\\btoExponential\\b" << "\\bclz32\\b" << "\\bgetUTCDate\\b" << "\\bsetYear\\b" << "\\breduce\\b"
		<< "\\blocaleCompare\\b" << "\\btoFixed\\b" << "\\bfround\\b" << "\\bgetUTCDay\\b" << "\\btoDateString\\b" << "\\breduceRight\\b"
		<< "\\bmatch\\b" << "\\btoPrecision\\b" << "\\blog10\\b" << "\\bgetUTCFullYear\\b" << "\\btoGMTString\\b" << "\\breverse\\b"
		<< "\\brepeat\\b" << "\\bparseInt\\b" << "\\blog2\\b" << "\\bgetUTCHours\\b" << "\\btoISOString\\b" << "\\bshift\\b"
		<< "\\breplace\\b" << "\\bparseFloat\\b" << "\\blog1p\\b" << "\\bgetUTCMilliseconds\\b" << "\\btoJSON\\b" << "\\bslice\\b"
		<< "\\bsearch\\b" << "\\bEPSILON\\b" << "\\bexpm1\\b" << "\\bgetUTCMinutes\\b" << "\\btoLocaleDateString\\b" << "\\bsome\\b"
		<< "\\bsplit\\b" << "\\bMAX_SAFE_INTEGER\\b" << "\\bcosh\\b" << "\\bgetUTCMonth\\b" << "\\btoLocaleTimeString\\b" << "\\bsort\\b"
		<< "\\bstartsWith\\b" << "\\babs\\b" << "\\bsinh\\b" << "\\bgetUTCSeconds\\b" << "\\btoLocaleString\\b" << "\\bsplice\\b"
		<< "\\bsubstr\\b" << "\\bacos\\b" << "\\btanh\\b" << "\\bgetYear\\b" << "\\btoTimeString\\b" << "\\bunshift\\b"
		<< "\\bsubstring\\b" << "\\basin\\b" << "\\bacosh\\b" << "\\bnow\\b" << "\\btoUTCString\\b" << "\\bfrom\\b" << "\\bbind\\b"
		<< "\\btoLocaleLowerCase\\b" << "\\batan\\b" << "\\basinh\\b" << "\\bparse\\b" << "\\bUTC\\b" << "\\bof\\b"
		<< "\\btoLocaleUpperCase\\b" << "\\batan2\\b" << "\\batanh\\b" << "\\bsetDate\\b" << "\\bconcat\\b" << "\\bentries\\b"
		<< "\\btoLowerCase\\b" << "\\bceil\\b" << "\\bhypot\\b" << "\\bsetFullYear\\b" << "\\bcopyWithin\\b" << "\\bkeys\\b"
		<< "\\btoString\\b" << "\\bcos\\b" << "\\btrunc\\b" << "\\bsetHours\\b" << "\\bevery\\b" << "\\bdecodeURI\\b"
		<< "\\btoUpperCase\\b" << "\\bexp\\b" << "\\bsign\\b" << "\\bsetMilliseconds\\b" << "\\bfill\\b" << "\\bdecodeURIComponent\\b"
		<< "\\btrim\\b" << "\\bfloor\\b" << "\\bcbrt\\b" << "\\bsetMinutes\\b" << "\\bfilter\\b" << "\\bencodeURI\\b"
		<< "\\bvalueOf\\b" << "\\blog\\b" << "\\bgetDate\\b" << "\\bsetMonth\\b" << "\\bfind\\b" << "\\bencodeURIComponent\\b"
		<< "\\bfromCodePoint\\b" << "\\bmax\\b" << "\\bgetDay\\b" << "\\bsetSeconds\\b" << "\\bfindIndex\\b" << "\\bescape\\b"
		<< "\\bcodePointAt\\b" << "\\bmin\\b" << "\\bgetFullYear\\b" << "\\bsetTime\\b" << "\\bforEach\\b" << "\\beval\\b"
		<< "\\bcontains\\b" << "\\bpow\\b" << "\\bgetHours\\b" << "\\bsetUTCDate\\b" << "\\bindexOf\\b" << "\\bNumber\\b"
		<< "\\bnormalize\\b" << "\\brandom\\b" << "\\bgetMilliseconds\\b" << "\\bsetUTCFullYear\\b" << "\\bisArray\\b" << "\\bString\\b"
		<< "\\braw\\b" << "\\bround\\b" << "\\bgetMinutes\\b" << "\\bsetUTCHours\\b" << "\\bjoin\\b" << "\\bforEach\\b"
		<< "\\bentries\\b" << "\\bkeys\\b" << "\\bvalues\\b" << "\\bWeakMap\\b" << "\\bclear\\b" << "\\bWeakSet\\b"
		<< "\\bapply\\b" << "\\bsetPrototypeOf\\b" << "\\bgetOwnPropertySymbols\\b" << "\\btoMethod\\b" << "\\bBoolean\\b" << "\\bEvalError\\b"
		<< "\\bInternalError\\b" << "\\bRangeError\\b" << "\\bReferenceError\\b" << "\\bSyntaxError\\b" << "\\bTypeError\\b" << "\\bURIError\\b"
		<< "\\bError\\b" << "\\bcreate\\b" << "\\bMath\\b" << "\\bcallee\\b" << "\\bcaller\\b" << "\\bwatch\\b" << "\\bseal\\b"
		<< "\\bisPrototypeOf\\b" << "\\bhasOwnProperty\\b" << "\\bassign\\b" << "\\bdefineProperties\\b" << "\\bdefineProperty\\b" << "\\bfreeze\\b" 
	    << "\\bgetOwnPropertyDescriptor\\b" << "\\bgetOwnPropertyNames\\b" << "\\bgetPrototypeOf\\b" << "\\bis\\b" << "\\bisExtensible\\b" 
		<< "\\bisFrozen\\b" << "\\bisSealed\\b" << "\\bobserve\\b" << "\\bpreventExtensions\\b" << "\\b__defineGetter__\\b" << "\\b__defineSetter__\\b" 
		<< "\\b__lookupGetter__\\b" << "\\b__lookupSetter__\\b" << "\\bpropertyIsEnumerable\\b" << "\\btoSource\\b" << "\\bunwatch\\b" << "\\bcall\\b";

	foreach(const QString &pattern, keywordPatternsMethods) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormatMethods;
		highlightingRules.append(rule);
	}
	//

	QStringList keywordPatternsIdentifiers;
	keywordPatternsIdentifiers
		<< "\\bbreak\\b" << "\\bcontinue\\b" << "\\bdebugger\\b" << "\\bdo\\b" << "\\bwhile\\b"
		<< "\\bfor\\b" << "\\bin\\b" << "\\bfunction\\b" << "\\bif\\b" << "\\belse\\b"
		<< "\\breturn\\b" << "\\bswitch\\b" << "\\bthrow\\b" << "\\btry\\b" << "\\bcatch\\b"
		<< "\\bfinally\\b" << "\\bof\\b" << "\\bimplements\\b" << "\\bvar\\b" << "\\blet\\b"
		<< "\\bnew\\b" << "\\b=>\\b" << "\\bthis\\b" << "\\bfunction\\b" << "\\btypeof\\b"
		<< "\\btrue\\b" << "\\bfalse\\b" << "\\bDataView\\b" << "\\bset\\b" << "\\binstanceof\\b"
		<< "\\bInt8Array\\b" << "\\bUint8Array\\b" << "\\bUint8ClampedArray\\b" << "\\bInt16Array\\b" << "\\bUint16Array\\b"
		<< "\\bInt32Array\\b" << "\\bUint32Array\\b" << "\\bFloat32Array\\b" << "\\bFloat64Array\\b" << "\\bconst\\b"
		<< "\\bstatic\\b" << "\\bclass\\b" << "\\bnull\\b" << "\\bprivate\\b" << "\\bpublic\\b" << "\\bsuper\\b"
		<< "\\bBool16x8\\b" << "\\bBool32x4\\b" << "\\bBool64x2\\b" << "\\bBool8x16\\b" << "\\bFloat32x4\\b"
		<< "\\bFloat64x2\\b" << "\\bInt16x8\\b" << "\\bInt32x4\\b" << "\\bInt8x16\\b" << "\\bUint16x8\\b"
		<< "\\bpackage\\b" << "\\bprotected\\b" << "\\babstract\\b" << "\\bdouble\\b" << "\\bgoto\\b"
		<< "\\bnative\\b" << "\\bboolean\\b" << "\\benum\\b" << "\\bpackage\\b" << "\\bbyte\\b"
		<< "\\bexport\\b" << "\\bimport\\b" << "\\bsynchronized\\b" << "\\bchar\\b" << "\\bextends\\b"
		<< "\\bint\\b" << "\\bprotected\\b" << "\\bthrows\\b" << "\\bfinal\\b" << "\\binterface\\b"
		<< "\\btransient\\b" << "\\bfloat\\b" << "\\blong\\b" << "\\bshort\\b" << "\\bvolatile\\b"
		<< "\\bUint32x4\\b" << "\\bUint8x16\\b" << "\\bget\\b" << "\\bset\\b" << "\\bwith\\b";
		

	foreach(const QString &pattern, keywordPatternsIdentifiers) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormatIdentifiers;
		highlightingRules.append(rule);
	}

	//
	QStringList keywordPatternsObjects;
	keywordPatternsObjects
		<< "\\bMap\\b" << "\\bmap\\b" << "\\bSet\\b" 
		<< "\\bPromise\\b" << "\\barguments\\b" << "\\bArray\\b"
		<< "\\bSymbol\\b" << "\\bRegExp\\b" << "\\bObject\\b" << "\\bInfinity\\b"
		<< "\\bundefined\\b" << "\\bconstructor\\b" << "\\blength\\b" << "\\bprototype\\b"
		<< "\\bMAX_VALUE\\b" << "\\bMIN_VALUE\\b" << "\\bNEGATIVE_INFINITY\\b" << "\\bNaN\\b"
		<< "\\bPOSITIVE_INFINITY\\b" << "\\bE\\b" << "\\bLN2\\b" << "\\bLN10\\b"
		<< "\\bLOG2E\\b" << "\\bLOG10E\\b" << "\\bPI\\b" << "\\bSQRT1_2\\b"
		<< "\\bSQRT2\\b" << "\\bArrayBuffer\\b" << "\\bStopIteration\\b" << "\\bTypedArray\\b"
		<< "\\bAtomics\\b" << "\\bDate\\b" << "\\bFunction\\b" << "\\bGenerator\\b" << "\\bGeneratorFunction\\b"
		<< "\\bIntl\\b" << "\\bCollator\\b" << "\\bDateTimeFormat\\b" << "\\bNumberFormat\\b" << "\\bIterator\\b"
		<< "\\bJSON\\b" << "\\bParallelArray\\b" << "\\bReflect\\b" << "\\bSIMD\\b" << "\\bSharedArrayBuffer\\b"; 

	foreach(const QString &pattern, keywordPatternsObjects) {
		rule.pattern = QRegExp(pattern);
		rule.format = keywordFormatObjects;
		highlightingRules.append(rule);
	}
	//

	classFormat.setFontWeight(QFont::Bold);
	classFormat.setForeground(Qt::darkMagenta);
	rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
	rule.format = classFormat;
	highlightingRules.append(rule);

	singleLineCommentFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("//[^\n]*");
	rule.format = singleLineCommentFormat;
	highlightingRules.append(rule);

	multiLineCommentFormat.setForeground(Qt::darkGreen);

	quotationFormat.setForeground(Qt::darkGreen);
	rule.pattern = QRegExp("\"([^\"]{1,})\"");
	rule.format = quotationFormat;
	highlightingRules.append(rule);

	functionFormat.setFontItalic(true);
	functionFormat.setForeground(Qt::darkRed);
	rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
	rule.format = functionFormat;
	highlightingRules.append(rule);

	commentStartExpression = QRegExp("/\\*");
	commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
	foreach(const HighlightingRule &rule, highlightingRules) {
		QRegExp expression(rule.pattern);
		int index = expression.indexIn(text);
		while (index >= 0) {
			int length = expression.matchedLength();
			setFormat(index, length, rule.format);
			index = expression.indexIn(text, index + length);
		}
	}

	setCurrentBlockState(0);

	int startIndex = 0;
	if (previousBlockState() != 1)
		startIndex = commentStartExpression.indexIn(text);

	while (startIndex >= 0) {

		int endIndex = commentEndExpression.indexIn(text, startIndex);
		int commentLength;
		if (endIndex == -1) {
			setCurrentBlockState(1);
			commentLength = text.length() - startIndex;
		}
		else {
			commentLength = endIndex - startIndex
				+ commentEndExpression.matchedLength();
		}
		setFormat(startIndex, commentLength, multiLineCommentFormat);
		startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
	}
}

Highlighter::~Highlighter() 
{
	
}
