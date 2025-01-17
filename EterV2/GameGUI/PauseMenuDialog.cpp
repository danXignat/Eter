#include "PauseMenuDialog.h"
#include <QVBoxLayout>
#include <QApplication>

PauseMenuDialog::PauseMenuDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Pause Menu");

    continueButton = new QPushButton("Continue Playing", this);
    exitButton = new QPushButton("Exit", this);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(continueButton);
    layout->addWidget(exitButton);

    connect(continueButton, &QPushButton::clicked, this, &PauseMenuDialog::onContinueClicked);
    connect(exitButton, &QPushButton::clicked, this, &PauseMenuDialog::onExitClicked);
}

void PauseMenuDialog::onContinueClicked() {
    close();
}

void PauseMenuDialog::onExitClicked() {
    QApplication::quit();
}
