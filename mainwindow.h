#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dither.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void paraInit();
    ~MainWindow();

private slots:
    void on_pushButton_play_clicked();


    void on_spinBox_R_valueChanged(int arg1);

    void on_spinBox_G_valueChanged(int arg1);

    void on_spinBox_B_valueChanged(int arg1);

    void on_horizontalSlider_R_valueChanged(int value);

    void on_horizontalSlider_G_valueChanged(int value);

    void on_horizontalSlider_B_valueChanged(int value);

    void on_spinBox_ratioR_valueChanged(int arg1);

    void on_spinBox_ratioG_valueChanged(int arg1);

    void on_spinBox_ratioB_valueChanged(int arg1);

    void on_horizontalSlider_ratioR_valueChanged(int value);

    void on_horizontalSlider_ratioG_valueChanged(int value);

    void on_horizontalSlider_ratioB_valueChanged(int value);

    void on_lineEdit_screenWidth_textChanged(const QString &arg1);

    void on_pushButton_fullscreen_clicked();

    void on_pushButton_fullscreen_2_clicked();

private:
    Ui::MainWindow *ui;
    Dither *dither;

};
#endif // MAINWINDOW_H
