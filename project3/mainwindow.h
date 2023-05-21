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

    DrawingArea *_drawingArea;

    QMenu *_fileMenu{};
    QAction *_openAction{};
    QAction *_saveAction{};
    QAction *_clearAction{};

    QMenu *_toolsMenu{};
    QAction *_penColorAction{};
    QAction *_penWidthAction{};
    QActionGroup *_toolsGroup{};
    QList<QAction *> _toolsActions;
    QAction *_antyaliasingAction{};
};

#endif // MAINWINDOW_H