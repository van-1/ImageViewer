#ifndef FULLSCREENWINDOW_H
#define FULLSCREENWINDOW_H

// Qt
#include <QMainWindow>
#include <QPainter>
#include <QKeyEvent>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

// Local
#include "ImageViewer.h"

namespace Ui
{
  class FullScreenWindow;
}

class FullScreenWindow : public QMainWindow
{
  Q_OBJECT

  public:
    explicit FullScreenWindow(QWidget* parent = nullptr);
    ~FullScreenWindow();
    bool rectImageFits() const;

  public slots:
    void setPixmapForFullScreen(const QPixmap& fullScreenIm, const QString& imageLabel);
    void setVisibleNextImgButton(bool visible);
    void setVisiblePrvImgButton(bool visible);
    void setCountImgsLabelText(const QString& countImgs);
    void setCurrImgNumLabelText(const QString& currImg);

  signals:
    void nextImage();
    void prvImage();
    void zoomIn();
    void zoomOut();

  protected:
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void mousePressEvent(QMouseEvent* eventPress) override;

  private slots:
    void setWidgetsVisible(bool visible);
    void rotateImageClockWise();
    void rotateImageAntiClockWise();

  private:
    Ui::FullScreenWindow* ui;
    ImageViewer* m_imageViewerFullScreen;
};

#endif // FULLSCREENWINDOW_H
