// Local
#include "ui_fullscreenwindow.h"
#include "FullScreenWindow.h"

FullScreenWindow::FullScreenWindow(QWidget* parent)
  : QMainWindow(parent)
  , ui(new Ui::FullScreenWindow)
  , m_imageViewerFullScreen(new ImageViewer(this))
{
  ui->setupUi(this);
  setAutoFillBackground(false);
  setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  setAttribute(Qt::WA_TranslucentBackground);
  setAttribute(Qt::WA_ShowModal, true);

  QFont font = ui->closeButton->font();
  ui->m_prvImgBtn->setFont(font);
  ui->m_nextImgBtn->setFont(font);

  QSizePolicy spRetainPrvImgBtn = ui->m_prvImgBtn->sizePolicy();
  spRetainPrvImgBtn.setRetainSizeWhenHidden(true);
  ui->m_prvImgBtn->setSizePolicy(spRetainPrvImgBtn);

  QSizePolicy spRetainNextImgBtn = ui->m_nextImgBtn->sizePolicy();
  spRetainNextImgBtn.setRetainSizeWhenHidden(true);
  ui->m_nextImgBtn->setSizePolicy(spRetainNextImgBtn);
  ui->m_layout->insertWidget(1, m_imageViewerFullScreen);

  font.setPixelSize(16);
  ui->m_currImageLabel->setFont(font);
  ui->m_imageFromCountLabel->setFont(font);
  ui->m_imagesCountLabel->setFont(font);
  ui->m_imageLabel->setFont(font);

  m_imageViewerFullScreen->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
  connect(ui->m_prvImgBtn, SIGNAL(clicked()), this, SIGNAL(prvImage()));
  connect(ui->m_nextImgBtn, SIGNAL(clicked()), this, SIGNAL(nextImage()));
  connect(ui->closeButton, SIGNAL(clicked(bool)), this, SLOT(hide()));
  connect(ui->antiClockWisePushButton, SIGNAL(clicked()), this, SLOT(rotateImageAntiClockWise()));
  connect(ui->clockWisePushButton, SIGNAL(clicked()), this, SLOT(rotateImageClockWise()));
}


FullScreenWindow::~FullScreenWindow()
{
  delete ui;
}


bool FullScreenWindow::rectImageFits() const
{
  return m_imageViewerFullScreen->rectImageFits();
}


void FullScreenWindow::setPixmapForFullScreen(const QPixmap& fullScreenIm, const QString& imageLabel)
{
  m_imageViewerFullScreen->displayImage(fullScreenIm);
  m_imageViewerFullScreen->setFitWindow();
  ui->m_imageLabel->setText(imageLabel);
}


void FullScreenWindow::setVisibleNextImgButton(bool visible)
{
  ui->m_nextImgBtn->setVisible(visible);

  if (visible)
  {
    QSizePolicy spRetainNextImgBtn = ui->m_nextImgBtn->sizePolicy();
    spRetainNextImgBtn.setRetainSizeWhenHidden(true);
    ui->m_nextImgBtn->setSizePolicy(spRetainNextImgBtn);
  }
}


void FullScreenWindow::setVisiblePrvImgButton(bool visible)
{
  ui->m_prvImgBtn->setVisible(visible);

  if (visible)
  {
    QSizePolicy spRetainPrvImgBtn = ui->m_prvImgBtn->sizePolicy();
    spRetainPrvImgBtn.setRetainSizeWhenHidden(true);
    ui->m_prvImgBtn->setSizePolicy(spRetainPrvImgBtn);
  }
}


void FullScreenWindow::setCountImgsLabelText(const QString& countImgs)
{
  ui->m_imagesCountLabel->setText(countImgs);
}


void FullScreenWindow::setCurrImgNumLabelText(const QString& currImg)
{
  ui->m_currImageLabel->setText(currImg);
}


