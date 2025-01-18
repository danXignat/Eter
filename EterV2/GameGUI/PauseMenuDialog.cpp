#include "PauseMenuDialog.h"
#include <QVBoxLayout>
#include <QApplication>

PauseMenuScene::PauseMenuScene(QWidget* parent)
    : QWidget(parent) {
    setWindowTitle("Pause Menu");
    resize(300, 200);

    continueButton = new QPushButton("Continue Playing", this);
    exitButton = new QPushButton("Exit", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(continueButton);
    layout->addWidget(exitButton);

    connect(continueButton, &QPushButton::clicked, this, &PauseMenuScene::onContinueClicked);
    connect(exitButton, &QPushButton::clicked, this, &PauseMenuScene::onExitClicked);
}

void PauseMenuScene::onContinueClicked() {
    // Logic to switch back to the previous game scene
    emit continueGameRequested();
}

void PauseMenuScene::onExitClicked() {
    QApplication::quit();
}