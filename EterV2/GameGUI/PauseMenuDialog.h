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
    void gameRematch();
    void mainMenu();

private slots:
    void onContinueClicked();
    void onExitClicked();

private:
    QPushButton* continueButton;
    QPushButton* exitButton;
    QPushButton* rematchButton;
    QPushButton* mainMenuButton;
};

#endif
