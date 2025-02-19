#include "scenes.h"

#include "settings.h"

RequestNameScene::RequestNameScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    playerRedNameInput = new QLineEdit(this);
    playerRedNameInput->setPlaceholderText("Enter Player 2 Name");
    playerRedNameInput->setText("Player RED");
    playerRedNameInput->setFixedSize(300, 50);
    playerRedNameInput->setGeometry(500, 250, 200, 90);
    playerRedNameInput->setStyleSheet("font-size: 18px;");

    playerBlueNameInput = new QLineEdit(this);
    playerBlueNameInput->setPlaceholderText("Enter Player 1 Name");
    playerBlueNameInput->setText("Player BLUE");
    playerBlueNameInput->setFixedSize(300, 50);
    playerBlueNameInput->setGeometry(500, 450, 200, 90);
    playerBlueNameInput->setStyleSheet("font-size: 18px;");

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

    trainingButton = new QPushButton(" ", this);
    trainingButton->setFixedSize(200, 90);
    trainingButton->setObjectName("myCustomButton");
    trainingButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/trainingButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 0px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/trainingButton_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/trainingButton_clicked.png');"
        "}"
    );
    layout->addWidget(trainingButton, 0, Qt::AlignCenter);

    mageDuelButton = new QPushButton(" ", this);
    mageDuelButton->setFixedSize(200, 90);
    mageDuelButton->setObjectName("myCustomButton");
    mageDuelButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/mageduelButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/mageduelButton_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/mageduelButton_pressed.png');"
        "}"
    );
    layout->addWidget(mageDuelButton, 0, Qt::AlignCenter);

    elementalBattleButton = new QPushButton(" ", this);
    elementalBattleButton->setFixedSize(200, 90);
    elementalBattleButton->setObjectName("myCustomButton");
    elementalBattleButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/elementalButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/elementalButton_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/elementalButton_pressed.png');"
        "}"
    );
    layout->addWidget(elementalBattleButton, 0, Qt::AlignCenter);

    tournamentModeButton = new QPushButton(" ", this);
    tournamentModeButton->setFixedSize(200, 90);
    tournamentModeButton->setObjectName("myCustomButton");
    tournamentModeButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/tournamentButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/tournamentButton_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/tournamentButton_pressed.png');"
        "}"
    );
    layout->addWidget(tournamentModeButton, 0, Qt::AlignCenter);

    timedModeButton = new QPushButton(" ", this);
    timedModeButton->setFixedSize(200, 90);
    timedModeButton->setObjectName("myCustomButton");
    timedModeButton->setStyleSheet(
        "QPushButton#myCustomButton {"
        "    background-image: url('../pictures/estetics/timedButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 2px solid #5a5a5a;"
        "    border-radius: 10px;"
        "    font-size: 18px;"
        "}"
        "QPushButton#myCustomButton:hover {"
        "    background-image: url('../pictures/estetics/timedButton_hover.png');"
        "}"
        "QPushButton#myCustomButton:pressed {"
        "    background-image: url('../pictures/estetics/timedButton_pressed.png');"
        "}"
    );
    layout->addWidget(timedModeButton, 0, Qt::AlignCenter);
    background.load("../pictures/estetics/name_select.png");

    connect(trainingButton, &QPushButton::clicked, this, [=]() { emit gameModeSelected("1"); });
    connect(mageDuelButton, &QPushButton::clicked, this, [=]() { emit gameModeSelected("2"); });
    connect(elementalBattleButton, &QPushButton::clicked, this, [=]() { emit gameModeSelected("3"); });
    connect(tournamentModeButton, &QPushButton::clicked, this, [=]() {emit gameModeSelected("4"); });
    connect(timedModeButton, &QPushButton::clicked, this, [=]() {emit gameModeSelected("5"); });
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
    illusions = new QCheckBox(" ", this);
    illusions->setStyleSheet(
        "QCheckBox {"
        "    background-image: url('../pictures/estetics/illusions.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-image: url('../pictures/estetics/illusions_hover.png');"
        "}"
        "QCheckBox:checked {"
        "    background-image: url('../pictures/estetics/illusions_clicked.png');"
        "}"
    );
    illusions->setGeometry(530, 300, 200, 90);

    explosions = new QCheckBox(" ", this);
    explosions->setStyleSheet(
        "QCheckBox {"
        "    background-image: url('../pictures/estetics/explosions.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-image: url('../pictures/estetics/explosions_hover.png');"
        "}"
        "QCheckBox:checked {"
        "    background-image: url('../pictures/estetics/explosions_clicked.png');"
        "}"
    );


    explosions->setGeometry(530, 500, 200, 90);


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


