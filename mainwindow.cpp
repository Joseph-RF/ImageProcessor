#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    normalFactor = 0.;
    nonNormalFactor = 1.;

    QString noticeString = "Note: Black&White and Temperature filters\nare affected by the upper scroll bar.\n";
    noticeString += "Blur, Contrast and Saturation filters are affected by the\nlower scroll bar.";

    ui->noticeLabelFilters->setText(noticeString);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::loadImage(QImage &inputImage, QString &inputFileName, QLabel &errorLabel)
{
    if (!inputImage.load(inputFileName)) {
        qInfo()<<"File failed to load properly";
        errorLabel.setText("File failed to load properly.");
        return false;
    }
    return true;
}

bool MainWindow::imageToPixmap(QImage &inputImage, QPixmap &inputPixmap, QLabel &errorLabel)
{
    if (!inputPixmap.convertFromImage(inputImage)) {
        qInfo()<<"File could not be converted to a pixmap.";
        errorLabel.setText("File could not be converted to a\npixmap.");
        return false;
    }
    return true;
}

void MainWindow::on_pushButtonASCII_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), "C:/Users/Joseph Rodriguez/Pictures/ProjectImages", tr("Image Files (*.png *.jpg *.bmp)"));

    if (fileName.isNull() || fileName.isEmpty()) {
        qInfo()<<"No file selected";
        ui->errorLabelASCII->setText("No file selected.");
        return;
    } else {
        qInfo()<<"File selected was: "<<fileName;
    }

    QImage image;

    if (!loadImage(image, fileName, *ui->errorLabelASCII)) return;

    int asciiArtSize = 100;

    if (validateSize(ui->widthLineEdit->text(), asciiArtSize)) {
        createASCIIArt(image, asciiArtSize);
    }
}

bool MainWindow::validateSize(const QString &input, int &artSize)
{
    bool validConversion = false;
    input.toInt(&validConversion);

    if (!validConversion) {
        ui->errorLabelASCII->setText("Ascii art size must be an integer value greater than 0\nand less than 1000.");
        return false;
    }

    artSize = input.toInt();

    if (artSize > 1000 || artSize < 1){
        ui->errorLabelASCII->setText("Ascii art size must be an integer value greater than 0\nand less than 1000.");
        return false;
    } else {
        return true;
    }
}

void MainWindow::createASCIIArt(QImage &image, const int &artSize)
{
    const float artSize_f = static_cast<float>(artSize);

    int imageWidth = image.width();
    int imageHeight = image.height();

    float scaleFactor = 0.0;
    if (imageWidth >= imageHeight) {
        scaleFactor = artSize_f / imageWidth;
    } else {
        scaleFactor = artSize_f / imageHeight;
    }

    QImage scaledImage = image.scaled(imageWidth * scaleFactor, imageHeight * scaleFactor / 2, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    int scaledImageWidth = scaledImage.width();
    int scaledImageHeight = scaledImage.height();

    const char ascii_chars[10] = {' ', '.', ':', '-', '=', '+', '*', '#', '%', '@'};

    std::vector<std::vector<char>> output (artSize, std::vector<char>(round(artSize / 2.)));

    for (int j = 0; j < scaledImageHeight; ++j) {
        QRgb *line = reinterpret_cast<QRgb*>(scaledImage.scanLine(j));
        for (int i = 0; i < scaledImageWidth; ++i) {
            QRgb pixel = line[i];
            int brightness = qRed(pixel) + qGreen(pixel) + qBlue(pixel);

            int index = floor(brightness / 76.5);

            if (brightness == 765) index = 9;

            output[i][j] = ascii_chars[index];
        }
    }

    QString outputstring = "";

    for (int j = 0; j < scaledImageHeight; ++j) {
        for (int i = 0; i < scaledImageWidth; ++i) {
            outputstring.append(QString(output[i][j]));
        }
        outputstring.append(QString("\n"));
    }

    ui->plainTextEdit->setReadOnly(true);
    QPalette *p = new QPalette;
    p->setColor(QPalette::Base, Qt::black);
    p->setColor(QPalette::Text, Qt::white);

    ui->plainTextEdit->setPalette(*p);
    ui->plainTextEdit->setPlainText(outputstring);

    ui->plainTextEdit->zoomOut(1);
}


void MainWindow::on_pushButtonFilters_clicked()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,
                                            tr("Open Image"), "C:/Users/Joseph Rodriguez/Pictures/ProjectImages", tr("Image Files (*.png *.jpg *.bmp)"));

    if (fileName.isNull() || fileName.isEmpty()) {
        qInfo()<<"No file selected";
        ui->errorLabelFilters->setText("No file selected.");
        return;
    } else {
        qInfo()<<"File selected was: "<<fileName;
    }

    QImage image;
    if (!loadImage(image, fileName, *ui->errorLabelFilters)) return;

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    filtersImageURL = fileName;

    //Resize label to avoid distorting the image.
    float factor = 0.1f;
    float increment = 0.05f;

    ui->imageLabelFilters->setGeometry(ui->imageLabelFilters->x(), ui->imageLabelFilters->y(), 720, 430); //Original dimensions

    while (image.width() * (factor + increment) < ui->imageLabelFilters->width() && image.height() * (factor + increment) < ui->imageLabelFilters->height()) {
        factor += increment;
        qInfo()<<factor;
    }

    ui->imageLabelFilters->setGeometry(ui->imageLabelFilters->x(), ui->imageLabelFilters->y(), image.width() * factor, image.height() * factor);

    ui->imageLabelFilters->setPixmap(pm);
    ui->imageLabelFilters->setScaledContents(true);
}



