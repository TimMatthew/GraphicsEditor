#include "PencilTool.h"

PencilTool::PencilTool()
    : drawing(false), penColor(Qt::black) {}

void PencilTool::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        drawing = true;
    }
}

void PencilTool::mouseMoveEvent(QMouseEvent *event, QPixmap *pixmap)
{
    if ((event->buttons() & Qt::LeftButton) && drawing) {
        QPainter painter(pixmap);
        painter.setPen(QPen(penColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter.drawLine(lastPoint, event->pos());
        lastPoint = event->pos();
    }
}

void PencilTool::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing) {
        drawing = false;
    }
}

void PencilTool::setColor(const QColor &color)
{
    penColor = color;
}
