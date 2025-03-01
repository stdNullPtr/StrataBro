#pragma once
#include <Stratagem.h>
#include <QList>

namespace Utils
{
	QList<Stratagem> LoadStratagems();
	QList<int> TransformDirectionStringSequenceToVkCodes(const QString& code);
	void PressKeys(const QList<int>& keySequence, const int delay);
}