#include "MainWindow.h"

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

    nextButton = new QPushButton("Next", this);
    nextButton->setFixedSize(200, 50);
    nextButton->setStyleSheet("font-size: 18px;"); 
    layout->addWidget(nextButton, 0, Qt::AlignCenter); 

    connect(nextButton, &QPushButton::clicked, this, &RequestNameScene::onNextClicked);
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

    connect(trainingButton, &QPushButton::clicked, this, [=]() { emit modeSelected("100"); });
    connect(mageDuelButton, &QPushButton::clicked, this, [=]() { emit modeSelected("200"); });
    connect(elementalBattleButton, &QPushButton::clicked, this, [=]() { emit modeSelected("300"); });
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
    /*gameScene = new GameScene();*/

    // Add scenes to the QStackedWidget
    stackedWidget->addWidget(requestNameScene); // Index 0
    stackedWidget->addWidget(selectModeScene);  // Index 1
    //stackedWidget->addWidget(gameScene);        // Index 2

    setCentralWidget(stackedWidget);

    connect(requestNameScene, &RequestNameScene::nameEntered, this, &MainWindow::onNameEntered);
    connect(selectModeScene, &SelectModeScene::modeSelected, this, &MainWindow::onModeSelected);
}

void MainWindow::onNameEntered(const QString& playerBlueName, const QString& playerRedName) {
    playerBlueNameGlobal = playerBlueName;
    playerRedNameGlobal = playerRedName;
    stackedWidget->setCurrentIndex(1);
}


void MainWindow::onModeSelected(const std::string& mode) {
    selectedMode = mode; 
    gameScene = new GameScene(mode, playerRedNameGlobal, playerBlueNameGlobal);
    stackedWidget->addWidget(gameScene);
    stackedWidget->setCurrentIndex(2); 
}