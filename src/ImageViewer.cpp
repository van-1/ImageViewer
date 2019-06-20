// Local
#include "ImageViewer.h"

// Qt
#include <QtMath>

ImageViewer::ImageViewer(QWidget* parent)
  : QWidget(parent)
  , m_fitWindowScale(0.125)
  , m_currentScale(1.0)
  , m_minScale(0.125)
  , m_maxScale(m_maxScaleLimit)
  , m_imageFitMode(FitOriginal)

{
  this->setMouseTracking(true);
  m_cursorTimer = new QTimer(this);

  connect(m_cursorTimer, SIGNAL(timeout()), this, SLOT(hideCursor()));
  m_desktopSize = QApplication::desktop()->size();
}


ImageViewer::~ImageViewer()
{}


void ImageViewer::displayImage(const QPixmap& image)
{
  if (!image.isNull())
  {
    m_image = image;
    readjust(m_image.size(), m_image.rect());
    update();
    hideCursorTimed(false);
  }
}


void ImageViewer::closeImage()
{
  update();
  showCursor();
}


void ImageViewer::rotateClockWise()
{
  rotateImage(90);
}


void ImageViewer::rortateAntiClockWise()
{
  rotateImage(-90);
}


void ImageViewer::readjust(QSize sourceSize, QRect drawingRect)
{
  m_sourceSize  = sourceSize;
  m_drawingRect =  drawingRect;

  updateMinScale();
  updateMaxScale();
  setScale(1.0);
  applyFitMode();
}


void ImageViewer::scrollUp()
{
  scroll(0, -m_scrollDistance);
}


void ImageViewer::scrollDown()
{
  scroll(0, m_scrollDistance);
}


void ImageViewer::scrollLeft()
{
  scroll(-m_scrollDistance, 0);
}


void ImageViewer::scrollRight()
{
  scroll(m_scrollDistance, 0);
}


void ImageViewer::updateFitWindowScale()
{
  float newMinScaleX = float(width() * devicePixelRatioF() / m_sourceSize.width());
  float newMinScaleY = float(height() * devicePixelRatioF() / m_sourceSize.height());

  m_fitWindowScale = newMinScaleY;
  if (newMinScaleX < newMinScaleY)
      m_fitWindowScale = newMinScaleX;
}


void ImageViewer::rotateImage(float angle)
{
  QTransform transform;
  transform.rotate(angle);
  m_image = m_image.transformed(transform);
  readjust(m_image.size(), m_image.rect());
  update();
}


bool ImageViewer::sourceImageFits() const
{
  bool fitWidth = m_sourceSize.width() < width() * devicePixelRatioF();
  bool fitHeight = m_sourceSize.height() < height() * devicePixelRatioF();
  return fitWidth && fitHeight;
}


void ImageViewer::updateMinScale()
{
  updateFitWindowScale();
  m_minScale = m_fitWindowScale;
  if (sourceImageFits())
    m_minScale = 1.0;
}


void ImageViewer::updateMaxScale()
{
  m_maxScale = m_maxScaleLimit;
  float srcRes = float(m_sourceSize.width() * m_sourceSize.height() / 1000000);
  float maxRes = float(m_maxScale * m_maxScale * m_sourceSize.width() * m_sourceSize.height() / 1000000);
  if (maxRes > m_maxResolutionLimit)
    m_maxScale = float((m_maxResolutionLimit / srcRes));
}


void ImageViewer::setScale(float scale)
{
  m_currentScale = scale;
  if (scale > m_maxScale)
  {
    m_currentScale = m_maxScale;
  }
  else if (scale <= m_minScale)
  {
    m_currentScale = m_minScale;
    if (m_imageFitMode == FitFree && m_currentScale == m_fitWindowScale)
      m_imageFitMode = FitWindow;
  }

  float w = scale * m_sourceSize.width();
  float h = scale * m_sourceSize.height();
  m_drawingRect.setWidth(w);
  m_drawingRect.setHeight(h);
}


void ImageViewer::paintEvent(QPaintEvent* event)
{
  Q_UNUSED(event)
  QPainter painter(this);

  m_image.setDevicePixelRatio(devicePixelRatioF());

  QRectF dpiAdjusted(m_drawingRect.topLeft() / devicePixelRatioF(),
                     m_drawingRect.size() / devicePixelRatioF());

  painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
  painter.drawPixmap(dpiAdjusted, m_image, m_image.rect());
}


void ImageViewer::mousePressEvent(QMouseEvent* event)
{
  QWidget::mousePressEvent(event);

  showCursor();
  setCursor(QCursor(Qt::ArrowCursor));

  m_mouseMoveStartPos = event->pos();
  if (event->button() == Qt::LeftButton)
    setCursor(QCursor(Qt::ClosedHandCursor));
}


void ImageViewer::mouseMoveEvent(QMouseEvent* event)
{
  if (event->buttons() & Qt::LeftButton)
  {
    mouseDragWrapping(event);
  }
  else
  {
    showCursor();
    hideCursorTimed(true);
  }
}


