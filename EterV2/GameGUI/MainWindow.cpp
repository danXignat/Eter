#include "MainWindow.h"

#include <ranges>
#include "utils.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Eter");
    setWindowIcon(QIcon(ICON_PATH));

    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    stackedWidget = new QStackedWidget(this);

    requestNameScene    = new RequestNameScene();
    selectModeScene     = new SelectModeScene();
    specialPlaysScene = new SpecialPlaysScene(this);
    pauseMenuScene      = new PauseMenuScene(this);
    pauseMenuScene->hide();

    stackedWidget->addWidget(requestNameScene);
    stackedWidget->addWidget(selectModeScene);
    stackedWidget->addWidget(specialPlaysScene);

    setCentralWidget(stackedWidget);

    connect(requestNameScene,   &RequestNameScene::nameEntered,         this, &MainWindow::onNameEntered);
    connect(selectModeScene,    &SelectModeScene::gameModeSelected,     this, &MainWindow::onGameModeSelected);
    connect(selectModeScene,    &SelectModeScene::backClicked,          this, &MainWindow::onGameModeSelectionBack);
    connect(pauseMenuScene,     &PauseMenuScene::continueGameRequested, this, &MainWindow::onResumeGame);
    connect(pauseMenuScene,     &PauseMenuScene::gameRematch,           this, &MainWindow::onGameRematch);
    connect(pauseMenuScene,     &PauseMenuScene::mainMenu,              this, &MainWindow::onMainMenuSwitch);
    connect(specialPlaysScene,  &SpecialPlaysScene::continueToGame,     this, &MainWindow::onSpecialPlaysCompleted);
    connect(specialPlaysScene,  &SpecialPlaysScene::backClicked,        this, &MainWindow::onSpecialPlaysBack);


    playerBlueNameGlobal = "BLUE";
    playerRedNameGlobal = "RED";
    selectedMode = "3";
    GameModeConfig config;
    config.id = selectedMode;

    gameScene = new GameScene(playerBlueNameGlobal, playerRedNameGlobal, config);
    stackedWidget->addWidget(gameScene);
    stackedWidget->setCurrentWidget(gameScene);
}

void MainWindow::onGameModeSelectionBack() {
    stackedWidget->setCurrentWidget(requestNameScene);
}

void MainWindow::onSpecialPlaysBack() {
    stackedWidget->setCurrentWidget(selectModeScene);
}

void MainWindow::onMainMenuSwitch() {
    if (gameScene != nullptr) {
        gameScene->disconnect();
        stackedWidget->removeWidget(gameScene);
        delete gameScene;
        gameScene = nullptr;
    }
    
    pauseMenuScene->hide();
    stackedWidget->setCurrentWidget(requestNameScene);
}

void MainWindow::onGameRematch() {
    if (gameScene != nullptr) {
        gameScene->disconnect();
        stackedWidget->removeWidget(gameScene);
        delete gameScene;
        gameScene = nullptr;
    }

    pauseMenuScene->hide();
    gameScene = new GameScene(playerBlueNameGlobal, playerRedNameGlobal, m_curr_game_config);
    connect(gameScene, &GameScene::mainMenu, this, &MainWindow::onMainMenuSwitch);
    stackedWidget->addWidget(gameScene);
    stackedWidget->setCurrentWidget(gameScene);
}

void MainWindow::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

void MainWindow::onNameEntered(const QString& playerBlueName, const QString& playerRedName) {
    playerBlueNameGlobal = playerBlueName;
    playerRedNameGlobal = playerRedName;
    stackedWidget->setCurrentIndex(1);
}


void MainWindow::onModeSelected(const std::string& mode) {
    
    
}

void MainWindow::onSpecialPlaysCompleted(GameModeConfig config) {
    if (gameScene != nullptr) {
        gameScene->disconnect();
        stackedWidget->removeWidget(gameScene);
        delete gameScene;
        gameScene = nullptr;
    }

    m_curr_game_config = config;


    gameScene = new GameScene(playerRedNameGlobal, playerBlueNameGlobal, config);

    connect(gameScene, &GameScene::mainMenu, this, &MainWindow::onMainMenuSwitch);

    stackedWidget->addWidget(gameScene);
    stackedWidget->setCurrentWidget(gameScene);
}


void MainWindow::onResumeGame() {
    if (pauseMenuScene->isVisible()) {
        pauseMenuScene->hide();
    }

    if (lastScene != nullptr) {
        stackedWidget->setCurrentWidget(lastScene);
        qDebug() << "Resumed to the last scene.";
    }
    else {
        stackedWidget->setCurrentWidget(selectModeScene); 
        qDebug() << "No last scene found. Resumed to SelectModeScene.";
    }
}

void MainWindow::onGameModeSelected(const QString& gameMode) {
    selectedMode = gameMode.toStdString();

    specialPlaysScene->resetFields();
    specialPlaysScene->setSelectedGameMode(gameMode);

    stackedWidget->setCurrentWidget(specialPlaysScene);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    bool is_game_scene{ dynamic_cast<GameScene*>(stackedWidget->currentWidget()) != nullptr};

    if (event->key() == Qt::Key_Escape && is_game_scene) {
        if (!pauseMenuScene->isVisible()) {
            
            lastScene = stackedWidget->currentWidget();

            pauseMenuScene->setGeometry(0, 0, this->width(), this->height());

            pauseMenuScene->show();
        }
        else {
            onResumeGame();
        }
    }
    else {
        QMainWindow::keyPressEvent(event);
    }
}