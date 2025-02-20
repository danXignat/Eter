#include "scenes.h"

#include "settings.h"

RequestNameScene::RequestNameScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    playerRedNameInput = new QLineEdit(this);
    playerRedNameInput->setPlaceholderText("Enter Player 1 Name");
    playerRedNameInput->setFixedSize(300, 50);
    playerRedNameInput->setGeometry(500, 250, 200, 90);
    playerRedNameInput->setStyleSheet(lineEditStyle);

    playerBlueNameInput = new QLineEdit(this);
    playerBlueNameInput->setPlaceholderText("Enter Player 2 Name");
    playerBlueNameInput->setFixedSize(300, 50);
    playerBlueNameInput->setGeometry(500, 450, 200, 90);    
    playerBlueNameInput->setStyleSheet(lineEditStyle);

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
    QString playe1_name, player2_name;

    if (playerRedNameInput->text().isEmpty()) {
        playe1_name = "RED";
    }
    else {
        playe1_name = playerRedNameInput->text();
    }

    if (playerBlueNameInput->text().isEmpty()) {
        player2_name = "BLUE";
    }
    else {
        player2_name = playerBlueNameInput->text();
    }

    emit nameEntered(playe1_name, player2_name);
}

QString RequestNameScene::lineEditStyle{
"QLineEdit {"
"    font-family: 'Luminari', 'Trattatello';"
"    font-size: 20px;"
"    color: #00A800;"
"    background-color: rgba(0, 0, 0, 0.7);"
"    border: 1px solid #00A800;"
"    border-radius: 8px;"
"    padding: 5px 15px;"
"    min-width: 200px;"
"}"
"QLineEdit:hover {"
"    border: 1px solid #00FF00;"
"    color: #00FF00;"
"}"
"QLineEdit:focus {"
"    border: 1px solid #00FF00;"
"    color: #00FF00;"
"    background-color: rgba(0, 0, 0, 0.9);"
"}"
"QLineEdit::selection {"
"    background-color: #00A800;"
"    color: black;"
"}" };

///-------------------------------------------------------------------

SelectModeScene::SelectModeScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 50, 20, 30);

    QWidget* gameModeWidget = new QWidget(this);
    QVBoxLayout* gameModeLayout = new QVBoxLayout(gameModeWidget);
    gameModeLayout->setAlignment(Qt::AlignCenter);
    gameModeLayout->setSpacing(30);

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
    gameModeLayout->addWidget(trainingButton, 0, Qt::AlignCenter);

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
    gameModeLayout->addWidget(mageDuelButton, 0, Qt::AlignCenter);

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
    gameModeLayout->addWidget(elementalBattleButton, 0, Qt::AlignCenter);

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
    gameModeLayout->addWidget(tournamentModeButton, 0, Qt::AlignCenter);

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
    gameModeLayout->addWidget(timedModeButton, 0, Qt::AlignCenter);

    layout->addWidget(gameModeWidget, 1);  // Give it a stretch factor of 1

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    backButton = new BackButton(this);
    backButton->setFixedSize(365, 92);
    buttonLayout->addWidget(backButton);
    buttonLayout->addStretch();

    // Add button layout to main layout
    layout->addLayout(buttonLayout);

    background.load("../pictures/estetics/name_select.png");

    connect(trainingButton,         &QPushButton::clicked, this, [=]() { emit gameModeSelected("1"); });
    connect(mageDuelButton,         &QPushButton::clicked, this, [=]() { emit gameModeSelected("2"); });
    connect(elementalBattleButton,  &QPushButton::clicked, this, [=]() { emit gameModeSelected("3"); });
    connect(tournamentModeButton,   &QPushButton::clicked, this, [=]() { emit gameModeSelected("4"); });
    connect(timedModeButton,        &QPushButton::clicked, this, [=]() { emit gameModeSelected("5"); });
    connect(backButton, &QPushButton::clicked, this, &SelectModeScene::backClicked);

}

void SelectModeScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

///-------------------------------------------------------------------