void FullScreenWindow::setWidgetsVisible(bool visible)
{
  ui->closeButton->setVisible(visible);
  ui->m_imageLabel->setVisible(visible);
  ui->m_currImageLabel->setVisible(visible);
  ui->m_imagesCountLabel->setVisible(visible);
  ui->m_imageFromCountLabel->setVisible(visible);
  ui->m_emptyLable->setVisible(visible);
  ui->clockWisePushButton->setVisible(visible);
  ui->antiClockWisePushButton->setVisible(visible);
  //ui->m_emptyLable1->setVisible(visible);

  QSizePolicy spRetainPrvImgBtn = ui->m_prvImgBtn->sizePolicy();
  spRetainPrvImgBtn.setRetainSizeWhenHidden(false);
  ui->m_prvImgBtn->setSizePolicy(spRetainPrvImgBtn);

  QSizePolicy spRetainNextImgBtn = ui->m_nextImgBtn->sizePolicy();
  spRetainNextImgBtn.setRetainSizeWhenHidden(false);
  ui->m_nextImgBtn->setSizePolicy(spRetainNextImgBtn);
}


void FullScreenWindow::rotateImageClockWise()
{
  m_imageViewerFullScreen->rotateClockWise();
}


void FullScreenWindow::rotateImageAntiClockWise()
{
  m_imageViewerFullScreen->rortateAntiClockWise();
}


void FullScreenWindow::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(20, 20, 20, 200));
  painter.drawRect(0, 0, width(), height());
}


void FullScreenWindow::wheelEvent(QWheelEvent* event)
{
  if (Qt::ControlModifier == QApplication::keyboardModifiers())
  {
    QPoint numPixels = event->pixelDelta();
    QPoint numDegrees = event->angleDelta() / 8;

     if (!numPixels.isNull())
     {
       if (numPixels.y() > 0)
       {
         m_imageViewerFullScreen->zoomIn();
         emit zoomIn();
         if (!m_imageViewerFullScreen->rectImageFits())
           setWidgetsVisible(false);
       }

       if (numPixels.y() < 0)
       {
         m_imageViewerFullScreen->zoomOut();
         emit zoomOut();
         if (m_imageViewerFullScreen->rectImageFits())
           setWidgetsVisible(true);
       }
     }
     else if (!numDegrees.isNull())
     {
       if (numDegrees.y() > 0)
       {
         m_imageViewerFullScreen->zoomIn();
         emit zoomIn();
         if (!m_imageViewerFullScreen->rectImageFits())
           setWidgetsVisible(false);
       }

       if (numDegrees.y() < 0)
       {
         m_imageViewerFullScreen->zoomOut();
         emit zoomOut();
         if (m_imageViewerFullScreen->rectImageFits())
           setWidgetsVisible(true);
       }
    }
  }
  event->accept();
}


void FullScreenWindow::keyPressEvent(QKeyEvent* event)
{
  if (event->key() == Qt::Key_Escape)
  {
    hide();
    m_imageViewerFullScreen->setFitWindow();
    setWidgetsVisible(true);
    return;
  }

  if (event->key() == Qt::Key_Down)
  {
    m_imageViewerFullScreen->scrollDown();
    return;
  }

  if (event->key() == Qt::Key_Up)
  {
    m_imageViewerFullScreen->scrollUp();
    return;
  }

  if (event->key() == Qt::Key_Right)
  {
    if (ui->closeButton->isVisible())
    {
      emit nextImage();
      return;
    }
    m_imageViewerFullScreen->scrollRight();
    return;
  }

  if (event->key() == Qt::Key_Left)
  {
    if (ui->closeButton->isVisible())
    {
      emit prvImage();
      return;
    }
    m_imageViewerFullScreen->scrollLeft();
    return;
  }

   // ctrl+-
   if (Qt::ControlModifier == QApplication::keyboardModifiers())
   {
     // 61 for ctrl+
     if (event->key() == Qt::Key_Equal)
     {
       m_imageViewerFullScreen->zoomIn();
       emit zoomIn();
       if (!m_imageViewerFullScreen->rectImageFits())
         setWidgetsVisible(false);
     }

     // ctrl-
     if (event->key() == Qt::Key_Minus)
     {
       m_imageViewerFullScreen->zoomOut();
       emit zoomOut();
       if (m_imageViewerFullScreen->rectImageFits())
         setWidgetsVisible(true);
     }

     if (event->key() == Qt::Key_0)
     {
       m_imageViewerFullScreen->setFitWindow();
       emit zoomOut();
       setWidgetsVisible(true);
     }
   }
}


void FullScreenWindow::mousePressEvent(QMouseEvent* eventPress)
{
  Q_UNUSED(eventPress)
  if (ui->closeButton->isVisible())
    hide();
}