void ImageViewer::mouseReleaseEvent(QMouseEvent* event)
{
  QWidget::mouseReleaseEvent(event);

  hideCursorTimed(false);
  setCursor(QCursor(Qt::ArrowCursor));
}


void ImageViewer::mouseDragWrapping(QMouseEvent* event)
{
  bool fitWidth = m_drawingRect.size().width() > width() * devicePixelRatioF();
  bool fitHeight = m_drawingRect.size().height() > height() * devicePixelRatioF();

  if (fitWidth || fitHeight)
  {
    bool wrapped = false;
    QPoint newPos = mapToGlobal(event->pos());
    QPoint delta = m_mouseMoveStartPos - event->pos();
    if (delta.x() && abs(delta.x()) < m_desktopSize.width() / 2)
    {
      int left = m_drawingRect.x() - delta.x();
      int right = left + m_drawingRect.width();
      if (left <= 0 && right > (width() * devicePixelRatioF()))
      {
        if ((left + 1) <= 0 && (right - 1) > (width() * devicePixelRatioF()))
        {
          if (newPos.x() >= (m_desktopSize.width() - 1))
          {
            newPos.setX(2);
            cursor().setPos(newPos);
            wrapped = true;
          }
          else if (newPos.x() <= 0)
          {
            newPos.setX(m_desktopSize.width() - 2);
            cursor().setPos(newPos);
            wrapped = true;
          }
        }
        m_drawingRect.moveLeft(left);
      }
    }
    if (delta.y() && abs(delta.y()) < (m_desktopSize.height() / 2))
    {
      int top = m_drawingRect.y() - delta.y();
      int bottom = top + m_drawingRect.height();
      if (top <= 0 && bottom > (height() * devicePixelRatioF()))
      {
        if ((top + 1) <= 0 && (bottom - 1) > (height() * devicePixelRatioF()))
        {
          if (newPos.y() >= (m_desktopSize.height() - 1))
          {
            newPos.setY(2);
            cursor().setPos(newPos);
            wrapped = true;
          }
          else if (newPos.y() <= 0)
          {
            newPos.setY(m_desktopSize.height() - 2);
            cursor().setPos(newPos);
            wrapped = true;
          }
        }
        m_drawingRect.moveTop(top);
      }
    }

    if (wrapped)
      m_mouseMoveStartPos = mapFromGlobal(newPos);
    else
      m_mouseMoveStartPos = event->pos();
    update();
  }
}


void ImageViewer::fitWidth()
{
  float scale = float (width() * devicePixelRatioF() / m_sourceSize.width());
  if (scale > 1.0)
  {
    fitNormal();
  }
  else
  {
    setScale(scale);
    centerImage();
    if (m_drawingRect.height() > (height() * devicePixelRatioF()))
      m_drawingRect.moveTop(0);
    update();
  }
}


void ImageViewer::fitWindow()
{
  bool fitWidth = m_sourceSize.width() <= width() * devicePixelRatioF();
  bool fitHeight = m_sourceSize.height() <= height() * devicePixelRatioF();

  if (fitWidth && fitHeight)
  {
    fitNormal();
    return;
  }
  else
  {
    setScale(m_minScale);
    centerImage();
    update();
  }
}


void ImageViewer::fitNormal()
{
  setScale(1.0);
  centerImage();
  if (m_drawingRect.height() > height() * devicePixelRatioF())
    m_drawingRect.moveTop(0);

  update();
}


void ImageViewer::setFitMode(ImageFitMode newMode)
{
  m_imageFitMode = newMode;
  applyFitMode();
}


void ImageViewer::applyFitMode()
{
  if (m_imageFitMode == FitOriginal)
  {
    fitNormal();
    return;
  }

  if (m_imageFitMode == FitWidth)
  {
    fitWidth();
    return;
  }

  fitWindow();
}


void ImageViewer::setFitOriginal()
{
  setFitMode(FitOriginal);
}


void ImageViewer::setFitWidth()
{
  setFitMode(FitWidth);
}


void ImageViewer::setFitWindow()
{
  setFitMode(FitWindow);
}


void ImageViewer::resizeEvent(QResizeEvent* event)
{
  Q_UNUSED(event)
  updateMinScale();

  if (m_imageFitMode == FitFree || m_imageFitMode == FitOriginal)
    centerImage();
  else
    applyFitMode();

  update();
}


void ImageViewer::centerImage()
{
  snapEdgeVertical();
  snapEdgeHorizontal();

  if (m_drawingRect.height() <= height() * devicePixelRatioF())
    m_drawingRect.moveTop((height() * devicePixelRatioF() - m_drawingRect.height()) / 2);

  if (m_drawingRect.width() <= width() * devicePixelRatioF())
    m_drawingRect.moveLeft((width() * devicePixelRatioF() - m_drawingRect.width()) / 2);
}


void ImageViewer::snapEdgeHorizontal()
{
  if (m_drawingRect.x() > 0 && m_drawingRect.right() > width()*devicePixelRatioF())
    m_drawingRect.moveLeft(0);

  if ((width() * devicePixelRatioF() - m_drawingRect.x()) > m_drawingRect.width())
    m_drawingRect.moveRight(width() * devicePixelRatioF());
}


