// Local
#include "ImageSwitcher.h"
#include "ui_imageswitcher.h"

ImageSwitcher::ImageSwitcher(QWidget* parent)
  : QWidget(parent)
  , ui(new Ui::ImageSwitcher)
{
  ui->setupUi(this);
}


ImageSwitcher::~ImageSwitcher()
{
  delete ui;
}


void ImageSwitcher::setCurrentImNum(QString currImNum)
{
  ui->currentImageLabel->setText(currImNum);
}


void ImageSwitcher::setImCount(QString imgsCount)
{
  ui->countImagesLabel->setText(imgsCount);
}


void ImageSwitcher::setVisibleNextImgButton(bool visible)
{
  ui->nextImageButton->setVisible(visible);
}


void ImageSwitcher::setVisiblePrvImgButton(bool visible)
{
  ui->prvImageButton->setVisible(visible);
}


void ImageSwitcher::on_nextImageButton_clicked()
{
  emit nextImageClicked();
}


void ImageSwitcher::on_prvImageButton_clicked()
{
  emit prvImageClicked();
}
