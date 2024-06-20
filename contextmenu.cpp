#include "contextmenu.h"

ContextMenu::ContextMenu(QWidget *parent)
    : QMenu(parent)
{
    undo = new QAction("Undo", this);
    redo = new QAction("Redo", this);

    addAction(undo);
    addAction(redo);

}
