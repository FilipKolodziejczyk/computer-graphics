#include "mainwindow.h"
#include "drawingarea.h"

#include <QFileDialog>
#include <QActionGroup>
#include <QMenu>
#include <QMenuBar>
#include <QInputDialog>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    _drawingArea = new DrawingArea;
    setCentralWidget(_drawingArea);

    createActions();
    createMenus();

    setWindowTitle(tr("Project 5"));
    resize(_drawingArea->size());
    setMinimumSize(_drawingArea->size());
    setMaximumSize(_drawingArea->size());
}

MainWindow::~MainWindow() {
    delete _drawingArea;
}

void MainWindow::loadTexture() {
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open Texture File"), QDir::currentPath(),
                                                 tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (!fileName.isEmpty()) {
        _drawingArea->loadTexture(fileName);
    }
}

void MainWindow::createActions() {
    _loadTextureAction = new QAction(tr("&Open ..."), this);
    _loadTextureAction->setShortcut(QKeySequence::Open);
    _loadTextureAction->setStatusTip(tr("Load a texture"));
    connect(_loadTextureAction, &QAction::triggered, this, &MainWindow::loadTexture);

    _toggleTextureAction = new QAction(tr("&Toggle Texture"), this);
    _toggleTextureAction->setShortcut(tr("Ctrl+T"));
    _toggleTextureAction->setStatusTip(tr("Toggle texture on/off"));
    connect(_toggleTextureAction, &QAction::triggered, _drawingArea, &DrawingArea::toggleTexture);
}

void MainWindow::createMenus() {
    _textureMenu = menuBar()->addMenu(tr("&Texture"));
    _textureMenu->addAction(_loadTextureAction);
    _textureMenu->addAction(_toggleTextureAction);
    menuBar()->addMenu(_textureMenu);
}