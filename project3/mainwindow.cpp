#include "mainwindow.h"
#include "drawingarea.h"

#include <QFileDialog>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QColorDialog>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    _drawingArea = new DrawingArea;
    setCentralWidget(_drawingArea);

    createActions(_drawingArea->getTool());
    createMenus();

    setWindowTitle(tr("Project 3"));
    resize(_drawingArea->size());
    setMinimumSize(_drawingArea->size());
    setMaximumSize(_drawingArea->size());
}

MainWindow::~MainWindow() {
    delete _drawingArea;
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        _drawingArea->open(fileName);
    }
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        _drawingArea->save(fileName);
    }
}

void MainWindow::penColor() {
    QColor newColor = QColorDialog::getColor(_drawingArea->getPenColor());
    if (newColor.isValid())
        _drawingArea->setPenColor(newColor);
}

void MainWindow::penWidth() {
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Scribble"), tr("Select pen _width:"), _drawingArea->getPenWidth(), 1,
                                        15, 2, &ok);
    if (ok)
        _drawingArea->setPenWidth(newWidth);
}

void MainWindow::fillingColor() {
    QColor newColor = QColorDialog::getColor(_drawingArea->getFillingColor());
    if (newColor.isValid())
        _drawingArea->setFillingColor(newColor);
}

void MainWindow::fillingImage() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        QImage newImage;
        if (newImage.load(fileName)) {
            _drawingArea->setFillingImage(newImage);
        }
    }
}

void MainWindow::toggleFillingType() {
    _drawingArea->toggleFillingType();
}

void MainWindow::createActions(Tools initTool) {
    _openAction = new QAction(tr("&Open..."), this);
    _openAction->setShortcuts(QKeySequence::Open);
    _openAction->setStatusTip(tr("Open an existing file"));
    connect(_openAction, &QAction::triggered, this, &MainWindow::open);

    _saveAction = new QAction(tr("&Save..."), this);
    _saveAction->setShortcuts(QKeySequence::Save);
    _saveAction->setStatusTip(tr("Save the _image to disk"));
    connect(_saveAction, &QAction::triggered, this, &MainWindow::save);

    _clearAction = new QAction(tr("&Clear"), this);
    _clearAction->setShortcuts(QKeySequence::Delete);
    _clearAction->setStatusTip(tr("Clear the _image"));
    connect(_clearAction, &QAction::triggered, _drawingArea, &DrawingArea::clear);

    _penColorAction = new QAction(tr("&Pen Color..."), this);
    _penColorAction->setStatusTip(tr("Change the pen _color"));
    connect(_penColorAction, &QAction::triggered, this, &MainWindow::penColor);

    _penWidthAction = new QAction(tr("&Pen Width..."), this);
    _penWidthAction->setStatusTip(tr("Change the pen _width"));
    connect(_penWidthAction, &QAction::triggered, this, &MainWindow::penWidth);

    _fillingColorAction = new QAction(tr("&Filling Color..."), this);
    _fillingColorAction->setStatusTip(tr("Change the filling _color"));
    connect(_fillingColorAction, &QAction::triggered, this, &MainWindow::fillingColor);

    _fillingImageAction = new QAction(tr("&Filling Image..."), this);
    _fillingImageAction->setStatusTip(tr("Change the filling _image"));
    connect(_fillingImageAction, &QAction::triggered, this, &MainWindow::fillingImage);

    _toggleFillingTypeAction = new QAction(tr("&Filling with image"), this);
    _toggleFillingTypeAction->setStatusTip(tr("Toggle type of filling"));
    _toggleFillingTypeAction->setCheckable(true);
    _toggleFillingTypeAction->setChecked(_drawingArea->getIfFillingWithImage());
    connect(_toggleFillingTypeAction, &QAction::triggered, this, &MainWindow::toggleFillingType);

    _toolsGroup = new QActionGroup(this);
    _toolsGroup->setExclusive(true);

    for (auto item: tools) {
        auto *action = new QAction(tr(item.second), this);
        action->setStatusTip(tr("Set the item as %1").arg(item.second));
        action->setCheckable(true);
        if (item.first == initTool)
            action->setChecked(true);
        action->setActionGroup(_toolsGroup);
        action->setData(item.first);
        connect(action, &QAction::triggered, _drawingArea, &DrawingArea::setTool);
        _toolsActions.append(action);
    }

    _antyaliasingAction = new QAction(tr("&Antialiasing"), this);
    _antyaliasingAction->setStatusTip(tr("Toggle antialiasing"));
    _antyaliasingAction->setCheckable(true);
    _antyaliasingAction->setChecked(_drawingArea->getAntyaliasing());
    connect(_antyaliasingAction, &QAction::triggered, _drawingArea, &DrawingArea::toggleAntialysing);
}

void MainWindow::createMenus() {
    _fileMenu = new QMenu(tr("&File"), this);
    _fileMenu->addAction(_openAction);
    _fileMenu->addAction(_saveAction);
    _fileMenu->addAction(_clearAction);

    _toolsMenu = new QMenu(tr("&Tools"), this);
    _toolsMenu->addAction(_penColorAction);
    _toolsMenu->addAction(_penWidthAction);
    _toolsMenu->addSeparator();
    _toolsMenu->addAction(_fillingColorAction);
    _toolsMenu->addAction(_fillingImageAction);
    _toolsMenu->addAction(_toggleFillingTypeAction);
    _toolsMenu->addSeparator();
    _toolsMenu->addActions(_toolsActions);
    _toolsMenu->addSeparator();
    _toolsMenu->addAction(_antyaliasingAction);

    menuBar()->addMenu(_fileMenu);
    menuBar()->addMenu(_toolsMenu);
}