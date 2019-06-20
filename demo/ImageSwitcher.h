#ifndef IMAGESWITCHER_H
#define IMAGESWITCHER_H

// Qt
#include <QWidget>

namespace Ui
{
  class ImageSwitcher;
}

class ImageSwitcher : public QWidget
{
    Q_OBJECT

  public:
    explicit ImageSwitcher(QWidget* parent = nullptr);
    ~ImageSwitcher();

  public slots:
    void setCurrentImNum(QString currImNum);
    void setImCount(QString imgsCount);
    void setVisibleNextImgButton(bool visible);
    void setVisiblePrvImgButton(bool visible);

  signals:
    void nextImageClicked();
    void prvImageClicked();

  private slots:
    void on_nextImageButton_clicked();
    void on_prvImageButton_clicked();

  private:
    Ui::ImageSwitcher* ui;
};

#endif // IMAGESWITCHER_H
