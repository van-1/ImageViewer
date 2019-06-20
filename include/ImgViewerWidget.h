#ifndef IMGVIEWERWIDGET_H
#define IMGVIEWERWIDGET_H

// Qt
#include <QWidget>

// STL
#include <memory>

class FullScreenWindow;

class ImgViewerWidget : public QWidget
{
  Q_OBJECT
  public:
    explicit ImgViewerWidget(QWidget* parent = nullptr);
    ~ImgViewerWidget();

    /*
    * @brief rectImageFits возвращает true если масштаб выводимого изображения позволяет его отрисовывать целиком в рабочей области
    * @return
    */
    bool rectImageFits() const;

  public slots:
    /*
    * @brief setImage передает в класс Pixmap, который необходимо отображат ьв полноэкранном режиме
    * @param fullScreenIm pixmap, который необходимо отрисовать в полноэкранном режиме
    * @param imageLabel подпись под изображением
    * @return
    */
    void setImage(const QPixmap& fullScreenIm, const QString& imageTittle);
    /*
    * @brief setNextEnabled скрывает/отображает клавишу >
    * @param visible true - отобразить кнопку, false-скрыть кнопку
    * @return
    */
    void setNextEnabled(bool enabled);
    /*
    * @brief setBackEnabled скрывает/отображает клавишу <
    * @param visible true - отобразить кнопку, false-скрыть кнопку
    * @return
    */
    void setBackEnabled(bool enabled);
    /*
    * @brief setCountImgsLabelText устанавливает текст подписи (в нижнем левом углу окна <m> из <n>) общего количества изображений <m> из <n>
    * @param countImgs текст подписи общего количества изображений - <n>
    * @return
    */
    void setCountImgsLabelText(const QString& countImgs);
    /*
    * @brief setCurrImgNumLabelText устанавливает текст подписи (в нижнем левом углу окна <m> из <n>) текущего изображения
    * @param currImg текст подписи общего количества изображений - <m>
    * @return
    */
    void setCurrImgNumLabelText(const QString& currImg);
    /*
    * @brief showFullScreen отображает окно просмотра заданного изображения в полноэкранном режиме
    * @return
    */
    void showFullScreen();

  signals:
    //! Сигнал генерируется при нажатии кнопки >
    void nextImage();
    //! Сигнал генерируется при нажатии кнопки <
    void prvImage();
    //! Сигнал генерируется при увеличении масштаба картинки
    void zoomIn();
    //! Сигнал генерируется при уменьшении масштаба картинки
    void zoomOut();

  private:
    std::unique_ptr<FullScreenWindow> m_viewerWidget;
};

#endif // IMGVIEWERWIDGET_H