SpecialPlaysScene::SpecialPlaysScene(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    QVBoxLayout* checkBoxLayout = new QVBoxLayout();
    checkBoxLayout->setAlignment(Qt::AlignCenter);
    checkBoxLayout->setSpacing(20);

    gameSizeCombo = new QComboBox(this);
    gameSizeCombo->addItem("Select game size");  // Default option
    gameSizeCombo->addItem("Small");
    gameSizeCombo->addItem("Big");
    gameSizeCombo->setCurrentIndex(0);  // Set to default option

    gameTimeCombo = new QComboBox(this);
    gameTimeCombo->addItem("Select time");  // Default option
    gameTimeCombo->addItem("60");
    gameTimeCombo->addItem("90");
    gameTimeCombo->addItem("120");
    gameTimeCombo->setCurrentIndex(0);  // Set to default option

    illusion_check_box = new QCheckBox("Illusions", this);
    explosion_check_box = new QCheckBox("Explosions", this);
    mage_check_box = new QCheckBox("Mages", this);
    elemental_check_box = new QCheckBox("Powers", this);
    arena_check_box = new QCheckBox("Arena", this);

    gameSizeCombo->setStyleSheet(comboBoxStyle);
    gameTimeCombo->setStyleSheet(comboBoxStyle);
    illusion_check_box->setStyleSheet(checkBoxStyle);
    explosion_check_box->setStyleSheet(checkBoxStyle);
    mage_check_box->setStyleSheet(checkBoxStyle);
    elemental_check_box->setStyleSheet(checkBoxStyle);
    arena_check_box->setStyleSheet(checkBoxStyle);

    checkBoxLayout->addWidget(gameSizeCombo);
    checkBoxLayout->addWidget(gameTimeCombo);
    checkBoxLayout->addWidget(illusion_check_box);
    checkBoxLayout->addWidget(explosion_check_box);
    checkBoxLayout->addWidget(mage_check_box);
    checkBoxLayout->addWidget(elemental_check_box);
    checkBoxLayout->addWidget(arena_check_box);

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setContentsMargins(20, 0, 20, 30);

    backButton = new BackButton(this);
    backButton->setFixedSize(365, 92);
    buttonLayout->addWidget(backButton);

    // Add stretch in the middle to push buttons to opposite sides
    buttonLayout->addStretch();

    nextButton = new NextButton(this);
    nextButton->setFixedSize(365, 92);
    buttonLayout->addWidget(nextButton);

    mainLayout->addSpacing(50);
    mainLayout->addWidget(infoLabel);
    mainLayout->addStretch();
    mainLayout->addLayout(checkBoxLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    background.load("../pictures/estetics/name_select.png");
    connect(nextButton, &QPushButton::clicked, this, &SpecialPlaysScene::onNextClicked);
    connect(backButton, &QPushButton::clicked, this, &SpecialPlaysScene::backClicked);
}

void SpecialPlaysScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    painter.drawPixmap(0, 0, width(), height(), background);

    QWidget::paintEvent(event);
}

void SpecialPlaysScene::onNextClicked() {
    bool is_timed = m_selected_gamemode == "5";
    bool unselected_combo = gameSizeCombo->currentIndex() == 0 || gameTimeCombo->currentIndex() == 0;
    if (is_timed && unselected_combo) {
        return;
    }

    m_selected_gamemode += illusion_check_box->isChecked()  ? "1" : "";
    m_selected_gamemode += explosion_check_box->isChecked() ? "2" : "";
    m_selected_gamemode += mage_check_box->isChecked()      ? "3" : "";
    m_selected_gamemode += elemental_check_box->isChecked() ? "4" : "";
    m_selected_gamemode += arena_check_box->isChecked()     ? "5" : "";
    
    GameModeConfig config;
    config.id = m_selected_gamemode.toStdString();
    if (is_timed) {
        config.size_type = (gameSizeCombo->currentText() == "Small") ? base::GameSizeType::SMALL : base::GameSizeType::BIG;
        config.time = gameTimeCombo->currentText().toUShort();
    }

    emit continueToGame(config);
}

void SpecialPlaysScene::resetFields() {
    gameSizeCombo->setCurrentIndex(0);
    gameTimeCombo->setCurrentIndex(0);
    illusion_check_box->setChecked(false);
    explosion_check_box->setChecked(false);
    mage_check_box->setChecked(false);
    elemental_check_box->setChecked(false);
    arena_check_box->setChecked(false);
}

