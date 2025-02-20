#pragma once

#include "qt_includes.h"

class NextButton : public QPushButton {
    Q_OBJECT
public:

    explicit NextButton(QWidget* parent = nullptr);

protected:

    void paintEvent(QPaintEvent* event) override;

    void enterEvent(QEnterEvent* event) override;

    void leaveEvent(QEvent* event);

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    bool event(QEvent* event);
private:
    QPixmap currentPixmap;
};


class BackButton : public QPushButton {
    Q_OBJECT
public:

    explicit BackButton(QWidget* parent = nullptr);

protected:

    void paintEvent(QPaintEvent* event) override;

    void enterEvent(QEnterEvent* event) override;

    void leaveEvent(QEvent* event);

    void mousePressEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    bool event(QEvent* event);

private:
    QPixmap currentPixmap;
};

class DescriptionPopup : public QMenu {
    Q_OBJECT
public:
    explicit DescriptionPopup(const QString& description, QWidget* parent = nullptr);
    void showAtPosition(const QPoint& globalPos);

private:
    void setupUI(const QString& description);
    void setupStyle();
};