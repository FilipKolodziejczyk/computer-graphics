#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "drawingarea.h"

#include <QMainWindow>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void loadTexture();

private:
    void createActions();

    void createMenus();

    DrawingArea *_drawingArea;

    QMenu *_textureMenu{};
    QAction *_loadTextureAction{};
    QAction *_toggleTextureAction{};
};

#endif // MAINWINDOW_H