//*******************************************
//**       Jose Vicente Lozano Copa        ** 
//**             28 / 11 / 2016            ** 
//**                                       ** 
//**    Proyecto (Semaforo inteligente)    ** 
//**         Sistema Industriales          ** 
//**       Universidad de Alicante         ** 
//**                                       ** 
//**                                       ** 
//*******************************************
#ifndef BARRERA_H
#define BARRERA_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <unistd.h>
#include <vector>


using namespace std;
using namespace cv;

class Barrera
{
    public:
        Barrera(Rect &);
        void setPresencia(bool b);
        bool colision(Rect &);
        bool hasPresencia();
        void draw(Mat &);
        virtual ~Barrera();
    protected:
    private:
        Rect posicion;
        bool presencia;
};

#endif // BARRERA_H