void SpecialPlaysScene::setSelectedGameMode(const QString& gamemode) {
    m_selected_gamemode = gamemode;

    switch (gamemode.front().toLatin1())
    {
    case '1': {
        gameSizeCombo->hide();
        mage_check_box->hide();
        elemental_check_box->hide();
        arena_check_box->hide();
        gameTimeCombo->hide();
        break;
    }
    
    case '2': {
        gameSizeCombo->hide();
        mage_check_box->hide();
        elemental_check_box->hide();
        arena_check_box->hide();
        gameTimeCombo->hide();
        break;
    }

    case '3': {
        gameSizeCombo->hide();
        mage_check_box->hide();
        elemental_check_box->hide();
        arena_check_box->hide();
        gameTimeCombo->hide();
        break;
    }

    case '4': {
        gameSizeCombo->hide();
        mage_check_box->show();
        elemental_check_box->show();
        arena_check_box->hide();
        gameTimeCombo->hide();
        break;
    }

    default: {
        gameSizeCombo->show();
        mage_check_box->show();
        elemental_check_box->show();
        arena_check_box->show();
        gameTimeCombo->show();
        break;
    }
    }
}

QString SpecialPlaysScene::comboBoxStyle{
"QComboBox {"
"    font-family: 'Luminari', 'Trattatello';"
"    font-size: 20px;"
"    color: #00A800;"
"    background-color: rgba(0, 0, 0, 0.7);"
"    border: 1px solid #00A800;"
"    border-radius: 8px;"
"    padding: 5px 15px;"
"    min-width: 200px;"
"}"
"QComboBox:hover {"
"    border: 1px solid #00FF00;"
"    color: #00FF00;"
"}"
"QComboBox::drop-down {"
"    border: none;"
"    width: 30px;"
"}"
"QComboBox::down-arrow {"
"    image: url(../pictures/estetics/down_arrow.png);"  // You'll need this image
"    width: 12px;"
"    height: 12px;"
"}"
"QComboBox QAbstractItemView {"
"    background-color: rgba(0, 0, 0, 0.9);"
"    border: 1px solid #00A800;"
"    border-radius: 0px;"
"    selection-background-color: #00A800;"
"    selection-color: black;"
"    color: #00A800;"
"    outline: none;"
"}" };

QString SpecialPlaysScene::checkBoxStyle{
    "QCheckBox {"
    "    font-family: 'Luminari', 'Trattatello', 'Magic School One';"
    "    font-size: 24px;"
    "    color: #00A800;"
    "    font-weight: bold;"
    "    spacing: 12px;"
    "    padding: 8px 15px;"
    "    background-color: rgba(0, 0, 0, 0.7);"
    "    border: 1px solid rgba(0, 168, 0, 0.3);"
    "    border-radius: 8px;"
    "}"
    "QCheckBox:hover {"
    "    color: #00FF00;"
    "    background-color: rgba(0, 0, 0, 0.8);"  // Slightly darker on hover
    "    border: 1px solid rgba(0, 168, 0, 0.6);"  // More visible border on hover
    "}"
    "QCheckBox::indicator {"
    "    width: 25px;"
    "    height: 25px;"
    "    border: 2px solid #00A800;"
    "    border-radius: 5px;"
    "    background-color: rgba(0, 0, 0, 0.8);"
    "    margin-right: 5px;"  // Space between indicator and text
    "}"
    "QCheckBox::indicator:hover {"
    "    border: 2px solid #00FF00;"
    "    background-color: rgba(0, 168, 0, 0.1);"
    "}"
    "QCheckBox::indicator:checked {"
    "    background-color: #00A800;"
    "    border: 2px solid #00A800;"
    "    image: url(../pictures/estetics/checkmark.png);"
    "}"
    "QCheckBox::indicator:checked:hover {"
    "    background-color: #00CC00;"
    "    border: 2px solid #00CC00;"
    "}"
};

///-------------------------------------------------------------------

GameScene::GameScene(const QString& playerBlueName, const QString& playerRedName,
    const GameModeConfig& config, QWidget* parent) :
    QWidget(parent),
    controller{ new GameController(this, playerRedName, playerBlueName, config) }
{
    connect(controller, &GameController::mainMenuRequested, this, &GameScene::mainMenu);
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
