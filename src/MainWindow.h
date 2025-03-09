#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

protected:
    // The method for processing native events from the OS in Qt
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
    Ui::MainWindowClass ui;
    static constexpr qsizetype MAX_STRATAGEM_HOTKEYS = 8;
};
