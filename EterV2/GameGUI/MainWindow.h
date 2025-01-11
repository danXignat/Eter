#pragma once
#include "qt_includes.h"
#include "game.h"
#include "settings.h"

#include "..\EterV2\GameModeFactory.h"
#include "..\EterV2\InputHandler.h"

class RequestNameScene : public QWidget {
    Q_OBJECT

public:
    explicit RequestNameScene(QWidget* parent = nullptr);

signals:
    void nameEntered(const QString& bluePlayer, const QString& redPlayer);

private slots:
    void onNextClicked();

private:
    QLineEdit* playerBlueNameInput;
    QLineEdit* playerRedNameInput;
    QPushButton* nextButton;
};

class SelectModeScene : public QWidget {
    Q_OBJECT

public:
    explicit SelectModeScene(QWidget* parent = nullptr);

signals:
    void modeSelected(const std::string& mode);

private:
    QPushButton* trainingButton;
    QPushButton* mageDuelButton;
    QPushButton* elementalBattleButton;
};

class GameScene : public QWidget {
    Q_OBJECT

public:
    explicit GameScene(const std::string& mode, const QString& playerBlueName, const QString& playerRedName, QWidget* parent = nullptr);

private:
    GameController* controller;
};


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);

private slots:
    void onNameEntered(const QString& playerBlueName, const QString& playerRedName);
    void onModeSelected(const std::string& mode);

private:
    QStackedWidget* stackedWidget;
    RequestNameScene* requestNameScene;
    SelectModeScene* selectModeScene;
    GameScene* gameScene;

    QString playerBlueNameGlobal;
    QString playerRedNameGlobal;
    std::string selectedMode;
};

