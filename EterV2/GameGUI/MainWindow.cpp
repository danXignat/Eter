#include "MainWindow.h"
#include "PauseMenuDialog.h"

#include <ranges>
#include "utils.h"


RequestNameScene::RequestNameScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    playerRedNameInput = new QLineEdit(this);
    playerRedNameInput->setPlaceholderText("Enter Player 2 Name");
    playerRedNameInput->setText("Player RED");
    playerRedNameInput->setFixedSize(300, 50);
    playerRedNameInput->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(playerRedNameInput, 0, Qt::AlignCenter); 

    playerBlueNameInput = new QLineEdit(this);
    playerBlueNameInput->setPlaceholderText("Enter Player 1 Name");
    playerBlueNameInput->setText("Player BLUE");
    playerBlueNameInput->setFixedSize(300, 50); 
    playerBlueNameInput->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(playerBlueNameInput, 0, Qt::AlignCenter);

    background.load("../pictures/estetics/name_select.png");
    NextButton* nextButton = new NextButton(this);
    nextButton->setFixedSize(365, 92);
    nextButton->move(WINDOW_WIDTH-385, WINDOW_HEIGHT-122);
    nextButton->raise();
    nextButton->show();
    nextButton->setMouseTracking(true);
    connect(nextButton, &QPushButton::clicked, this, &RequestNameScene::onNextClicked);
}

void RequestNameScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

void RequestNameScene::onNextClicked() {
    emit nameEntered(playerRedNameInput->text(), playerBlueNameInput->text());
}

SelectModeScene::SelectModeScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    trainingButton = new QPushButton("Training", this);
    trainingButton->setFixedSize(200, 50); 
    trainingButton->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(trainingButton, 0, Qt::AlignCenter);

    mageDuelButton = new QPushButton("Mage-Duel", this);
    mageDuelButton->setFixedSize(200, 50); 
    mageDuelButton->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(mageDuelButton, 0, Qt::AlignCenter); 

    elementalBattleButton = new QPushButton("Elemental Battle", this);
    elementalBattleButton->setFixedSize(200, 50);
    elementalBattleButton->setStyleSheet("font-size: 18px;");
    layout->addWidget(elementalBattleButton, 0, Qt::AlignCenter);

    connect(trainingButton, &QPushButton::clicked, this, [=]() { emit modeSelected("102"); });
    connect(mageDuelButton, &QPushButton::clicked, this, [=]() { emit modeSelected("202"); });
    connect(elementalBattleButton, &QPushButton::clicked, this, [=]() { emit modeSelected("302"); });
}

GameScene::GameScene(const std::string& mode, const QString& playerBlueName, const QString& playerRedName, QWidget* parent) :
    QWidget(parent),
    controller{ this, mode, playerRedName, playerBlueName }
{}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("Eter");
    setWindowIcon(QIcon(ICON_PATH));

    resize(WINDOW_WIDTH, WINDOW_HEIGHT);

    stackedWidget = new QStackedWidget(this);

    requestNameScene = new RequestNameScene();
    selectModeScene = new SelectModeScene();
    pauseMenuScene = new PauseMenuScene();
    /*gameScene = new GameScene();*/

    // Add scenes to the QStackedWidget
    stackedWidget->addWidget(requestNameScene); // Index 0
    stackedWidget->addWidget(selectModeScene);// Index 1
    //stackedWidget->addWidget(gameScene);// Index 2
    stackedWidget->addWidget(pauseMenuScene);
            

    setCentralWidget(stackedWidget);

    connect(requestNameScene, &RequestNameScene::nameEntered, this, &MainWindow::onNameEntered);
    connect(selectModeScene, &SelectModeScene::modeSelected, this, &MainWindow::onModeSelected);
    connect(pauseMenuScene, &PauseMenuScene::continueGameRequested, this, &MainWindow::onResumeGame);
}

void MainWindow::onNameEntered(const QString& playerBlueName, const QString& playerRedName) {
    playerBlueNameGlobal = playerBlueName;
    playerRedNameGlobal = playerRedName;
    stackedWidget->setCurrentIndex(1);
}


