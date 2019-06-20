// Local
#include "ViewerWidget.h"

ViewerWidget::ViewerWidget(QWidget* parent)
  : QWidget(parent)
  , m_fullScreenWindow( new ImgViewerWidget)
  , m_imageViewer(new ImageViewer(this))
  , m_imageHeaderText (new QLabel)
  , m_imageText (new QLabel)
  , m_bgColor(Qt::white)
  , m_imageSwitcher(new ImageSwitcher)
  , m_imagePixmap(new QPixmap)
  , m_currentImage(-1)
{
  this->setWindowFlags(Qt::FramelessWindowHint | Qt::CustomizeWindowHint);
  this->setMouseTracking(true);
  m_layout.setContentsMargins(0, 0, 0, 0);
  this->setLayout(&m_layout);

  m_layout.setMargin(0);
  m_layout.setSpacing(0);

  m_layout.addWidget(m_imageHeaderText);
  m_layout.addWidget(m_imageViewer);
  m_layout.addWidget(m_imageText);
  m_layout.addWidget(m_imageSwitcher);

  setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
  m_imageViewer->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
  m_imageViewer->setMaximumSize(this->size());
  setMaximumSize(this->size());

  m_imageHeaderText->setIndent(m_textIndent);
  m_imageHeaderText->setMargin(m_textMargin);
  m_imageText->setIndent(m_textIndent);
  m_imageText->setMargin(m_textMargin);
  m_imageHeaderText->setStyleSheet("font-weight: bold; color: darkBlue");
  m_fullScreenWindow->hide();
  connect(m_imageSwitcher, SIGNAL(nextImageClicked()), this, SLOT(nextImage()));
  connect(m_imageSwitcher, SIGNAL(prvImageClicked()), this, SLOT(prvImage()));
  connect(m_fullScreenWindow, SIGNAL(nextImage()), this, SLOT(fullScreenNextImage()));
  connect(m_fullScreenWindow, SIGNAL(prvImage()), this, SLOT(fullScreenPrvImage()));
  connect(m_fullScreenWindow, SIGNAL(zoomOut()), this, SLOT(zoomOut()));
  connect(m_fullScreenWindow, SIGNAL(zoomIn()), this, SLOT(zoomIn()));
}


QString ViewerWidget::imageHeaderText() const
{
  return m_imageHeaderText->text();
}


QString ViewerWidget::imageText() const
{
  return m_imageText->text();
}


bool ViewerWidget::showImage(QPixmap* pixmap)
{
  if (!pixmap)
    return false;

  m_imageViewer->displayImage(*pixmap);
  if (!m_imageViewer->sourceImageFits())
    m_imageViewer->setFitWindow();
  return true;
}


void ViewerWidget::openImageAtIndex(int i)
{
  if (i == -1)
    return;

  if (m_imagesList.count() == 0)
    return;

  m_imageSwitcher->setImCount(QString("%1").number(m_imagesList.count()));
  m_fullScreenWindow->setCountImgsLabelText(QString("%1").number(m_imagesList.count()));

  m_imagePixmap = new QPixmap;
  m_imagePixmap->convertFromImage(m_imagesList[i].first);
  closeImage();
  if (!showImage(m_imagePixmap))
  {
    setImageHeaderText("Ошибка! Не удалось открыть изображение");
    setImageText("");
  }
  setImageText(m_imagesList[i].second);
}


void ViewerWidget::closeImage()
{
  m_imageViewer->closeImage();
  m_imageHeaderText->setText("");
  m_imageText->setText("");
}


void ViewerWidget::setImageHeaderText(QString headerText)
{
  m_imageHeaderText->setText(headerText);
}


void ViewerWidget::setImageText(QString imageText)
{
  m_imageText->setText(imageText);
}


void ViewerWidget::setImagesList(const QList<QPair<QImage, QString>>& imagesList)
{
  m_imagesList.clear();
  m_currentImage = -1;
  if (!imagesList.isEmpty())
    m_imagesList = imagesList;
}


