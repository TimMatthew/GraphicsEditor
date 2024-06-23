#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow),
    selectedColor(Qt::black),
    currentTool(None),
    history(new QList<QPixmap>()),
    historyIndex(0)
{

    ui->setupUi(this);
    pixmap = new QPixmap(width()-151, height());
    pixmap->fill(Qt::white);
    setAcceptDrops(true);

    history->append(*pixmap);

    sidebar = new QWidget(this);
    sidebar->setGeometry(width() - 151, 0, 151, height());
    sidebar->setStyleSheet("background-color: lightgrey; border-left: 4px solid black;");

    QPixmap pencil(":/sidebar_icons/pencil.png");
    QPixmap square(":/sidebar_icons/square.png");
    QPixmap circle(":/sidebar_icons/circle.png");
    QPixmap fill(":/sidebar_icons/paint.png");

    QLabel *pencilLabel = new QLabel(sidebar);
    pencilLabel->setGeometry(35, 30, 80, 80);
    pencilLabel->setAlignment(Qt::AlignCenter);
    pencilLabel->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    pencilLabel->setScaledContents(true);
    pencilLabel->setPixmap(pencil);
    pencilLabel->setProperty("tool", Pencil);
    pencilLabel->installEventFilter(this);
    pencilLabel->setToolTip("Pencil Tool");

    QLabel *squareLabel = new QLabel(sidebar);
    squareLabel->setGeometry(35, 140, 80, 80);
    squareLabel->setAlignment(Qt::AlignCenter);
    squareLabel->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    squareLabel->setScaledContents(true);
    squareLabel->setPixmap(square);
    squareLabel->setProperty("tool", Square);
    squareLabel->installEventFilter(this);
    squareLabel->setToolTip("Square Tool");

    QLabel *circleLabel = new QLabel(sidebar);
    circleLabel->setGeometry(35, 250, 80, 80);
    circleLabel->setAlignment(Qt::AlignCenter);
    circleLabel->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    circleLabel->setScaledContents(true);
    circleLabel->setPixmap(circle);
    circleLabel->setProperty("tool", Circle);
    circleLabel->installEventFilter(this);
    circleLabel->setToolTip("Circle Tool");

    QLabel *fillLabel = new QLabel(sidebar);
    fillLabel->setGeometry(35, 360, 80, 80);
    fillLabel->setAlignment(Qt::AlignCenter);
    fillLabel->setStyleSheet("background-color: white; border-radius: 10px; border: 5px solid lightblue;");
    fillLabel->setScaledContents(true);
    fillLabel->setPixmap(fill);
    fillLabel->setProperty("tool", Fill);
    fillLabel->installEventFilter(this);
    fillLabel->setToolTip("Fill an area");

    colorButton = new QPushButton("Select Color", sidebar);
    colorButton->setGeometry(35, 470, 80, 30);
    connect(colorButton, &QPushButton::clicked, this, &MainWindow::openColorDialog);

    colorLabel = new QLabel(sidebar);
    colorLabel->setGeometry(44, 507, 60, 35);
    colorLabel->setStyleSheet("background-color: black; border: none; border-radius: 10px");

    contextMenu = new ContextMenu(this);
    connect(contextMenu->getUndo(), &QAction::triggered, this, &MainWindow::undo);
    connect(contextMenu->getRedo(), &QAction::triggered, this, &MainWindow::redo);

    QPushButton *helpButton = new QPushButton("Help", sidebar);
    helpButton->setGeometry(35, 550, 80, 30);
    connect(helpButton, &QPushButton::clicked, this, &MainWindow::showHelpDialog);

    connect(ui->savePNG, &QAction::triggered, this, &MainWindow::saveAsPNG);
    connect(ui->openPNG, &QAction::triggered, this, &MainWindow::openAsPNG);
    connect(ui->saveTXT, &QAction::triggered, this, &MainWindow::saveAsTXT);
    connect(ui->openTXT, &QAction::triggered, this, &MainWindow::openAsTXT);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete pixmap;
    delete history;
}

