#include "FloatingDockTitleBar.h"

#include <QApplication>
#include <QVBoxLayout>

#include <QPainter>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>

FloatingDockTitleBar::FloatingDockTitleBar(QWidget* parent) : QWidget(parent)
{
    setFixedHeight(30);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(2, 0, 2, 0);
    layout->setSpacing(0);

    m_titleLabel = new QLabel("Title");
    m_titleLabel->setStyleSheet("color: white; font-weight: bold;");
    layout->addWidget(m_titleLabel, 1);

    m_maximizeButton = new QPushButton();
    m_maximizeButton->setFixedSize(50, 30);
    m_maximizeButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none;"
        "   qproperty-icon: url(:/ads/images/maximize-button-focused.svg);"
	    "   qproperty-iconSize: 20px; }"
        "QPushButton:hover { background-color: #E5E5E5; }");

    connect(m_maximizeButton, &QPushButton::clicked, this,
            &FloatingDockTitleBar::maximizeRequested);
    layout->addWidget(m_maximizeButton);


    m_closeButton = new QPushButton();
    m_closeButton->setFixedSize(50, 30);
    m_closeButton->setStyleSheet(
        "QPushButton { background-color: transparent; border: none;"
        "   qproperty-icon: url(:/ads/images/close-button-focused.svg);"
	    "   qproperty-iconSize: 20px; }"
        "QPushButton:hover { background-color: #E81123; }");

    connect(m_closeButton, &QPushButton::clicked, this,
            &FloatingDockTitleBar::closeRequested);
    layout->addWidget(m_closeButton);

    setLayout(layout);
}

void FloatingDockTitleBar::setTitleColor(const QColor& color)
{
    m_titleColor = color;
    update();
}

void FloatingDockTitleBar::setWindowTitle(const QString& title)
{
    m_title = title;
    m_titleLabel->setText(title);
    update();
}

QRect FloatingDockTitleBar::dragArea() const
{
    QRect titleBarRect = geometry();

    int len = 4 + m_closeButton->width() * 2;
    titleBarRect.setWidth(titleBarRect.width() - len);

    return titleBarRect;
}


void FloatingDockTitleBar::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    painter.fillRect(rect(), m_titleColor);
}

void FloatingDockTitleBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragPosition = event->globalPos() - parentWidget()->pos();
        event->accept();
    }
}

void FloatingDockTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        parentWidget()->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void FloatingDockTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) emit maximizeRequested();
}

