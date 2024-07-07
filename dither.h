#ifndef DITHER_H
#define DITHER_H

#include <QWidget>
#include <QFile>


namespace Ui {
class Dither;
}

class Dither : public QWidget
{
    Q_OBJECT

public:
    explicit Dither(QWidget *parent = nullptr);
    ~Dither();
    void autoDither(double R_12bit,double G_12bit,double B_12bit);
    void getDitherMatrix(double matrix[32][32]);
    void ditherRun();
    void displayLoop();
    void loadDitherMatrix(QString fileName_dither_matrix);
    void getDitherMatrix();
    void close();
    void hide();
    double valueCal(double value,double ratio);
    void fullscreen();
    void fullscreenEnd();

private:

    int *R_index;
    int *G_index;
    int *B_index;
    int *R_out;
    int *G_out;
    int *B_out;

    bool Isplay=false;

    double matrixValue[32][32];

public slots:
    void setDitherPixmap(int R,int G,int B);


};

#endif // DITHER_H