TournamentPlaysScene::TournamentPlaysScene(QWidget* parent)
    : QWidget(parent) {

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    infoLabel = new QLabel("Choose your special play options:", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("font-size: 48px; font-weight: bold;");
    mainLayout->addWidget(infoLabel, 0, Qt::AlignTop | Qt::AlignCenter);

    QVBoxLayout* checkBoxLayout = new QVBoxLayout();
    illusions = new QCheckBox(" ", this);
    illusions->setStyleSheet(
        "QCheckBox {"
        "    background-image: url('../pictures/estetics/illusions.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-image: url('../pictures/estetics/illusions_hover.png');"
        "}"
        "QCheckBox:checked {"
        "    background-image: url('../pictures/estetics/illusions_clicked.png');"
        "}"
    );
    illusions->setGeometry(530, 200, 200, 90);

    explosions = new QCheckBox(" ", this);
    explosions->setStyleSheet(
        "QCheckBox {"
        "    background-image: url('../pictures/estetics/explosions.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-image: url('../pictures/estetics/explosions_hover.png');"
        "}"
        "QCheckBox:checked {"
        "    background-image: url('../pictures/estetics/explosions_clicked.png');"
        "}"
    );


    explosions->setGeometry(530, 350, 200, 90);




    mage = new QCheckBox(" ", this);
    mage->setStyleSheet(
        "QCheckBox {"
        "    background-image: url('../pictures/estetics/mageduelButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-image: url('../pictures/estetics/mageduelButton_hover.png');"
        "}"
        "QCheckBox:checked {"
        "    background-image: url('../pictures/estetics/mageduelButton_pressed.png');"
        "}"
    );
    mage->setGeometry(530, 500, 200, 90);

    elemental = new QCheckBox(" ", this);
    elemental->setStyleSheet(
        "QCheckBox {"
        "    background-image: url('../pictures/estetics/elementalButton.png');"
        "    background-repeat: no-repeat;"
        "    background-position: center;"
        "    border: 1px solid #1b444b;"
        "    border-radius: 3px;"
        "    font-size: 14px;"
        "}"
        "QCheckBox:hover {"
        "    background-image: url('../pictures/estetics/elementalButton_hover.png');"
        "}"
        "QCheckBox:checked {"
        "    background-image: url('../pictures/estetics/elementalButton_pressed.png');"
        "}"
    );
    elemental->setGeometry(530, 650, 200, 90);



    nextButton = new NextButton(this);
    nextButton->setFixedSize(365, 92);
    nextButton->move(WINDOW_WIDTH - 385, WINDOW_HEIGHT - 122);
    background.load("../pictures/estetics/name_select.png");



    connect(nextButton, &QPushButton::clicked, this, &TournamentPlaysScene::onNextClicked);
}

void TournamentPlaysScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

void TournamentPlaysScene::onNextClicked() {
    std::string special_modes;

    special_modes += illusions->isChecked() ? '1' : '0';
    special_modes += explosions->isChecked() ? '2' : '0';
    special_modes += mage->isChecked() ? '3' : '0';
    special_modes += elemental->isChecked() ? '4' : '0';
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