void ImageViewer::snapEdgeVertical()
{
  if (m_drawingRect.y() > 0 && m_drawingRect.bottom() > height() * devicePixelRatioF())
    m_drawingRect.moveTop(0);

  if ((height() * devicePixelRatioF() - m_drawingRect.y()) > m_drawingRect.height())
    m_drawingRect.moveBottom(height() * devicePixelRatioF());
}


void ImageViewer::scroll(int dx, int dy)
{
  QPoint destTopLeft = m_drawingRect.topLeft();
  if (m_drawingRect.size().width() > width() * devicePixelRatioF())
    destTopLeft.setX(scrolledX(dx));

  if (m_drawingRect.size().height() > height() * devicePixelRatioF())
    destTopLeft.setY(scrolledY(dy));

  m_drawingRect.moveTopLeft(destTopLeft);
  update();
}


int ImageViewer::scrolledX(int dx)
{
  int newXPos = m_drawingRect.left();
  if (dx)
  {
    int left = m_drawingRect.x() - dx;
    int right = left + m_drawingRect.width();
    if (left > 0)
      left = 0;
    else if (right <= width() * devicePixelRatioF())
      left = width() * devicePixelRatioF() - m_drawingRect.width();
    if (left <= 0) newXPos = left;
  }
  return newXPos;
}


int ImageViewer::scrolledY(int dy)
{
  int newYPos = m_drawingRect.top();
  if (dy)
  {
    int top = m_drawingRect.y() - dy;
    int bottom = top + m_drawingRect.height();
    if (top > 0)
      top = 0;
    else if (bottom <= height() * devicePixelRatioF())
      top = height() * devicePixelRatioF() - m_drawingRect.height();
    if (top <= 0)
      newYPos = top;
  }
  return newYPos;
}


void ImageViewer::hideCursorTimed(bool restartTimer)
{
  if (restartTimer || !m_cursorTimer->isActive())
    m_cursorTimer->start(m_cursorHideTimeout);
}


void ImageViewer::setZoomPoint(QPoint pos)
{
  m_zoomPoint = pos;
  m_zoomDrawRectPoint.setX(float((m_zoomPoint.x() - m_drawingRect.x())) / m_drawingRect.width());
  m_zoomDrawRectPoint.setY(float((m_zoomPoint.y() - m_drawingRect.y())) / m_drawingRect.height());
}


void ImageViewer::scaleAroundZoomPoint(float newScale)
{
  setScale(newScale);
  float mappedX = m_drawingRect.width() * m_zoomDrawRectPoint.x() + m_drawingRect.left();
  float mappedY = m_drawingRect.height() * m_zoomDrawRectPoint.y() + m_drawingRect.top();
  int diffX = mappedX - m_zoomPoint.x();
  int diffY = mappedY - m_zoomPoint.y();
  m_drawingRect.moveLeft(m_drawingRect.left() - diffX);
  m_drawingRect.moveTop(m_drawingRect.top() - diffY);
  centerImage();
}


void ImageViewer::zoomIn()
{
  setZoomPoint(rect().center() * devicePixelRatioF());
  doZoomIn();
}


void ImageViewer::zoomOut()
{
  setZoomPoint(rect().center() * devicePixelRatioF());
  doZoomOut();
}


void ImageViewer::doZoomIn()
{
  float newScale = m_currentScale * m_zoomInStep;
  if (newScale == m_currentScale)
    return;

  if (newScale > m_maxScale)
    newScale = m_maxScale;
  m_imageFitMode = FitFree;

  scaleAroundZoomPoint(newScale);
  update();
}


void ImageViewer::doZoomOut()
{
  float newScale = m_currentScale * m_zoomOutStep;
  if (newScale == m_currentScale)
    return;

  if (newScale < m_minScale)
    newScale = m_minScale;

  m_imageFitMode = FitFree;
  scaleAroundZoomPoint(newScale);
  update();
}


ImageViewer::ImageFitMode ImageViewer::fitMode() const
{
  return m_imageFitMode;
}


QRect ImageViewer::imageRect() const
{
  return m_drawingRect;
}


float ImageViewer::currentScale() const
{
  return m_currentScale;
}


QSize ImageViewer::sourceSize() const
{
  return m_sourceSize;
}


QPixmap ImageViewer::pixmap() const
{
  return m_image;
}


bool ImageViewer::rectImageFits() const
{
  bool fitWidth = m_drawingRect.width() <= width() * devicePixelRatioF();
  bool fitHeight = m_drawingRect.height() <= height() * devicePixelRatioF();
  return fitWidth && fitHeight;
}


void ImageViewer::hideCursor()
{
  m_cursorTimer->stop();
  if (this->underMouse())
    setCursor(QCursor(Qt::BlankCursor));
}


void ImageViewer::showCursor()
{
  m_cursorTimer->stop();
  setCursor(QCursor(Qt::ArrowCursor));
}
