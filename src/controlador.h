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

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "estadoSemaforo.h"
#include "Barrera.h"
#include <vector>
#include <sys/time.h>
#include <iostream>
#include "GPIOClass.h"
#include <unistd.h>

using namespace std;
using namespace cv;

class controlador
{
    public:
        controlador(const int &, vector<Rect>&, const int &, const int &);
        virtual ~controlador();
        void verify(vector<Rect>&, Mat&);
        int cochesContados();
        int getEstado();
        long getTimeToChange();
        int getDurationToState(int &d);
        int getDensidadToState(int &d);
        int getNumEstados();
    protected:
    private:
        long currentTimeMilis();
        void checkState();
        long ultimoCambio;
        int numEstados;
        int estadoActual;
        vector<Barrera> zonas;
        int tiempo;
        vector<estadoSemaforo> estados;
        int tiempoMinimo;
	GPIOClass* puerto;
};

#endif // CONTROLADOR_H
