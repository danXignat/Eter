#include "controls.h"

NextButton::NextButton(QWidget* parent)
    : QPushButton(parent), currentPixmap("../pictures/estetics/next_normal.png") {
    setFixedSize(currentPixmap.size());
}

void NextButton::paintEvent(QPaintEvent* event) {
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

void NextButton::leaveEvent(QEvent* event) {
    qDebug() << "Hover ended";
    currentPixmap.load("../pictures/estetics/next_normal.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
    QPushButton::leaveEvent(event);
}

void NextButton::mousePressEvent(QMouseEvent* event) {

    currentPixmap.load("../pictures/estetics/next_click.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
    QPushButton::mousePressEvent(event);
}

void NextButton::mouseReleaseEvent(QMouseEvent* event) {

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


///-------------------------

BackButton::BackButton(QWidget* parent)
    : QPushButton(parent), currentPixmap("../pictures/estetics/back_normal.png") {
    setFixedSize(currentPixmap.size());
}

void BackButton::paintEvent(QPaintEvent* event) {
    QPainter painter(this);
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawPixmap(0, 0, currentPixmap);
}

void BackButton::enterEvent(QEnterEvent* event) {
    qDebug() << "Hover started";

    currentPixmap.load("../pictures/estetics/back_hover.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    qDebug() << currentPixmap.size();
    update();
    QPushButton::enterEvent(event);
}

void BackButton::leaveEvent(QEvent* event) {
    qDebug() << "Hover ended";
    currentPixmap.load("../pictures/estetics/back_normal.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
    QPushButton::leaveEvent(event);
}

void BackButton::mousePressEvent(QMouseEvent* event) {

    currentPixmap.load("../pictures/estetics/back_click.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    update();
    QPushButton::mousePressEvent(event);
}

void BackButton::mouseReleaseEvent(QMouseEvent* event) {

    currentPixmap.load("../pictures/estetics/back_normal.png");
    currentPixmap = currentPixmap.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

    update();
    QPushButton::mouseReleaseEvent(event);
}
bool BackButton::event(QEvent* event) {
    if (event->type() == QEvent::Leave) {
        qDebug() << "Generic Leave event detected";
    }
    return QPushButton::event(event); // Pass to the base class
}



/// <summary>
/// -------------
/// </summary>
/// <param name="description"></param>
/// <param name="parent"></param>

DescriptionPopup::DescriptionPopup(const QString& description, QWidget* parent)
    : QMenu(parent) {
    setFixedWidth(300);
    setupStyle();
    setupUI(description);
}

void DescriptionPopup::setupStyle() {
    setStyleSheet(R"(
        QMenu {
            background-color: rgba(0, 0, 0, 0.8);
            border: 1px solid rgba(204, 204, 204, 0.8);
            border-radius: 5px;
            padding: 5px;
        }
        QMenu::item {
            background-color: transparent;
            padding: 5px;
        }
        QLabel {
            color: rgba(255, 255, 255, 1);
            font-size: 12px;
            background-color: transparent;
        }
    )");
}

void DescriptionPopup::setupUI(const QString& description) {
    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);

    QLabel* descLabel = new QLabel(description, container);
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    descLabel->setStyleSheet("padding: 10px; background-color: transparent;");

    layout->addWidget(descLabel);

    QWidgetAction* widgetAction = new QWidgetAction(this);
    widgetAction->setDefaultWidget(container);
    addAction(widgetAction);
}

void DescriptionPopup::showAtPosition(const QPoint& globalPos) {
    popup(globalPos);
    connect(this, &QMenu::aboutToHide, this, &QObject::deleteLater);
}
