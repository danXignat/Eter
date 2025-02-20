#pragma once
#include "qt_includes.h"

#include "controls.h"
#include "GameController.h"
#include "utils.h"

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
    static QString lineEditStyle;

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
    void backClicked();

private slots:

private:
    QPushButton* trainingButton;
    QPushButton* mageDuelButton;
    QPushButton* elementalBattleButton;
    QPushButton* tournamentModeButton;
    QPushButton* timedModeButton;

    QPushButton* backButton;
    QPixmap background;
};

///-------------------------------------------------------------------

class SpecialPlaysScene : public QWidget {
    Q_OBJECT

public:
    explicit SpecialPlaysScene(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;
    void setSelectedGameMode(const QString& gamemode);
    void resetFields();

signals:
    void continueToGame(GameModeConfig config);
    void backClicked();

private slots:
    void onNextClicked();

private:
    static QString checkBoxStyle;
    static QString comboBoxStyle;
    QString m_selected_gamemode;

    QPushButton* nextButton;
    QPushButton* backButton;

    QCheckBox* illusion_check_box;
    QCheckBox* explosion_check_box;
    QCheckBox* mage_check_box;
    QCheckBox* elemental_check_box;
    QCheckBox* arena_check_box;
    QComboBox* gameSizeCombo;
    QComboBox* gameTimeCombo;

    QLabel* infoLabel;
    QPixmap background;
};

///-------------------------------------------------------------------

class GameScene : public QWidget {
    Q_OBJECT

signals:
    void mainMenu();

private:
    bool isIllusionEnabled = false;
    QPushButton* toggleButton;
public:
    explicit GameScene(const QString& playerBlueName, const QString& playerRedName,
        const GameModeConfig& config,  QWidget* parent = nullptr);

private:
    GameController* controller;

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