void ViewerWidget::nextImage()
{
  m_imageSwitcher->setVisiblePrvImgButton(true);
  m_imageSwitcher->setVisibleNextImgButton(true);
  m_fullScreenWindow->setBackEnabled(true);
  m_fullScreenWindow->setNextEnabled(true);
  m_currentImage++;

  m_currentImage = std::min(m_imagesList.count() - 1, m_currentImage);
  openImageAtIndex(m_currentImage);

  m_imageSwitcher->setCurrentImNum(QString("%1").number(m_currentImage + 1));
  m_fullScreenWindow->setCurrImgNumLabelText(QString("%1").number(m_currentImage + 1));

  if ((m_currentImage + 1) == m_imagesList.count() && m_imagesList.count() != 0)
  {
    m_imageSwitcher->setVisiblePrvImgButton(true);
    m_imageSwitcher->setVisibleNextImgButton(false);
    m_fullScreenWindow->setNextEnabled(false);
  }
}


void ViewerWidget::prvImage()
{
  m_imageSwitcher->setVisiblePrvImgButton(true);
  m_imageSwitcher->setVisibleNextImgButton(true);
  m_fullScreenWindow->setBackEnabled(true);
  m_fullScreenWindow->setNextEnabled(true);

  m_currentImage--;
  m_currentImage = std::max(0, m_currentImage);;
  openImageAtIndex(m_currentImage);

  m_imageSwitcher->setCurrentImNum(QString("%1").number(m_currentImage + 1));
  m_fullScreenWindow->setCurrImgNumLabelText(QString("%1").number(m_currentImage + 1));

  if (m_currentImage == 0 && m_imagesList.count() != 0)
  {
    m_imageSwitcher->setVisiblePrvImgButton(false);
    m_imageSwitcher->setVisibleNextImgButton(true);
    m_fullScreenWindow->setBackEnabled(false);
  }
}


void ViewerWidget::fullScreenNextImage()
{
  nextImage();
  m_fullScreenWindow->setImage(m_imageViewer->pixmap(), m_imagesList[m_currentImage].second);
}


void ViewerWidget::fullScreenPrvImage()
{
  prvImage();
  m_fullScreenWindow->setImage(m_imageViewer->pixmap(), m_imagesList[m_currentImage].second);
}


void ViewerWidget::zoomOut()
{
  if (m_fullScreenWindow->rectImageFits())
  {
    m_fullScreenWindow->setBackEnabled(true);
    m_fullScreenWindow->setNextEnabled(true);

    if (m_currentImage == 0 && m_imagesList.count() != 0)
    {
      m_fullScreenWindow->setBackEnabled(false);
      return;
    }

    if ((m_currentImage + 1) == m_imagesList.count() && m_imagesList.count() != 0)
    {
      m_fullScreenWindow->setNextEnabled(false);
      return;
    }

  }
}


void ViewerWidget::zoomIn()
{
  m_fullScreenWindow->setBackEnabled(false);
  m_fullScreenWindow->setNextEnabled(false);
}


void ViewerWidget::showImages()
{
  nextImage();
}


void ViewerWidget::closeImages()
{
  m_currentImage = -1;
  m_imageSwitcher->setImCount("");
  m_imageSwitcher->setCurrentImNum("");
  m_fullScreenWindow->setCurrImgNumLabelText("");
  m_fullScreenWindow->setCountImgsLabelText("");
  closeImage();
  m_imagesList.clear();
}


void ViewerWidget::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)
  QPainter p(this);
  p.setBrush(QBrush(m_bgColor));
  p.fillRect(this->rect(), p.brush());
}


void ViewerWidget::mouseMoveEvent(QMouseEvent* event)
{
  event->ignore();
}


void ViewerWidget::mousePressEvent(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (m_imagesList.isEmpty())
    return;

  m_fullScreenWindow->showFullScreen();
  m_fullScreenWindow->setImage(m_imageViewer->pixmap(), m_imagesList[m_currentImage].second);
}


void ViewerWidget::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event)

  QPixmap pixmap(size());
  QPainter painter(&pixmap);
  painter.fillRect(pixmap.rect(), Qt::white);
  painter.setBrush(Qt::black);
  painter.drawRoundRect(pixmap.rect(), m_radius, m_radius);
  setMask(pixmap.createMaskFromColor(Qt::white));
}
