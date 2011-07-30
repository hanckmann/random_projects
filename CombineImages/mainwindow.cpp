#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

#include <QFileInfo>
#include <QFileDialog>
#include <QString>
#include <QSize>
#include <QMatrix>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    target_folder("")
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_buttonSetTargetFolder_clicked()
{
    // Ask for target folder
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    // Sanity check
    if(dir=="")
        return;

    // Save the target folder in memory
    target_folder.setPath(dir);

    // Update status
    QString label("Target folder: ");
    label += target_folder.absolutePath();
    ui->label->setText(label);
}

void MainWindow::on_buttonSelectPictures_clicked()
{
    // Ask for the images to combine
    QStringList files = QFileDialog::getOpenFileNames(this,
                                                      "Open images",
                                                      "/home",
                                                      "Image Files (*.png *.jpg *.bmp)");
    // If there are no images selected, stop
    if(files.size() <= 0)
        return;

    // If no target folder set, use the folder of the input images
    if(target_folder == QDir(""))
        target_folder = QFileInfo(files.at(1)).absoluteDir();

    // Update status
    ui->progressBar->setMaximum(files.size()*2);
    ui->label->setText(target_folder.absolutePath());

    // Sort the files on filesize
    QList<QString> file_list = sortFiles(files);

    // Combine the files
    combineImages(file_list);
}

QList<QString> MainWindow::sortFiles(const QStringList& files)
{
    QList<QString> file_list;
    QList<int> width_list;
    QStringList lof = files;

    // Update status
    ui->label->setText("Checking file sizes.");
    // Sort (remaining) images
    while(lof.size() > 0) {
        // Open image
        QImage img(lof.first());
        // Determine the width (smallest image side)
        int w = std::min(img.width(),img.height());
        // Sort the images based on the width (largest first)
        int index = 0;
        while(index < width_list.size()) {
            if(w > width_list.at(index))
                break;
            ++index;
        }

        file_list.insert(index,lof.first());
        width_list.insert(index,w);
        lof.removeFirst();

        // Update states
        ui->progressBar->setValue(ui->progressBar->value()+1);
    }
    return file_list;
}

void MainWindow::combineImages(QList<QString>& file_list)
{
    int count = 1;
    while(file_list.size() > 2) {
        QString label;
        if(file_list.size() > 1) {
            QString file1 = file_list.first();
            QString file2 = file_list.first();
            file_list.removeFirst();
            file_list.removeFirst();
            QString file3;
            file3 = target_folder.absolutePath();
            file3 += "/combined_" + QString::number(count) + ".jpg" ;
            QImage img;
            img = mergeImages(file1, file2);
            img.save(file3);
            label = "Combined " + file1 + " and " + file2 + " to " + file3;
            ui->progressBar->setValue(ui->progressBar->value()+2);
        } else {
            label = "There was 1 file unprocessed: " + QFileInfo(file_list.first()).fileName();
        }
        ui->label->setText(label);
        ++count;
    }

    if(file_list.size() > 1)
        ui->label->setText("Finished.");
    ui->progressBar->setValue(ui->progressBar->maximum());
}


QImage MainWindow::mergeImages(const QString& file1, const QString& file2)
{
    // Load images
    QImage img1(file1);
    QImage img2(file2);

    // Determine size
    QSize size1 = img1.size();
    QSize size2 = img2.size();
    // Determine if rotation is needed (and rotate)
    QMatrix matrix;
    matrix.scale(1,1);
    matrix.rotate(90);
    if (size1.width() > size1.height())
        img1 = img1.transformed(matrix);
    if (size2.width() > size2.height())
        img2 = img2.transformed(matrix);

    size1 = img1.size();
    size2 = img2.size();
    // Combine side-by-side
    QSize size3(size1.width()+size2.width(), std::max(size1.height(),size2.height()));
    QImage img3(size3,QImage::Format_ARGB32);

    QPainter paint(&img3);
    QRect rect(0,0,size3.width(),size3.height());
    paint.fillRect(rect,QColor("white"));

    paint.drawImage(0,0,img1);
    paint.drawImage(size1.width(),0,img2);

    paint.end();

    // return new image
    return img3;
}
