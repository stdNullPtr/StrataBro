#pragma once

#include <QString>

class Stratagem
{
public:
	QString name_;
	QString code_;

	Stratagem(const QString& name, const QString& code)
		: name_(name), code_(code) {
	}
};