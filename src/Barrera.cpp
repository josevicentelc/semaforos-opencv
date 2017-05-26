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

#include "Barrera.h"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <unistd.h>


using namespace std;
using namespace cv;

//Clase que representa a un punto de control del trafico
//Parametro pos: Contendra en Rect con la posicion de la barrera en la imagen
Barrera::Barrera(Rect &pos)
{
    posicion = pos;
    presencia = false;
}

Barrera::~Barrera(){}

//Cambia el estado de presencia e un vehiculo sobre la barrera
void Barrera::setPresencia(bool b){
    presencia = b;
}

//Retorna true si hay un vehiculo sobre la barrera
bool Barrera::hasPresencia(){
    return presencia;
}

//Recibe el Rect de un coche y retorna true si parte de ese rect colisiona con la barrera
bool Barrera::colision(Rect &otro){
    Rect total = otro & posicion;
    return total.area() > 0;
}

//Recibe una imagen y pinta la barrera en ella
// El color cambia segun si hay o no un coche sobre la barrera
void Barrera::draw(Mat &src){
    if (presencia)
       rectangle(src, posicion.tl(), posicion.br(), Scalar(255,0,0), 2, 8, 0);
    else
       rectangle(src, posicion.tl(), posicion.br(), Scalar(255,255,255), 2, 8, 0);
}
