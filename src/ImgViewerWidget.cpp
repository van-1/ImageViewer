// Local
#include "include/ImgViewerWidget.h"
#include "FullScreenWindow.h"

ImgViewerWidget::ImgViewerWidget(QWidget* parent)
  : QWidget(parent)
  , m_viewerWidget(new FullScreenWindow)
{
  hide();

  m_viewerWidget->hide();
  connect(m_viewerWidget.get(), SIGNAL(nextImage()), this, SIGNAL(nextImage()));
  connect(m_viewerWidget.get(), SIGNAL(prvImage()), this, SIGNAL(prvImage()));
  connect(m_viewerWidget.get(), SIGNAL(zoomIn()), this, SIGNAL(zoomIn()));
  connect(m_viewerWidget.get(), SIGNAL(zoomOut()), this, SIGNAL(zoomOut()));
}


ImgViewerWidget::~ImgViewerWidget()
{}


bool ImgViewerWidget::rectImageFits() const
{
  return m_viewerWidget->rectImageFits();
}


void ImgViewerWidget::setImage(const QPixmap& fullScreenIm, const QString& imageTittle)
{
  m_viewerWidget->setPixmapForFullScreen(fullScreenIm, imageTittle);
}


void ImgViewerWidget::setNextEnabled(bool enabled)
{
  m_viewerWidget->setVisibleNextImgButton(enabled);
}


void ImgViewerWidget::setBackEnabled(bool enabled)
{
  m_viewerWidget->setVisiblePrvImgButton(enabled);
}


void ImgViewerWidget::setCountImgsLabelText(const QString& countImgs)
{
  m_viewerWidget->setCountImgsLabelText(countImgs);
}


void ImgViewerWidget::setCurrImgNumLabelText(const QString& currImg)
{
  m_viewerWidget->setCurrImgNumLabelText(currImg);
}


void ImgViewerWidget::showFullScreen()
{
  m_viewerWidget->showFullScreen();
}
