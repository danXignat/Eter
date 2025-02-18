#include "scenes.h"

#include "settings.h"

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
    nextButton->move(WINDOW_WIDTH - 385, WINDOW_HEIGHT - 122);
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

///-------------------------------------------------------------------

SelectModeScene::SelectModeScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    trainingButton = new QPushButton("Training", this);
    trainingButton->setFixedSize(200, 50);
    trainingButton->setObjectName("myCustomButton");
    trainingButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/button.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/button_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/button_clicked.png');"
        "}"
    );
    layout->addWidget(trainingButton, 0, Qt::AlignCenter);

    mageDuelButton = new QPushButton("Mage-Duel", this);
    mageDuelButton->setFixedSize(200, 50);
    mageDuelButton->setObjectName("myCustomButton");
    mageDuelButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/button.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/button_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/button_clicked.png');"
        "}"
    );
    layout->addWidget(mageDuelButton, 0, Qt::AlignCenter);

    elementalBattleButton = new QPushButton("Elemental Battle", this);
    elementalBattleButton->setFixedSize(200, 50);
    elementalBattleButton->setObjectName("myCustomButton");
    elementalBattleButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/button.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/button_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/button_clicked.png');"
        "}"
    );
    layout->addWidget(elementalBattleButton, 0, Qt::AlignCenter);

    tournamentModeButton = new QPushButton("Tournament Battle", this);
    tournamentModeButton->setFixedSize(200, 50);
    tournamentModeButton->setObjectName("myCustomButton");
    tournamentModeButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/button.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/button_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/button_clicked.png');"
        "}"
    );
    layout->addWidget(tournamentModeButton, 0, Qt::AlignCenter);

    timedModeButton = new QPushButton("Timed Battle", this);
    timedModeButton->setFixedSize(200, 50);
    timedModeButton->setObjectName("myCustomButton");
    timedModeButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/button.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/button_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/button_clicked.png');"
        "}"
    );
    layout->addWidget(timedModeButton, 0, Qt::AlignCenter);
    background.load("../pictures/estetics/name_select.png");

    connect(trainingButton, &QPushButton::clicked, this, [=]() { emit gameModeSelected("1"); });
    connect(mageDuelButton, &QPushButton::clicked, this, [=]() { emit gameModeSelected("2"); });
    connect(elementalBattleButton, &QPushButton::clicked, this, [=]() { emit gameModeSelected("3"); });

}

void SelectModeScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

///-------------------------------------------------------------------

SpecialPlaysScene::SpecialPlaysScene(QWidget* parent)
    : QWidget(parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    infoLabel = new QLabel("Choose your special play options:", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 48px; font-weight: bold;");
    mainLayout->addWidget(infoLabel, 0, Qt::AlignTop | Qt::AlignCenter);

    QVBoxLayout* checkBoxLayout = new QVBoxLayout();
    illusions = new QCheckBox("Enable illusions", this);
    illusions->setStyleSheet(
        "QCheckBox {"
        "    background-color: #1b3d50;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    padding: 2px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-color: #466876;"
        "}"
        "QCheckBox:checked {"
        "    background-color: #466876;"
        "}"
    );
    illusions->setGeometry(530, 300, 150, 50);

    explosions = new QCheckBox("Enable explosions", this);
    explosions->setStyleSheet(
        "QCheckBox {"
        "    background-color: #1b3d50;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    padding: 2px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-color: #466876;"
        "}"
        "QCheckBox:checked {"
        "    background-color: #466876;"
        "}"
    );
    explosions->setGeometry(530, 400, 150, 50);

    nextButton = new NextButton(this);
    nextButton->setFixedSize(365, 92);
    nextButton->move(WINDOW_WIDTH - 385, WINDOW_HEIGHT - 122);
    background.load("../pictures/estetics/name_select.png");

    connect(nextButton, &QPushButton::clicked, this, &SpecialPlaysScene::onNextClicked);
}

void SpecialPlaysScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

void SpecialPlaysScene::onNextClicked() {
    std::string special_modes;

    special_modes += illusions->isChecked() ? '1' : '0';
    special_modes += explosions->isChecked() ? '2' : '0';

    qDebug() << "Full mode selected:" << special_modes;

    emit continueToGame(special_modes);
}

///-------------------------------------------------------------------

GameScene::GameScene(const std::string& mode, const QString& playerBlueName, const QString& playerRedName, QWidget* parent) :
    QWidget(parent),
    controller{ this, mode, playerRedName, playerBlueName }
{}

void GameScene::toggleIllusion() {
    isIllusionEnabled = !isIllusionEnabled;
    toggleButton->setText(isIllusionEnabled ? "Illusion Enabled" : "Illusion Disabled");
}

///-------------------------------------------------------------------

PauseMenuScene::PauseMenuScene(QWidget* parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint) {
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setStyleSheet("background-color: rgba(0, 0, 0, 180);");

    continueButton  = new QPushButton("Continue"    , this);
    rematchButton   = new QPushButton("Rematch"     , this);
    mainMenuButton  = new QPushButton("Main Menu"   , this);
    exitButton      = new QPushButton("Exit"        , this);

    QString buttonStyle = "font-size: 18px; text-align: center; padding: 15px; min-width: 200px;";
    continueButton  ->setStyleSheet(buttonStyle);
    rematchButton   ->setStyleSheet(buttonStyle);
    mainMenuButton  ->setStyleSheet(buttonStyle);
    exitButton      ->setStyleSheet(buttonStyle);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(30);

    layout->addWidget(continueButton);
    layout->addWidget(rematchButton);
    layout->addWidget(mainMenuButton);
    layout->addWidget(exitButton);

    connect(continueButton, &QPushButton::clicked, this, &PauseMenuScene::onContinueClicked);
    connect(exitButton,     &QPushButton::clicked, this, &PauseMenuScene::onExitClicked);
    connect(rematchButton,  &QPushButton::clicked, this, &PauseMenuScene::gameRematch);
    connect(mainMenuButton, &QPushButton::clicked, this, &PauseMenuScene::mainMenu);
}

void PauseMenuScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    QColor backgroundColor(0, 0, 0, 180); // Black with alpha 180 (semitransparent)
    QRect backgroundRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // x, y, width, height
    painter.fillRect(backgroundRect, backgroundColor);

    QWidget::paintEvent(event);
}

void PauseMenuScene::onContinueClicked() {
    emit continueGameRequested();
}

void PauseMenuScene::onExitClicked() {
    QApplication::quit();
}
