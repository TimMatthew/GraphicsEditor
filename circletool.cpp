#include "CircleTool.h"

CircleTool::CircleTool()
    : drawing(false), penColor(Qt::black) {}

void CircleTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        drawing = true;
    }
}

void CircleTool::mouseMoveEvent(QMouseEvent *event, QPixmap *pixmap)
{
    if (drawing) {
        currentPoint = event->pos();
    }
}

void CircleTool::mouseReleaseEvent(QMouseEvent *event, QPixmap *pixmap)
{
    if (event->button() == Qt::LeftButton && drawing) {
        QPainter painter(pixmap);
        painter.setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        QRect rect(startPoint, currentPoint);
        painter.drawEllipse(rect.normalized());
        drawing = false;
    }
}

void CircleTool::paint(QPainter &painter)
{
    if (drawing) {
        painter.setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        QRect rect(startPoint, currentPoint);
        painter.drawEllipse(rect.normalized());
    }
}

void CircleTool::setColor(const QColor &color)
{
    penColor = color;
}
