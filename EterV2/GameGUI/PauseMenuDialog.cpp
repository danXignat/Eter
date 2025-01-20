#include "PauseMenuDialog.h"
#include <QVBoxLayout>
#include <QApplication>
#include "settings.h"
#include "qt_includes.h"

PauseMenuScene::PauseMenuScene(QWidget* parent)
    : QWidget(parent, Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint) {
    setWindowModality(Qt::ApplicationModal);
    setAttribute(Qt::WA_TranslucentBackground, true); // Allow transparency
    setStyleSheet("background-color: rgba(0, 0, 0, 180);"); 

    // Buttons
    continueButton = new QPushButton("Continue Playing", this);
    exitButton = new QPushButton("Exit", this);
    rematchButton = new QPushButton("Rematch", this);
    mainMenuButton = new QPushButton("Main menu", this);

    // Set custom positions and sizes
    continueButton->setGeometry(500, 200, 200, 50); // Adjust x, y, width, height
    exitButton->setGeometry(500, 350, 200, 50);
    rematchButton->setGeometry(500, 500, 200, 50);
    mainMenuButton->setGeometry(500, 650, 200, 50);

    // Optional: Set font and alignment styles
    continueButton->setStyleSheet("font-size: 18px; text-align: center;");
    exitButton->setStyleSheet("font-size: 18px; text-align: center;");
    rematchButton->setStyleSheet("font-size: 18px; text-align: center;");
    mainMenuButton->setStyleSheet("font-size: 18px; text-align: center;");

    connect(continueButton, &QPushButton::clicked, this, &PauseMenuScene::onContinueClicked);
    connect(exitButton, &QPushButton::clicked, this, &PauseMenuScene::onExitClicked);
    connect(rematchButton, &QPushButton::clicked, this, &PauseMenuScene::gameRematch);
    connect(mainMenuButton, &QPushButton::clicked, this, &PauseMenuScene::mainMenu);
}

void PauseMenuScene::paintEvent(QPaintEvent* event) {
    QPainter painter(this);

    // Draw a semitransparent rectangle
    QColor backgroundColor(0, 0, 0, 180); // Black with alpha 180 (semitransparent)
    QRect backgroundRect(-100, -100, 2000, 9000); // x, y, width, height (adjust as needed)
    painter.fillRect(backgroundRect, backgroundColor);

    QWidget::paintEvent(event); // Call the base class implementation
}

void PauseMenuScene::onContinueClicked() {
    // Logic to switch back to the previous game scene
    emit continueGameRequested();
}

void PauseMenuScene::onExitClicked() {
    QApplication::quit();
}