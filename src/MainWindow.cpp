#include "MainWindow.h"
#include <Utils.h>
#include <QMessageBox>
#include <QCompleter>
#include <QSlider>
#include <QComboBox>
#include <QSettings>
#include <QStandardItemModel>
#define WIN32_LEAN_AND_MEAN   
#include <windows.h>
#include <Version.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

	setWindowTitle(QString("StrataBro v%1").arg(APP_VERSION_STR));

	connect(ui.dialSpeedSlider, &QSlider::valueChanged, this, [=](int value) {
		ui.dialSpeedSpinBox->setValue(value);
		});

	const char* dialogErrorMsgTemplate = "Unexpected error while loading stratagem settings: %1";

	const QList<Stratagem> stratagemList = Utils::LoadStratagems();
	if (stratagemList.isEmpty())
	{
		qDebug() << "Failed loading stratagems, check data.json";
		QMessageBox::critical(this, "Error", QString(dialogErrorMsgTemplate).arg("0x10"));
	}

	for (qsizetype comboBoxCounter = 0; comboBoxCounter < MAX_STRATAGEM_HOTKEYS; comboBoxCounter++)
	{
		QComboBox* stratagemComboBox = findChild<QComboBox*>(QString("stratagemComboBox_%1").arg(comboBoxCounter));
		if (!stratagemComboBox)
		{
			QMessageBox::critical(this, "Error", QString(dialogErrorMsgTemplate).arg("0x11"));
			// don't loop further, this is a critical error
			break;
		}

		// Enhance filtering of comboboxes
		QCompleter* completer = new QCompleter(stratagemComboBox->model(), stratagemComboBox);
		completer->setCaseSensitivity(Qt::CaseInsensitive);
		completer->setFilterMode(Qt::MatchContains);
		completer->setCompletionMode(QCompleter::PopupCompletion);
		stratagemComboBox->setCompleter(completer);

		connect(stratagemComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
			qDebug() << "Stratagem" << comboBoxCounter << "changed to" << stratagemList[index].name_ << "that has code" << stratagemList[index].code_;

			QVariant data = stratagemComboBox->itemData(index);
			if (!data.isValid()) {
				qDebug() << "No valid item data found in combobox" << index;
				return;
			}

			QString stratagemCode = data.toString();
			qDebug() << "Combobox data" << stratagemCode;

			if (stratagemCode.isNull() || stratagemCode.isEmpty())
			{
				qDebug() << "Empty stratagem code, nothing to do here";
				return;
			}

			QSettings settings("stdnullptr", "StrataBro");
			settings.setValue(QString("comboIndex_%1").arg(comboBoxCounter), index);

			const int hotkey = comboBoxCounter + 1;
			UnregisterHotKey((HWND)this->window()->winId(), hotkey);
			RegisterHotKey(
				(HWND)this->window()->winId(),
				hotkey,
				MOD_CONTROL,
				'0' + hotkey); // hack, properly retrieve the vkCode one day instead of this ASCII building
			});

		for (qsizetype stratagemCounter = 0; stratagemCounter < stratagemList.size(); stratagemCounter++)
		{
			const Stratagem& stratagem = stratagemList[stratagemCounter];
			stratagemComboBox->addItem(stratagem.name_, stratagem.code_);
		}

		QSettings settings("stdnullptr", "StrataBro");
		int savedIndex = settings.value(QString("comboIndex_%1").arg(comboBoxCounter), 0).toInt();
		if (savedIndex >= 0 && savedIndex < stratagemComboBox->count()) {
			stratagemComboBox->setCurrentIndex(savedIndex);
		}
	}
}

MainWindow::~MainWindow()
{}

bool MainWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
	MSG* msg = static_cast<MSG*>(message);

	if (msg->message == WM_HOTKEY) {
		int hotkeyId = static_cast<int>(msg->wParam); // Get hotkey ID
		DWORD vk = HIWORD(msg->lParam); // Virtual key code

		qDebug() << "Hotkey Pressed! ID:" << hotkeyId << "Virtual Key:" << vk;

		if (hotkeyId >= 1 && hotkeyId <= MAX_STRATAGEM_HOTKEYS) {

			// -1 because they have zero-based names but the keybinds start from 1
			QComboBox* stratagemComboBox = findChild<QComboBox*>(QString("stratagemComboBox_%1").arg(hotkeyId - 1));
			if (!stratagemComboBox) {
				qDebug() << "No combo box found for hotkey" << hotkeyId;
				return false;
			}

			QVariant data = stratagemComboBox->currentData();
			QString code = data.toString();

			qDebug() << "Executing stratagem for Hotkey ID =" << hotkeyId << ", code=" << code;

			QList<int> keySequence = Utils::TransformDirectionStringSequenceToVkCodes(code);
			qDebug() << "Key sequence that will be pressed:" << keySequence;

			Utils::PressKeys(keySequence, ui.dialSpeedSpinBox->value());
		}
		return true;
	}
	return false;
}
