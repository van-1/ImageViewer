#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

// Qt
#include <QVBoxLayout>
#include <QLabel>
#include <QBitmap>

#include <iostream>

// Local
#include "../include/ImgViewerWidget.h"
#include "../src/ImageViewer.h"
#include "ImageSwitcher.h"

class ViewerWidget : public QWidget
{
  Q_OBJECT

  public:
    explicit ViewerWidget(QWidget* parent = nullptr);

    QString imageHeaderText() const;
    QString imageText() const;

  public slots:
    void setImagesList(const QList<QPair<QImage, QString>>& imagesList);
    void showImages();
    void closeImages();

  protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* event) override;

  private slots:
    bool showImage(QPixmap* pixmap);
    void openImageAtIndex(int i);
    void closeImage();
    void setImageHeaderText(QString headerText);
    void setImageText(QString imageText);
    void nextImage();
    void prvImage();
    void fullScreenNextImage();
    void fullScreenPrvImage();
    void zoomOut();
    void zoomIn();

  private:
    ImgViewerWidget* m_fullScreenWindow;
    QVBoxLayout m_layout;
    ImageViewer* m_imageViewer;
    QLabel* m_imageHeaderText;
    QLabel* m_imageText;
    QList<QPair<QImage,QString>> m_imagesList;
    QColor m_bgColor;
    ImageSwitcher* m_imageSwitcher;
    QPixmap* m_imagePixmap;
    int m_currentImage;
    const int m_radius = 10;
    const int m_maxImageHeaderTextLen = 256;
    const int m_maxImageTextLen = 512;
    const int m_textIndent = 40;
    const int m_textMargin = 40;
};

#endif // VIEWERWIDGET_H
