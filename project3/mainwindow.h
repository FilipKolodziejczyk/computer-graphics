#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "shape.h"
#include "drawingarea.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void open();

    void save();

    void penColor();

    void penWidth();

private:
    void createActions(Tools initTool);

    void createMenus();

    DrawingArea *drawingArea;

    QMenu *fileMenu;
    QAction *openAction;
    QAction *saveAction;
    QAction *clearAction;

    QMenu *toolsMenu;
    QAction *penColorAction;
    QAction *penWidthAction;
    QActionGroup *toolsGroup;
    QList<QAction *> toolsActions;
    QAction *antyaliasingAction;
};

#endif // MAINWINDOW_H