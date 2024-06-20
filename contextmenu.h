#ifndef CONTEXTMENU_H
#define CONTEXTMENU_H

#include <QMenu>
#include <QAction>
#include <QWidget>

class ContextMenu : public QMenu
{
    Q_OBJECT

public:
    explicit ContextMenu(QWidget *parent = nullptr);
    QAction* getUndo() const { return undo; }
    QAction* getRedo() const { return redo; }

private:
    QAction *undo;
    QAction *redo;

};

#endif // CONTEXTMENU_H
