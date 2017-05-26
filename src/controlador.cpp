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

//Clase que controla los posibles estados de un cruce con semaforos
//Se ocuopa de cambiar el estado del semaforo y obtener las medidas de densidad del trafico
//Requiere conocer los puntos de control de trafico para contar coches

#include "controlador.h"

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include <vector>
#include "estadoSemaforo.h"
#include "GPIOClass.h"

using namespace std;
using namespace cv;

//Constructor
// estados_ Numero de posibles estados en los que puede estar el semaforo, un cruce de 2 calles deberia tener 2 estados
// zonas_ Rects en los cuales se debe de contar que un coche accede a la interseccion
// tiempo_ Tiempo total del ciclo para todos los estados, en este tiempo todos los semaforos debe haber pasado por todos sus estados
//     Por ejemplo_
//          Un cruce de dos semaforos, donde cada uno esta abierto durante 30 segundos, tendra un tiempo de ciclo total de 60.000 ms
//minimo_ En caso de que un carril no tenga trafico, le asignara este tiempo como minimo a su semaforo para que no tenga un 0
controlador::controlador(const int &estados_, vector<Rect>&zonas_,const int &tiempo_, const int &minimo)
{
    for (int i = 0; i< zonas_.size(); i++){
        Barrera b(zonas_[i]);
        zonas.push_back(b);
    }

    puerto = new GPIOClass("4");
    puerto->export_gpio();
    puerto->setdir_gpio("out");

    estadoActual = 0;
    tiempoMinimo = minimo;

    if (estados_ < 2)   numEstados = 2;
    else                numEstados = estados_;

    if (tiempo_ < 1000) tiempo = 5000;
    else                tiempo = tiempo_;

    for (int i=0;i<numEstados;i++){
        estadoSemaforo nuevoestado;
        nuevoestado.setDuracion(tiempo/numEstados);
        if (i == 0){
            nuevoestado.enable();
            ultimoCambio = currentTimeMilis();
        }
        estados.push_back(nuevoestado);
    }

    cout << "Controlador del trafico:"<<endl;
    cout << "  Tiempo total   : "<< tiempo<<endl;
    cout << "  Nro Estados    : "<<numEstados<< endl;
    cout << "  Puntos de Ctrl :" << zonas.size()<<endl;

}

controlador::~controlador(){}

//Recibe el vector con las posiciones de los coches
//en este momento este metodo se ocupa de contar el trafico que pasa por los puntos de control 
//A continuacion llama a checkstate para hacer un control del estado de los semaforos
void controlador::verify(vector<Rect>& coches, Mat& src){
    for (int j=0; j<zonas.size(); j++){
        bool hayCoche = false;
        for (int i=0; i<coches.size();i++){
            if (zonas[j].colision(coches[i])){
                hayCoche = true;
            }
        }
        if (hayCoche && !zonas[j].hasPresencia()) estados[estadoActual].sumaCoche();
        zonas[j].setPresencia(hayCoche);
        zonas[j].draw(src);
    }

    checkState();
}

//Metodo que controla el cambio de estado de los semaforos
//Cuando acaba el ultimo estado, recalcula los tiempos que debe durar cada uno en funcion del volumen de trafico
void controlador::checkState(){
    if (currentTimeMilis() - ultimoCambio >= estados[estadoActual].getDuracion()){
        estados[estadoActual].disable();

        estadoActual++;
        if (estadoActual == estados.size()){
            estadoActual =0;
            int sumaDensidades = 0;
            for (int i = 0;i<estados.size();i++)
                sumaDensidades += estados[i].getDensidad();

            //Si no cuanto ningun coche en todo el ciclo, no hago ningun cambio en los tiempos
            if (sumaDensidades != 0){
                for (int i = 0; i< estados.size(); i++){
                    estados[i].setDuracion(tiempo * (estados[i].getDensidad()* 1.0 / sumaDensidades));
                    if (estados[i].getDuracion() < tiempoMinimo)
                        estados[i].setDuracion(tiempoMinimo);
                }
            }
        }
        ultimoCambio = currentTimeMilis();
        estados[estadoActual].enable();
        
        if(estadoActual == 0)
		puerto->setval_gpio("0");

	else
		puerto->setval_gpio("1");
    }
}

//Retora el numero de coches que se han contado de momento en el actual estado del semaforo
int controlador::cochesContados(){
    return estados[estadoActual].cochesContados();
}

//Retorna el numero de estado actual del semaforo
int controlador::getEstado(){
    return estadoActual;
}

//Retorna los milisegundos que faltan hasta el proximo cambio de estado
long controlador::getTimeToChange(){
    return (ultimoCambio + estados[estadoActual].getDuracion() - currentTimeMilis());
}

//Retorna el numero de estados que tiene asignado el controlador
int controlador::getNumEstados(){
    return numEstados;
}

//Retorna la ultima densidad calculada para un cierto estado del controlador
int controlador::getDensidadToState(int &d){
    if (d<0) return 0;
    if (d >= estados.size()) return 0;
    return estados[d].getDensidad();
}

//Retorna el tiempo asignado a un cierto estado del controlador
int controlador::getDurationToState(int &d){
    if (d<0) return 0;
    if (d >= estados.size()) return 0;
    return estados[d].getDuracion();
}

//Retorna la fecha actual del sistema en milisegundos
long controlador::currentTimeMilis(){
    struct timeval t;
    long timeMilis;
    gettimeofday(&t, NULL);

    timeMilis = (t.tv_sec * 1000 + t.tv_usec/1000.0)+0.5;
    return timeMilis;
}