void MainWindow::on_blackAndWhiteRadioButton_toggled(bool checked)
{
    QImage image;

    if (!loadImage(image, filtersImageURL, *ui->errorLabelFilters)) return;

    int imageWidth = image.width();
    int imageHeight = image.height();

    for (int j = 0; j < imageHeight; ++j) {
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(j));
        for (int i = 0; i < imageWidth; ++i) {
            QRgb pixel = line[i];
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            float average = (red + green + blue) / 3.;

            red = round(red - normalFactor * (red - average));
            clampRGB(red);

            green = round(green - normalFactor * (green - average));
            clampRGB(green);

            blue = round(blue - normalFactor * (blue - average));
            clampRGB(blue);

            line[i] = qRgb(red, green, blue);
        }
    }

    filteredImage = image.copy();

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    ui->imageLabelFilters->setPixmap(pm);
}

void MainWindow::on_blackAndWhiteSlider_valueChanged(int value)
{
    normalFactor = value / 1000.;

    if (ui->blackAndWhiteRadioButton->isChecked()) {
        on_blackAndWhiteRadioButton_toggled(true);
    } else if (ui->temperatureRadioButton->isChecked()) {
        on_temperatureRadioButton_toggled(true);
    }
}

void MainWindow::on_blurRadioButton_toggled(bool checked)
{
    int blurRadius = round(nonNormalFactor);
    QImage image;

    if (!loadImage(image, filtersImageURL, *ui->errorLabelFilters)) return;

    int imageWidth = image.width();
    int imageHeight = image.height();

    for (int j = 0; j < imageHeight; ++j) {
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(j));

        for (int i = 0; i < imageWidth; ++i) {

            int qRedSum = 0, redAverage = 0;
            int qGreenSum = 0, greenAverage = 0;
            int qBlueSum = 0, blueAverage = 0;
            int counts = 0;

            for (int y = -blurRadius; y <= blurRadius; ++y) {
                for (int x = -blurRadius; x <= blurRadius; ++x) {
                    blurSum(image, i + x, j + y, qRedSum, qGreenSum, qBlueSum, counts);
                }
            }
            redAverage = round(qRedSum / counts * 1.);
            greenAverage = round(qGreenSum / counts * 1.);
            blueAverage = round(qBlueSum / counts * 1.);

            line[i] = qRgb(redAverage, greenAverage, blueAverage);
        }
    }

    filteredImage = image.copy();

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    ui->imageLabelFilters->setPixmap(pm);
}

void MainWindow::on_contrastRadioButton_toggled(bool checked)
{
    QImage image;
    const int MIDPOINT = 128;
    float contrastFactor = nonNormalFactor;

    if (!loadImage(image, filtersImageURL, *ui->errorLabelFilters)) return;

    int imageWidth = image.width();
    int imageHeight = image.height();

    for (int j = 0; j < imageHeight; ++j) {
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(j));
        for (int i = 0; i < imageWidth; ++i) {
            QRgb pixel = line[i];

            line[i] = qRgb(linearContrast(MIDPOINT, contrastFactor, qRed(pixel)), linearContrast(MIDPOINT, contrastFactor, qGreen(pixel)),
                           linearContrast(MIDPOINT, contrastFactor, qBlue(pixel)));
        }
    }

    filteredImage = image.copy();

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    ui->imageLabelFilters->setPixmap(pm);
}

int MainWindow::linearContrast(const int midpoint, float factor, int colourValue)
{
    int newColourValue = round(midpoint + (colourValue - 128) * factor);

    if (newColourValue > 255) {
        return 255;
    } else if (newColourValue < 0) {
        return 0;
    } else {
        return newColourValue;
    }
}


