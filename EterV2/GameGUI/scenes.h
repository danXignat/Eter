#pragma once
#include "qt_includes.h"

#include "controls.h"
#include "GameController.h"

class RequestNameScene : public QWidget {
    Q_OBJECT

public:
    explicit RequestNameScene(QWidget* parent = nullptr);

signals:
    void nameEntered(const QString& bluePlayer, const QString& redPlayer);

private slots:
    void onNextClicked();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QLineEdit* playerBlueNameInput;
    QLineEdit* playerRedNameInput;
    NextButton nextButton;
    QPixmap background;
};

///-------------------------------------------------------------------

class SelectModeScene : public QWidget {
    Q_OBJECT

public:
    explicit SelectModeScene(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

signals:
    void modeSelected(const std::string& mode);
    void gameModeSelected(const QString& gameMode);
private slots:

private:
    QPushButton* trainingButton;
    QPushButton* mageDuelButton;
    QPushButton* elementalBattleButton;
    QPushButton* tournamentModeButton;
    QPushButton* timedModeButton;
    QPixmap background;
};

///-------------------------------------------------------------------

class SpecialPlaysScene : public QWidget {
    Q_OBJECT

public:
    explicit SpecialPlaysScene(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

signals:
    void continueToGame(const std::string&);

private slots:
    void onNextClicked();

private:
    QPushButton* nextButton;
    QCheckBox* illusions;
    QCheckBox* explosions;
    QLabel* infoLabel;
    QPixmap background;
};

///-------------------------------------------------------------------

class GameScene : public QWidget {
    Q_OBJECT
private:
    bool isIllusionEnabled = false;
    QPushButton* toggleButton;
public:
    explicit GameScene(const std::string& mode, const QString& playerBlueName, const QString& playerRedName, QWidget* parent = nullptr);

private:
    GameController controller;

private slots:
    void toggleIllusion();
};

///-------------------------------------------------------------------

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