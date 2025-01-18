#pragma once
#ifndef PAUSEMENUDIALOG_H
#define PAUSEMENUDIALOG_H

#include <QWidget>
#include <QPushButton>

class PauseMenuScene : public QWidget {
    Q_OBJECT

public:
    explicit PauseMenuScene(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event);
signals:
    void continueGameRequested();

private slots:
    void onContinueClicked();
    void onExitClicked();

private:
    QPushButton* continueButton;
    QPushButton* exitButton;
};

#endif
