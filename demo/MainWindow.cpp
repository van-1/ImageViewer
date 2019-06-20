// Local
#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
  , imageViewer(new ViewerWidget)
{
  ui->setupUi(this);
  ui->gridLayout->addWidget(imageViewer, 0, 0);
  imageViewer->setFixedSize(800, 800);
  imageViewer->show();
}


MainWindow::~MainWindow()
{
  delete ui;
}


void MainWindow::on_openImageBtn_clicked()
{
  QStringList imgFiles = QFileDialog::getOpenFileNames(this, "Директория с изображениями",
                                                       "Image Files (*.png *.jpg *.bmp)");
  m_imagesList.clear();
  if (!imgFiles.isEmpty())
  {
    ui->imagePath->setText(QFileInfo(imgFiles[0]).absolutePath());
    for (int i = 0; i < imgFiles.count(); i++)
    {
      QImage image;
      image.load(imgFiles[i]);
      QPair<QImage,QString> newImg;
      newImg.first = image;
      newImg.second = QFileInfo(imgFiles[i]).fileName();
      m_imagesList.append(newImg);
    }
    imageViewer->setImagesList(m_imagesList);
    imageViewer->showImages();
  }
}
