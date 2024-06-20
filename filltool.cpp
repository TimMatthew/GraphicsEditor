#include "FillTool.h"

FillTool::FillTool()
    : targetColor(Qt::white), replacementColor(Qt::yellow) {}

void FillTool::mousePressEvent(QMouseEvent *event, QPixmap *pixmap) {
    QPoint startPoint = event->pos();
    QImage image = pixmap->toImage();

    targetColor = image.pixelColor(startPoint);
    if (targetColor != replacementColor) {
        floodFill(image, startPoint, replacementColor);
        *pixmap = QPixmap::fromImage(image);
    }
}

void FillTool::floodFill(QImage &image, const QPoint &startPoint, const QColor &newColor) {
    QStack<QPoint> stack;
    stack.push(startPoint);

    while (!stack.isEmpty()) {
        QPoint point = stack.pop();
        if (image.pixelColor(point) == targetColor) {
            image.setPixelColor(point, newColor);

            if (point.x() > 0) stack.push(QPoint(point.x() - 1, point.y()));
            if (point.x() < image.width() - 1) stack.push(QPoint(point.x() + 1, point.y()));
            if (point.y() > 0) stack.push(QPoint(point.x(), point.y() - 1));
            if (point.y() < image.height() - 1) stack.push(QPoint(point.x(), point.y() + 1));
        }
    }
}

void FillTool::setColor(const QColor &color)
{
    replacementColor = color;
}
