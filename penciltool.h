#ifndef PENCILTOOL_H
#define PENCILTOOL_H

#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QPoint>
#include <QColor>

class PencilTool {
public:
    PencilTool();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event, QPixmap *pixmap);
    void mouseReleaseEvent(QMouseEvent *event);
    void setColor(const QColor &color);

private:
    QPoint lastPoint;
    bool drawing;
    QColor penColor;
};

#endif // PENCILTOOL_H
