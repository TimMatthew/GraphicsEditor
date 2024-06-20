#ifndef FILLTOOL_H
#define FILLTOOL_H

#include <QMouseEvent>
#include <QPixmap>
#include <QStack>
#include <QColor>
#include <QImage>

class FillTool {
public:
    FillTool();
    void mousePressEvent(QMouseEvent *event, QPixmap *pixmap);
    void setColor(const QColor &color);

private:
    void floodFill(QImage &image, const QPoint &startPoint, const QColor &newColor);
    QColor targetColor;
    QColor replacementColor;
};

#endif // FILLTOOL_H
