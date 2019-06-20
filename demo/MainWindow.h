#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// Qt
#include <QMainWindow>
#include <QFileDialog>
#include <QPair>
#include <QList>

// Local
#include "ViewerWidget.h"

namespace Ui
{
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  public slots:
    void on_openImageBtn_clicked();

  private:
    Ui::MainWindow* ui;
    ViewerWidget* imageViewer;
    QList<QPair<QImage,QString>> m_imagesList;
};

#endif // MAINWINDOW_H
