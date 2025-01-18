#pragma once
#ifndef PAUSEMENUDIALOG_H
#define PAUSEMENUDIALOG_H

#include <QDialog>
#include <QPushButton>

class PauseMenuDialog : public QDialog {
    Q_OBJECT

public:
    explicit PauseMenuDialog(QWidget* parent = nullptr);

private slots:
    void onContinueClicked();
    void onExitClicked();

private:
    QPushButton* continueButton;
    QPushButton* exitButton;
};

#endif
