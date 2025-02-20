#pragma once
#include "qt_includes.h"
#include "settings.h"

#include "scenes.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

protected:
    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onNameEntered(const QString& playerBlueName, const QString& playerRedName);
    void onGameModeSelected(const QString& gameMode);
    void onModeSelected(const std::string& mode);
    void onSpecialPlaysCompleted(GameModeConfig config);

    void onResumeGame();
    void onGameRematch();
    void onMainMenuSwitch();
    void onSpecialPlaysBack();
    void onGameModeSelectionBack();

private:
    QStackedWidget*     stackedWidget;
    SpecialPlaysScene*  specialPlaysScene;
    RequestNameScene*   requestNameScene;
    SelectModeScene*    selectModeScene;
    PauseMenuScene*     pauseMenuScene;

    GameScene*          gameScene = nullptr;
    QWidget*            lastScene = nullptr;
    GameModeConfig m_curr_game_config;

    QString         playerBlueNameGlobal;
    QString         playerRedNameGlobal;
    std::string     selectedMode;

    QPixmap background;
};

