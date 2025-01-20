#pragma once
#include "qt_includes.h"
#include "game.h"
#include "settings.h"
#include "PauseMenuDialog.h"
#include "..\EterV2\GameModeFactory.h"
#include "..\EterV2\InputHandler.h"

class NextButton : public QPushButton {
    Q_OBJECT
public:

    explicit NextButton(QWidget* parent = nullptr);

protected:

    void paintEvent(QPaintEvent* event) override;

    void enterEvent(QEnterEvent* event) override;

    void leaveEvent(QEvent* event);

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    bool event(QEvent* event);
private:
    QPixmap currentPixmap;
};

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
class MainWindow;
class SpecialPlaysScene : public QWidget {
    Q_OBJECT

public:
    explicit SpecialPlaysScene(MainWindow* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
signals:
    void continueToGame(const std::string&);
private slots:
    void onNextClicked();
    
private:
    MainWindow* mainWindow;
    QPushButton* nextButton;
    QCheckBox* illusions;
    QCheckBox* explosions;
    QLabel* infoLabel;
    QPixmap background;
};

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


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    std::string getSelectedMode() const;
    void paintEvent(QPaintEvent* event) override;
protected:
    void keyPressEvent(QKeyEvent* event) override;
private slots:
    void onNameEntered(const QString& playerBlueName, const QString& playerRedName);
    void onModeSelected(const std::string& mode);
    void onResumeGame();
    void onSpecialPlaysCompleted(const std::string& fullMode);
    void onGameModeSelected(const QString& gameMode);
    void onGameRematch();
    void onMainMenuSwitch();

private:
    SpecialPlaysScene* specialPlaysScene;
    QStackedWidget* stackedWidget;
    RequestNameScene* requestNameScene;
    SelectModeScene* selectModeScene;
    GameScene* gameScene = nullptr;

    PauseMenuScene* pauseMenuScene;
    QGraphicsRectItem* overlay = nullptr;
    QWidget* lastScene = nullptr;
    QString playerBlueNameGlobal;
    QString playerRedNameGlobal;
    std::string selectedMode;
    QPixmap background;
};

