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

//Clase que representa a uno de los posibles estados en los que se puede encontrar un cruce de semaforos
//La intencion de esta clase es calcular la densidad del trafico mientras un estado esta activo

#include "estadoSemaforo.h"
#include <sys/time.h>
#include <iostream>
#include <unistd.h>

using namespace std;

estadoSemaforo::estadoSemaforo(){
    densidad = 0;
    duracionAsignada = 0;
    cuentaCoches = 0;
}

estadoSemaforo::~estadoSemaforo(){}

//Pone este estado como activo
void estadoSemaforo::enable(){
    tiempoEnable = currentTimeMilis();
    cuentaCoches = 0;
}

//Establece el tiempo que debe durar este estado
void estadoSemaforo::setDuracion(int d){
    duracionAsignada = d;
}

//Retorna el tiempo que debe durar este estado
int estadoSemaforo::getDuracion(){
    return duracionAsignada;
}

//En el estado actual se suma un coche a la densidad de trafico
void estadoSemaforo::sumaCoche(){
    cuentaCoches++;
}

//Retorna el numero de coches contados en el ultimo periodo de activacion
int estadoSemaforo::cochesContados(){
    return cuentaCoches;
}

//El estado pasa a desactivado
//Se calculara la densidad en funcion del tiempo que duro el estado
void estadoSemaforo::disable(){
    int nuevaDensidad;
    //Obtengo la cantidad de milisegundos que ha estado abierto el semaforo
    double tiempoActivo = currentTimeMilis() - tiempoEnable;
    nuevaDensidad = (int)(cuentaCoches* 1.0 / (tiempoActivo / 1000) * 60);
    densidad = (densidad + nuevaDensidad)*1.0 / 2;
}

//Retorna la densidad del trafico en coches/minuto
int estadoSemaforo::getDensidad(){
    return densidad;
}

//Obtiene la fecha del sistema en milisegundos
long estadoSemaforo::currentTimeMilis(){
    struct timeval t;
    long timeMilis;
    gettimeofday(&t, NULL);
    timeMilis = (t.tv_sec * 1000 + t.tv_usec/1000.0)+0.5;
    return timeMilis;
}
