#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QColor>

class CircleTool {
public:
    CircleTool();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event, QPixmap *pixmap);
    void mouseReleaseEvent(QMouseEvent *event, QPixmap *pixmap);
    void paint(QPainter &painter);
    void setColor(const QColor &color);

private:
    QPoint startPoint;
    QPoint currentPoint;
    bool drawing;
    QColor penColor;
};

#endif // CIRCLETOOL_H
