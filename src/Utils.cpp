#include "Utils.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QThread>
#define WIN32_LEAN_AND_MEAN   
#include <windows.h>

namespace Utils
{
	QList<Stratagem> LoadStratagems()
	{
		QFile file(":/data/stratagem_data.json");
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			qDebug() << "Failed to open JSON file!";
			return {};
		}

		QByteArray jsonData = file.readAll();
		file.close();

		QJsonDocument doc = QJsonDocument::fromJson(jsonData);
		if (!doc.isArray()) {
			qDebug() << "Invalid JSON format!";
			return{};
		}

		QJsonArray jsonArray = doc.array();
		QList<Stratagem> stratagemList{};

		for (const QJsonValue& value : jsonArray) {
			QJsonObject obj = value.toObject();
			QString name = obj["stratagem"].toString();
			QString code = obj["code"].toString();

			stratagemList.append(Stratagem(name, code));
		}

		qDebug() << "Loaded data for" << stratagemList.size() << "stratagems!";
		return stratagemList;
	}

	// Example: "Up Down Left Right" -> [VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT]
	QList<int> TransformDirectionStringSequenceToVkCodes(const QString& code)
	{
		QList<int> keySequence;
		QStringList commands = code.split(' ', Qt::SkipEmptyParts);

		for (const QString& cmd : commands)
		{
			if (cmd == "Up") keySequence.append(VK_UP);
			else if (cmd == "Down") keySequence.append(VK_DOWN);
			else if (cmd == "Left") keySequence.append(VK_LEFT);
			else if (cmd == "Right") keySequence.append(VK_RIGHT);
		}

		return keySequence;
	}

	void PressKeys(const QList<int>& keySequence, const int delay)
	{
		for (const int vkCode : keySequence)
		{
			keybd_event(vkCode, 0, 0, 0);
			QThread::msleep(delay);
			keybd_event(vkCode, 0, KEYEVENTF_KEYUP, 0);
			QThread::msleep(delay);
		}
	}
}