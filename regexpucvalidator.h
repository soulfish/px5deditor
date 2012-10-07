#ifndef REGEXPUCVALIDATOR_H
#define REGEXPUCVALIDATOR_H

#include <QRegExpValidator>

class RegExpUCValidator : public QRegExpValidator
{
	Q_OBJECT
public:
	explicit RegExpUCValidator(const QRegExp& regexp, QObject *parent = 0) : QRegExpValidator(parent) {
		this->setRegExp(regExp());
	};

	virtual void fixup(QString &input) const {
		input = input.toUpper();
	}

	virtual State validate(QString &input, int &pos) const {

		State result = QRegExpValidator::validate(input, pos);

		if (result!=Acceptable) {
			fixup(input);
			result = Acceptable;
		}
		if(!input.isEmpty()) {
			fixup(input);
		}
		return QValidator::Acceptable;
	}

signals:

public slots:

};

#endif // REGEXPUCVALIDATOR_H
