#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QImageWriter>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT //Required macro for dynamic behaviour, real time events and signals/slots

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushButtonASCII_clicked();

    void on_pushButtonFilters_clicked();

    void on_blackAndWhiteRadioButton_toggled(bool checked);

    void on_blurRadioButton_toggled(bool checked);

    void on_contrastRadioButton_toggled(bool checked);

    void on_saturationRadioButton_toggled(bool checked);

    void on_noFilterRadioButton_toggled(bool checked);

    void on_blackAndWhiteSlider_valueChanged(int value);

    void on_nonNormalSlider_valueChanged(int value);

    void on_downloadPushButton_clicked();

    void on_temperatureRadioButton_toggled(bool checked);

private:
    QImage filteredImage;
    QString filtersImageURL;
    float normalFactor;
    float nonNormalFactor;

    Ui::MainWindow *ui;
    bool loadImage(QImage &inputImage, QString &inputFileName, QLabel &errorLabel);
    bool imageToPixmap(QImage &inputImage, QPixmap &inputPixmap, QLabel &errorLabel);
    bool validateSize(const QString &input, int &artSize);
    void createASCIIArt(QImage &image, const int &artSize);
    int linearContrast(const int midpoint, float factor, int colourValue);
    void clampRGB(int &value);
    void blurSum(QImage &inputImage, int x_coord, int y_coord, int &redSum, int &greenSum, int &blueSum, int &counts);
};
#endif // MAINWINDOW_H
