#include "SquareTool.h"

SquareTool::SquareTool()
    : drawing(false), penColor(Qt::black) {}

void SquareTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        startPoint = event->pos();
        drawing = true;
    }
}

void SquareTool::mouseMoveEvent(QMouseEvent *event, QPixmap *pixmap)
{
    if (drawing) {
        currentPoint = event->pos();
    }
}

void SquareTool::mouseReleaseEvent(QMouseEvent *event, QPixmap *pixmap)
{
    if (event->button() == Qt::LeftButton && drawing) {
        QPainter painter(pixmap);
        painter.setPen(QPen(penColor, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        QRect rect(startPoint, currentPoint);
        painter.drawRect(rect.normalized());
        drawing = false;
    }
}

void SquareTool::paint(QPainter &painter)
{
    if (drawing) {
        painter.setPen(QPen(penColor, 2, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin));
        QRect rect(startPoint, currentPoint);
        painter.drawRect(rect.normalized());
    }
}

void SquareTool::setColor(const QColor &color)
{
    penColor = color;
}
