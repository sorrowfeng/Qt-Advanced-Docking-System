#ifndef FLOATINGDOCKTITLEBAR_H
#define FLOATINGDOCKTITLEBAR_H

#include <QWidget>

#include <QWidget>
#include <QColor>

class QLabel;
class QPushButton;

class FloatingDockTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit FloatingDockTitleBar(QWidget* parent = nullptr);

    void setTitleColor(const QColor& color);
    void setWindowTitle(const QString& title);

    QRect dragArea() const;

signals:
    void closeRequested();
    void maximizeRequested();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private:
    QColor m_titleColor = QColor(45, 150, 255);  // 默认蓝色
    QString m_title;
    QPoint m_dragPosition;

    QLabel* m_titleLabel;
    QPushButton* m_closeButton;
    QPushButton* m_maximizeButton;
};

#endif  // FLOATINGDOCKTITLEBAR_H