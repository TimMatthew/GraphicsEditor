#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , selectedColor(Qt::black)
    , currentTool(None)
{

    ui->setupUi(this);
    pixmap = new QPixmap(width()-151, height());
    pixmap->fill(Qt::white);
    setAcceptDrops(true);

    history.append(*pixmap);
    historyIndex = 0;

    sidebar = new QWidget(this);
    sidebar->setGeometry(width() - 151, 0, 151, height());
    sidebar->setStyleSheet("background-color: lightgrey; border-left: 4px solid black;");

    QPixmap pencil(":/sidebar_icons/pencil.png");
    QPixmap square(":/sidebar_icons/square.png");
    QPixmap circle(":/sidebar_icons/circle.png");
    QPixmap fill(":/sidebar_icons/paint.png");

    QLabel *label1 = new QLabel(sidebar);
    label1->setGeometry(35, 30, 80, 80);
    label1->setAlignment(Qt::AlignCenter);
    label1->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    label1->setScaledContents(true);
    label1->setPixmap(pencil);
    label1->setProperty("tool", Pencil);
    label1->installEventFilter(this);

    QLabel *label2 = new QLabel(sidebar);
    label2->setGeometry(35, 140, 80, 80);
    label2->setAlignment(Qt::AlignCenter);
    label2->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    label2->setScaledContents(true);
    label2->setPixmap(square);
    label2->setProperty("tool", Square);
    label2->installEventFilter(this);

    QLabel *label3 = new QLabel(sidebar);
    label3->setGeometry(35, 250, 80, 80);
    label3->setAlignment(Qt::AlignCenter);
    label3->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    label3->setScaledContents(true);
    label3->setPixmap(circle);
    label3->setProperty("tool", Circle);
    label3->installEventFilter(this);

    QLabel *label4 = new QLabel(sidebar);
    label4->setGeometry(35, 360, 80, 80);
    label4->setAlignment(Qt::AlignCenter);
    label4->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    label4->setScaledContents(true);
    label4->setPixmap(fill);
    label4->setProperty("tool", Fill);
    label4->installEventFilter(this);

    colorButton = new QPushButton("Select Color", sidebar);
    colorButton->setGeometry(35, 470, 80, 30);
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::openColorDialog);

    colorLabel = new QLabel(sidebar);
    colorLabel->setGeometry(35, 510, 80, 30);
    colorLabel->setStyleSheet("background-color: black; border-radius: 10px;");

    contextMenu = new ContextMenu(this);
    connect(contextMenu->getUndo(), &QAction::triggered, this, &MainWindow::undo);
    connect(contextMenu->getRedo(), &QAction::triggered, this, &MainWindow::redo);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete pixmap;
}

void MainWindow::openColorDialog()
{
    QColor color = QColorDialog::getColor(selectedColor, this, "Select Color");
    if (color.isValid()) {
        selectedColor = color;
        pencilTool.setColor(selectedColor);
        squareTool.setColor(selectedColor);
        circleTool.setColor(selectedColor);
        fillTool.setColor(selectedColor);

        colorLabel->setStyleSheet(QString("background-color: %1; border: 2px solid grey;").arg(selectedColor.name()));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (currentTool == Pencil) {
        pencilTool.mousePressEvent(event);
        update();
    } else if (currentTool == Square) {
        squareTool.mousePressEvent(event);
        update();
    } else if (currentTool == Circle) {
        circleTool.mousePressEvent(event);
        update();
    } else if (event->button() == Qt::LeftButton && currentTool == Fill) {
        fillTool.mousePressEvent(event, pixmap);
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (currentTool == Pencil) {
        pencilTool.mouseMoveEvent(event, pixmap);
        update();
    } else if (currentTool == Square) {
        squareTool.mouseMoveEvent(event, pixmap);
        update();
    } else if (currentTool == Circle) {
        circleTool.mouseMoveEvent(event, pixmap);
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (currentTool == Pencil) {
        pencilTool.mouseReleaseEvent(event);
        update();
    } else if (currentTool == Square) {
        squareTool.mouseReleaseEvent(event, pixmap);
        update();
    } else if (currentTool == Circle) {
        circleTool.mouseReleaseEvent(event, pixmap);
        update();
    }

    if (event->button() == Qt::LeftButton && currentTool != None && pixmap->rect().contains(event->pos())) {
        recordInHistory();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *pixmap);

    if (currentTool == Square) {
        squareTool.paint(painter);
    } else if (currentTool == Circle) {
        circleTool.paint(painter);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress) {
        QLabel *label = qobject_cast<QLabel*>(obj);
        if (label) {
            currentTool = static_cast<Tool>(label->property("tool").toInt());
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::saveToFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", "", "PNG Files (*.png);;All Files (*)");
    if (!fileName.isEmpty()) {
        pixmap->save(fileName);
    }
}

void MainWindow::openFromFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Image", "", "PNG Files (*.png);;All Files (*)");
    if (!fileName.isEmpty()) {
        QPixmap newPixmap;
        if (newPixmap.load(fileName)) {
            *pixmap = newPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            update();
        }
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString fileName = urlList.first().toLocalFile();
            if (QFileInfo(fileName).suffix().toLower() == "png") {
                QPixmap newPixmap;
                if (newPixmap.load(fileName)) {
                    *pixmap = newPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
                    update();
                }
            }
        }
    }
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    contextMenu->exec(event->globalPos());
}

void MainWindow::undo()
{
    if (historyIndex > 0) {
        historyIndex--;
        *pixmap = history.at(historyIndex);
        update();
    }
}

void MainWindow::redo()
{
    if (historyIndex < history.size() - 1) {
        historyIndex++;
        *pixmap = history.at(historyIndex);
        update();
    }
}

void MainWindow::recordInHistory()
{
    if (currentTool != None) {

        while (historyIndex < history.size() - 1) {
            history.removeLast();
        }
        history.append(*pixmap);
        historyIndex++;
    }
}