void MainWindow::openColorDialog()
{
    QColor color = QColorDialog::getColor(selectedColor, this, "Обрати колір");
    if (color.isValid())
    {
        selectedColor = color;
        pencilTool.setColor(selectedColor);
        squareTool.setColor(selectedColor);
        circleTool.setColor(selectedColor);
        fillTool.setColor(selectedColor);

        qDebug() << selectedColor.name();
        colorLabel->setStyleSheet(QString("background-color: %1; border: none; border-radius: 10px").arg(selectedColor.name()));
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (currentTool == Pencil) {
        pencilTool.mousePressEvent(event);
        update();
    }
    else if (currentTool == Square)
    {
        squareTool.mousePressEvent(event);
        update();
    }
    else if (currentTool == Circle)
    {
        circleTool.mousePressEvent(event);
        update();
    }
    else if (event->button() == Qt::LeftButton && currentTool == Fill)
    {
        fillTool.mousePressEvent(event, pixmap);
        update();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (currentTool == Pencil)
    {
        pencilTool.mouseMoveEvent(event, pixmap);
        update();
    }
    else if (currentTool == Square)
    {
        squareTool.mouseMoveEvent(event, pixmap);
        update();
    }
    else if (currentTool == Circle)
    {
        circleTool.mouseMoveEvent(event, pixmap);
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (currentTool == Pencil)
    {
        pencilTool.mouseReleaseEvent(event);
        update();
    }
    else if (currentTool == Square)
    {
        squareTool.mouseReleaseEvent(event, pixmap);
        update();
    }
    else if (currentTool == Circle)
    {
        circleTool.mouseReleaseEvent(event, pixmap);
        update();
    }

    if (event->button() == Qt::LeftButton && currentTool != None && pixmap->rect().contains(event->pos()))
    {
        recordInHistory();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(0, 0, *pixmap);

    if (currentTool == Square)
    {
        squareTool.paint(painter);
    }
    else if (currentTool == Circle)
    {
        circleTool.paint(painter);
    }
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        QLabel *label = qobject_cast<QLabel*>(obj);
        if (label)
        {
            currentTool = static_cast<Tool>(label->property("tool").toInt());
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void MainWindow::saveAsPNG()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as PNG", "", "PNG Files (*.png);;All Files (*)");
    if (!fileName.isEmpty())
    {
        pixmap->save(fileName);
    }
}

void MainWindow::openAsPNG()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open as PNG", "", "PNG Files (*.png);;All Files (*)");
    if (!fileName.isEmpty())
    {
        QPixmap newPixmap;
        if (newPixmap.load(fileName))
        {
            *pixmap = newPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            update();
        }
    }
}

void MainWindow::saveAsTXT()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save as TXT", "", "TXT Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            pixmap->save(&buffer, "PNG");
            QString base64Data = byteArray.toBase64();
            QTextStream out(&file);
            out << base64Data;
            file.close();
        }
    }
}


void MainWindow::openAsTXT()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open as TXT", "", "TXT Files (*.txt);;All Files (*)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&file);
            QString base64Data = in.readAll();
            QByteArray byteArray = QByteArray::fromBase64(base64Data.toUtf8());
            QPixmap newPixmap;
            newPixmap.loadFromData(byteArray, "PNG");
            *pixmap = newPixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            file.close();
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
    if (mimeData->hasUrls())
    {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty())
        {
            QString fileName = urlList.first().toLocalFile();

            if (QFileInfo(fileName).suffix().toLower() == "png")
            {
                QPixmap newPixmap;
                if (newPixmap.load(fileName))
                {
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
    if (historyIndex > 0)
    {
        historyIndex--;
        *pixmap = *pixmap = history->at(historyIndex);
        update();
    }
}

void MainWindow::redo()
{
    if (historyIndex < history->size() - 1)
    {
        historyIndex++;
        *pixmap = *pixmap = history->at(historyIndex);
        update();
    }
}

void MainWindow::recordInHistory()
{
    if (currentTool != None)
    {

        while (historyIndex < history->size() - 1)
        {
            history->removeLast();
        }
        history->append(*pixmap);
        historyIndex++;
    }
}

void MainWindow::showHelpDialog()
{
    QMessageBox::information(this, "Інструкція для користувача", "Програма дозволяє малювати довільні лінії, квадрати, кола, а також зафарбовувати ділянки.\n\n"
                                                                 "Щоб створити довільну лінію натисніть на олівець і проведіть лінію по білому фоні.\n"
                                                                 "Щоб створити квадрат, натисніть на іконку з квадратом і проведіть його по фону.\n"
                                                                 "Щоб створити коло, натисніть на іконку з колом і проведіть його по фону.\n"
                                                                 "Щоб зафарбувати потрібну Вам ділянку фону, натисніть на відерце і клацніть по зоні, яка Вам потрібна.\n"
                                                                 "Щоб змінити колір, натисність на кнопку 'Select Color'. У відкритому вікні оберіть потрібний колір та атисніть 'OK'\n\n"
                                                                 "У контекстному меню доступні 2 функції: undo й redo. Undo скасовує дію на полотні, коли як Redo повертає її.\n"
                                                                 "Вдалого творчого процесу!");
}
