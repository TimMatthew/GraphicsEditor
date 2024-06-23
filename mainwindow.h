#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QColorDialog>
#include <QFileDialog>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include <QBuffer>
#include <QMimeData>
#include <QMessageBox>
#include "squaretool.h"
#include "penciltool.h"
#include "circletool.h"
#include "filltool.h"
#include "contextmenu.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void mousePressEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
    void paintEvent(QPaintEvent *e) override;
    bool eventFilter(QObject *obj, QEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
    void undo();
    void redo();
    void recordInHistory();
    void showHelpDialog();

private slots:
    void openColorDialog();
    void saveAsPNG();
    void openAsPNG();
    void saveAsTXT();
    void openAsTXT();

private:
    Ui::MainWindow *ui;
    QPixmap *pixmap;
    QWidget *sidebar;
    QPushButton *colorButton;
    QColor selectedColor;
    enum Tool { None, Pencil, Square, Circle, Fill };
    QLabel *colorLabel;
    Tool currentTool;
    SquareTool squareTool;
    PencilTool pencilTool;
    CircleTool circleTool;
    FillTool fillTool;
    ContextMenu *contextMenu;
    QList<QPixmap> *history;
    int historyIndex;
};

#endif // MAINWINDOW_H
