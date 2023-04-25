#include "mainwindow.h"
#include "drawingarea.h"

#include <QFileDialog>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    drawingArea = new DrawingArea;
    setCentralWidget(drawingArea);

    createActions(drawingArea->getTool());
    createMenus();

    setWindowTitle(tr("Project 3"));
    resize(drawingArea->size());
    setMinimumSize(drawingArea->size());
    setMaximumSize(drawingArea->size());
}

MainWindow::~MainWindow() {
    delete drawingArea;
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        drawingArea->open(fileName);
    }
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        drawingArea->save(fileName);
    }
}

void MainWindow::penColor() {
    QColor newColor = QColorDialog::getColor(drawingArea->getPenColor());
    if (newColor.isValid())
        drawingArea->setPenColor(newColor);
}

void MainWindow::penWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen width:"), drawingArea->getPenWidth(), 1,
                                        15, 2, &ok);
    if (ok)
        drawingArea->setPenWidth(newWidth);
}

void MainWindow::createActions(Tools initTool) {
    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing file"));
    connect(openAction, &QAction::triggered, this, &MainWindow::open);

    saveAction = new QAction(tr("&Save..."), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the image to disk"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);

    clearAction = new QAction(tr("&Clear"), this);
    clearAction->setShortcuts(QKeySequence::Delete);
    clearAction->setStatusTip(tr("Clear the image"));
    connect(clearAction, &QAction::triggered, drawingArea, &DrawingArea::clear);

    penColorAction = new QAction(tr("&Pen Color..."), this);
    penColorAction->setStatusTip(tr("Change the pen color"));
    connect(penColorAction, &QAction::triggered, this, &MainWindow::penColor);

    penWidthAction = new QAction(tr("&Pen Width..."), this);
    penWidthAction->setStatusTip(tr("Change the pen width"));
    connect(penWidthAction, &QAction::triggered, this, &MainWindow::penWidth);

    toolsGroup = new QActionGroup(this);
    toolsGroup->setExclusive(true);

    for (auto item: tools) {
        auto *action = new QAction(tr(item.second), this);
        action->setStatusTip(tr("Set the item as %1").arg(item.second));
        action->setCheckable(true);
        if (item.first == initTool)
            action->setChecked(true);
        action->setActionGroup(toolsGroup);
        action->setData(item.first);
        connect(action, &QAction::triggered, drawingArea, &DrawingArea::setTool);
        toolsActions.append(action);
    }

    antyaliasingAction = new QAction(tr("&Antialiasing"), this);
    antyaliasingAction->setStatusTip(tr("Toggle antialiasing"));
    antyaliasingAction->setCheckable(true);
    antyaliasingAction->setChecked(drawingArea->getAntyaliasing());
    connect(antyaliasingAction, &QAction::triggered, drawingArea, &DrawingArea::toggleAntialysing);
}

void MainWindow::createMenus() {
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(clearAction);

    toolsMenu = new QMenu(tr("&Tools"), this);
    toolsMenu->addAction(penColorAction);
    toolsMenu->addAction(penWidthAction);
    toolsMenu->addSeparator();
    toolsMenu->addActions(toolsActions);
    toolsMenu->addSeparator();
    toolsMenu->addAction(antyaliasingAction);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(toolsMenu);
}