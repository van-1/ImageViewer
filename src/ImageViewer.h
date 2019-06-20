#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

// Qt
#include <QWidget>
#include <QDesktopWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QColor>
#include <QTimer>
#include <QApplication>

class ImageViewer : public QWidget
{
  Q_OBJECT

  public:
    enum ImageFitMode
    {
      FitWindow,
      FitWidth,
      FitOriginal,
      FitFree
    };

    ImageViewer(QWidget* parent = nullptr);
    ~ImageViewer();

    ImageFitMode fitMode() const;
    QRect imageRect() const;
    float currentScale() const;
    QSize sourceSize() const;
    QPixmap pixmap() const;
    bool sourceImageFits() const;
    bool rectImageFits() const;


  public slots:
    void displayImage(const QPixmap& image);
    //Calculating Scaling Ratio (qt doc)
    void setFitMode(ImageFitMode mode);
    void setFitOriginal();
    void setFitWidth();
    void setFitWindow();
    void zoomIn();
    void zoomOut();
    void scrollUp();
    void scrollDown();
    void scrollLeft();
    void scrollRight();
    void closeImage();
    void rotateClockWise();
    void rortateAntiClockWise();

  protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

  private slots:
    void hideCursor();
    void showCursor();

  private:
    void setScale(float scale);
    void updateMinScale();
    void scaleAroundZoomPoint(float oldScale);
    void fitNormal();
    void fitWidth();
    void fitWindow();
    void updateMaxScale();
    void centerImage();
    void snapEdgeHorizontal();
    void snapEdgeVertical();
    void scroll(int dx, int dy);
    int scrolledX(int dx);
    int scrolledY(int dy);
    void hideCursorTimed(bool restartTimer);
    void mouseDragWrapping(QMouseEvent* event);
    void readjust(QSize sourceSize, QRect drawingRect);
    void applyFitMode();
    void setZoomPoint(QPoint pos);
    void doZoomIn();
    void doZoomOut();
    void updateFitWindowScale();
    void rotateImage(float angle);

  private:
    QPixmap m_image;
    QTimer* m_cursorTimer;
    QRect m_drawingRect;
    QPoint m_mouseMoveStartPos;
    QPoint m_drawPos;
    QSize m_sourceSize;
    float m_fitWindowScale;
    float m_currentScale;
    float m_minScale;
    float m_maxScale;
    QPoint m_zoomPoint;
    QPointF m_zoomDrawRectPoint; // [0-1, 0-1]
    QSize m_desktopSize;
    ImageFitMode m_imageFitMode;
    const int m_cursorHideTimeout = 1000;
    const int m_scrollDistance = 250;
    const float m_maxScaleLimit = 4.0;
    const float m_maxResolutionLimit = 75.0; //default initvalue in megapixels
    const float m_zoomInStep = 1.25;
    const float m_zoomOutStep = 0.9;
};

#endif // IMAGEVIEWER_H