void MainWindow::on_saturationRadioButton_toggled(bool checked)
{
    //Convert RGB values of every pixel to HSL
    QImage image;
    float saturationFactor = nonNormalFactor;

    if(!loadImage(image, filtersImageURL, *ui->errorLabelFilters)) return;

    int imageWidth = image.width();
    int imageHeight = image.height();

    for (int j = 0; j < imageHeight; ++j) {
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(j));
        for (int i = 0; i < imageWidth; ++i) {
            QColor colour(qRed(line[i]), qGreen(line[i]), qBlue(line[i]));

            int hue, saturation, lightness, red, blue, green;
            colour.getHsl(&hue, &saturation, &lightness);

            saturation *= saturationFactor;

            if (saturation > 255) {
                saturation = 255;
            } else if (saturation < 0) {
                saturation = 0;
            }

            colour.setHsl(hue, saturation, lightness);
            colour.getRgb(&red, &green, &blue);

            line[i] = qRgb(red, green, blue);
        }
    }

    filteredImage = image.copy();

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    ui->imageLabelFilters->setPixmap(pm);
}

void MainWindow::on_noFilterRadioButton_toggled(bool checked)
{
    QImage image;
    if (!loadImage(image, filtersImageURL, *ui->errorLabelFilters)) return;

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    ui->imageLabelFilters->setPixmap(pm);
    ui->imageLabelFilters->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void MainWindow::on_nonNormalSlider_valueChanged(int value)
{
    nonNormalFactor = value / 1000.;

    if (ui->saturationRadioButton->isChecked()) {
        on_saturationRadioButton_toggled(true);
    } else if (ui->contrastRadioButton->isChecked()) {
        on_contrastRadioButton_toggled(true);
    } else if (ui->blurRadioButton->isChecked() && value >= 1) {
        on_blurRadioButton_toggled(true);
    }
}


void MainWindow::on_downloadPushButton_clicked()
{
    QString saveFileLocation= QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "C:/Users/Joseph Rodriguez/Pictures/ProjectImages/untitled.png",
                                                    tr("Images (*.png *.bmp *.jpg)"));

    if (saveFileLocation.isNull() || saveFileLocation.isEmpty()) {
        qInfo()<<"Save file could not be made.";
        ui->errorLabelFilters->setText("Save file could not be saved.");
        return;
    }

    QImageWriter imageWriter(saveFileLocation);

    if (imageWriter.write(filteredImage)) {
        qInfo()<<"Filtered image saved successfully.";
        ui->errorLabelFilters->setText("Filtered image saved succesfully.");
    }
}

void MainWindow::on_temperatureRadioButton_toggled(bool checked)
{
    QImage image;
    if (!loadImage(image, filtersImageURL, *ui->errorLabelFilters)) return;

    int imageWidth = image.width();
    int imageHeight = image.height();

    for (int j = 0; j < imageHeight; ++j) {
        QRgb *line = reinterpret_cast<QRgb*>(image.scanLine(j));
        for (int i = 0; i < imageWidth; ++i) {
            QRgb pixel = line[i];
            int red = qRed(pixel);
            int green = qGreen(pixel);
            int blue = qBlue(pixel);

            if (normalFactor > 0) {
                red = round(red + normalFactor * 40.);
                clampRGB(red);

                green = round(green + normalFactor * 40. * 0.5f);
                clampRGB(green);

                blue = round(blue - normalFactor * 40. * 0.5f);
                clampRGB(blue);
            } else {
                //Note normal factor will be negative here.
                red = round(red + normalFactor * 40. * 0.5f);
                clampRGB(red);

                green = round(green + normalFactor * 40. * 0.5f);
                clampRGB(green);

                blue = round(blue - normalFactor * 40.);
                clampRGB(blue);
            }

            line[i] = qRgb(red, green, blue);
        }
    }

    filteredImage = image.copy();

    QPixmap pm;
    if (!imageToPixmap(image, pm, *ui->errorLabelFilters)) return;

    ui->imageLabelFilters->setPixmap(pm);
}

void MainWindow::clampRGB(int &value)
{
    if (value > 255) {
        value = 255;
    } else if (value < 0) {
        value = 0;
    }
}

void MainWindow::blurSum(QImage &inputImage, int x_coord, int y_coord, int &redSum, int &greenSum, int &blueSum, int &counts)
{
    const int imageWidth = inputImage.width();
    const int imageHeight = inputImage.height();

    if (x_coord < 0 || y_coord < 0) return;
    else if (x_coord >= imageWidth || y_coord >= imageHeight) return;

    QRgb currentPixel = inputImage.pixel(x_coord, y_coord);

    redSum += qRed(currentPixel);
    greenSum += qGreen(currentPixel);
    blueSum += qBlue(currentPixel);

    counts++;
}
