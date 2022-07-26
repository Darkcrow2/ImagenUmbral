#include <opencv2/opencv.hpp>
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main()
{
    Mat imagen;
    imagen = imread("greenscreen.jpg", 1);

    int filas = imagen.rows;
    int columnas = imagen.cols;
    int sumaTotal = 0;
    int sumaPixeles = 0;
    int Umbral = 0;
    int multiplica = filas * columnas;

    Mat gris(filas, columnas, CV_8UC1);
    Mat grisUmbral(filas, columnas, CV_8UC1);
    Mat grisUmbralBordes(filas + 2, columnas + 2, CV_8UC1);
    Mat grisUmbralFiltro(filas, columnas, CV_8UC1);

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            cv::Vec3b pixel = imagen.at<Vec3b>(i, j);

            uchar Azul = pixel[0];
            uchar Verde = pixel[1];
            uchar Rojo = pixel[2];

            gris.at<uchar>(i, j) = (Azul + Verde + Rojo)/3;

            sumaPixeles += gris.at<uchar>(i, j);
        }
    }

    Umbral = sumaPixeles / multiplica;

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            if (gris.at<uchar>(i, j) >= Umbral)
            {
                grisUmbral.at<uchar>(i, j) = 0;
            }
            else
            {
                grisUmbral.at<uchar>(i, j) = 255;
            }

        }
    }

    for (int k = 0; k < filas + 2; k++)
    {
        for (int l = 0; l < columnas + 2; l++)
        {
            if ((k == 0) || (l == 0))
            {
                grisUmbralBordes.at<uchar>(k, l) = 0;
            }
            else if ((k == filas + 1) || (l == columnas + 1))
            {
                grisUmbralBordes.at<uchar>(k, l) = 0;
            }
            else
            {
                grisUmbralBordes.at<uchar>(k, l) = grisUmbral.at<uchar>(k - 1, l - 1);
            }
        }
    }

    for (int m = 0; m < filas; m++)
    {
        for (int n = 0; n < columnas; n++)
        {
            for (int p = 0; p < 3; p++)
            {
                for (int q = 0; q < 3; q++)
                {
                    sumaTotal = sumaTotal + grisUmbralBordes.at<uchar>(p + m, q + n);
                }
            }

            grisUmbralFiltro.at<uchar>(m, n) = sumaTotal / 9;
            sumaTotal = 0;
        }
    }

    imshow("Imagen gris", gris);
    imshow("Imagen gris umbralizada", grisUmbral);
    imshow("Imagen gris con filtro de media", grisUmbralFiltro);
    printf("%d", Umbral);
    
    waitKey();
    return 0;
}