void MainWindow::onModeSelected(const std::string& mode) {
    selectedMode = mode; 
    if (gameScene != nullptr) {
        stackedWidget->removeWidget(gameScene);
        delete gameScene;
        gameScene = nullptr;
    }
    gameScene = new GameScene(mode, playerRedNameGlobal, playerBlueNameGlobal);
    stackedWidget->addWidget(gameScene);
    stackedWidget->setCurrentWidget(gameScene);
    qDebug() << "Switched to GameScene with mode:" << QString::fromStdString(mode);
}

//void MainWindow::showPauseMenu() {
//    QDialog pauseDialog(this);
//    pauseDialog.setWindowTitle("Pause Menu");
//    pauseDialog.resize(300, 200);
//
//    QPushButton* continueButton = new QPushButton("Continue Playing", &pauseDialog);
//    QPushButton* exitButton = new QPushButton("Exit", &pauseDialog);
//
//    QVBoxLayout* layout = new QVBoxLayout(&pauseDialog);
//    layout->addWidget(continueButton);
//    layout->addWidget(exitButton);
//
//    exitButton->setDefault(true);
//    exitButton->setFocus();
//
//    continueButton->setMinimumSize(100, 50);
//    exitButton->setMinimumSize(150, 50);
//
//    connect(continueButton, &QPushButton::clicked, &pauseDialog, &QDialog::accept);
//    connect(exitButton, &QPushButton::clicked, qApp, &QApplication::quit);
//
//    pauseDialog.exec();
//}

void MainWindow::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Escape) {
        if (stackedWidget->currentWidget() != pauseMenuScene) {
            // Store the current scene before switching to the pause menu
            lastScene = stackedWidget->currentWidget();
            stackedWidget->setCurrentWidget(pauseMenuScene);
        }
        else {
            // Return to the last active scene
            if (lastScene != nullptr) {
                stackedWidget->setCurrentWidget(lastScene);
                lastScene = nullptr; // Reset lastScene to avoid stale references
            }
            else {
                // Fallback to a default scene if lastScene is not set
                if (gameScene != nullptr) {
                    stackedWidget->setCurrentWidget(gameScene);
                }
                else {
                    stackedWidget->setCurrentWidget(selectModeScene);
                }
            }
        }
    }
    else {
        QMainWindow::keyPressEvent(event);
    }
}


void MainWindow::onResumeGame() {
    if (lastScene != nullptr) {
        stackedWidget->setCurrentWidget(lastScene); // Return to the last scene
        lastScene = nullptr; // Clear the lastScene reference
        qDebug() << "Returned to the last scene.";
    }
    else {
        // Fallback logic in case lastScene is null
        qDebug() << "Error: lastScene is null. Returning to default.";
        stackedWidget->setCurrentWidget(selectModeScene); // Default fallback
    }
}
NextButton::NextButton(QWidget* parent )
    : QPushButton(parent), currentPixmap("../pictures/estetics/next_normal.png") {
    setFixedSize(currentPixmap.size());
}

void NextButton::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, currentPixmap);
}

void NextButton::enterEvent(QEnterEvent* event) {
    qDebug() << "Hover started";

    currentPixmap.load("../pictures/estetics/next_hover.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qDebug() << currentPixmap.size();
    update();
    QPushButton::enterEvent(event);
}

void NextButton::leaveEvent(QEvent* event){
    qDebug() << "Hover ended";
    currentPixmap.load("../pictures/estetics/next_normal.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
    QPushButton::leaveEvent(event);
}

void NextButton::mousePressEvent(QMouseEvent* event){
    
    currentPixmap.load("../pictures/estetics/next_click.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
    QPushButton::mousePressEvent(event);
}

void NextButton::mouseReleaseEvent(QMouseEvent* event){

    currentPixmap.load("../pictures/estetics/next_normal.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    
    update();
    QPushButton::mouseReleaseEvent(event);
}
bool NextButton::event(QEvent* event) {
    if (event->type() == QEvent::Leave) {
        qDebug() << "Generic Leave event detected";
    }
    return QPushButton::event(event); // Pass to the base class
}
