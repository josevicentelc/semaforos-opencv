package com.jvlc.trafic;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Batch;

import java.awt.Canvas;
import java.util.ArrayList;
import java.util.List;

import sun.awt.Mutex;

/**
 * Created by Josev on 22/11/2016.
 */

public class Carril {


    private Semaforo semaforo;

    private List<Coche> coches;

    private int cochesPorMinuto;

    private int xInicio = 0;

    private int yInicio = 0;

    private int xFin = 0;

    private int yFin = 0;

    private long siguientecoche  =0;

    private long periodo = 0;

    private float scalaCoches = 1;

    private float velocidad = 0;



    public Carril(int xIni_, int yIni_, int xFin_, int yFin_, int xSemaforo, int ySemaforo, int cochesPorMinuto_, int xPosSemaforo, int yPosSemaforo){
        coches = new ArrayList<Coche>();
        xInicio = xIni_;
        yInicio = yIni_;
        xFin = xFin_;
        yFin = yFin_;
        cochesPorMinuto = cochesPorMinuto_;
        semaforo = new Semaforo(xSemaforo, ySemaforo, xPosSemaforo, yPosSemaforo);
        periodo = 60/cochesPorMinuto*1000;
        velocidad = cochesPorMinuto_/60 *2;
        if (velocidad==0) velocidad=2;
    }

    public void setScalaCoches(float f){
        scalaCoches = f;
    }


    private void destruirCochesFueraDeRango(){
        int margen = 200;
        for (int i = coches.size()-1; i>=0; i--){
            if (xInicio<xFin){  //Van a la derecha, me cargo aquellos donde la X es mayor que xFin
                if (coches.get(i).getX() > xFin + margen) coches.remove(i);
            }
            else if (xInicio>xFin){ //Van a la izquierda, me cargo los que tienen una x menor que xFin
                if (coches.get(i).getX() < xFin - margen) coches.remove(i);
            }
            else if (yInicio < yFin){ //Van arriba, me cargo los que tienen una Y mayor que yfin
                if (coches.get(i).getY() > yFin + margen) coches.remove(i);
            }
            else{   //Van abajo, me cargo los que tienen una Y menor que yfin
                if (coches.get(i).getY() < yFin - margen) coches.remove(i);
            }
        }
    }


    public void draw(Batch batch){

        destruirCochesFueraDeRango();

        //Si se ha cumplido el periodo de tiempo entre coches, creo uno nuevo
        if (System.currentTimeMillis() > siguientecoche){
            if (!puntoOcupado(-1, xInicio, yInicio)){
                addCoche();
                siguientecoche = System.currentTimeMillis() + periodo;
            }
        }

        for (int i = 0 ; i<coches.size(); i++){
            if (xInicio<xFin)       moveCoche(i, velocidad, 0);
            else if (xInicio>xFin)  moveCoche(i, velocidad*-1, 0);

            if (yInicio<yFin)       moveCoche(i, 0, velocidad);
            else if (yInicio>yFin)  moveCoche(i, 0, velocidad*-1);

            coches.get(i).draw(batch);
            //batch.draw(imageGestor.getImage("prohibido"), coches.get(i).getX(), coches.get(i).getY(), 5, 5);
        }
        semaforo.draw(batch);
    }

    /**
     * Abre el semaforo de este carril
     */
    public void cerrarSemaforo(){
        semaforo.cerrar();
    }

    public void ambarSemaforo(){
        semaforo.ambar();
    }

    /**
     * Cierra el semaforo de este carril
     */
    public void abrirSemaforo(){
        semaforo.abrir();
    }

    /**
     * Añade un coche, siempre y cuando no haya otro coche en el punto de spam
     */
    private void addCoche(){
        Texture img;

        if (xInicio<xFin) img = imageGestor.getImage("cocheDerecha");
        else if (xInicio>xFin) img = imageGestor.getImage("cocheIzquierda");
        else if (yInicio<yFin) img = imageGestor.getImage("cocheArriba");
        else  img = imageGestor.getImage("cocheAbajo");

        Coche nuevo = new Coche(xInicio, yInicio, img);
        nuevo.setScale(scalaCoches);
        coches.add(nuevo);
    }

    /**
     * Muevo el coche en un incremento siempre y cuando no se mueva a una posicion ocupada por
     * otro coche o por un semaforo cerrado
     * @param idCoche   Id del coche a mover
     * @param incX      Incremento del eje X
     * @param incY      Incremento del eje Y
     */
    private void moveCoche(int idCoche, float incX, float incY){
        //Antes de mover el moche debo verificar que no colisionara con otro coche o con el semaforo
        if (esAccesible(idCoche, coches.get(idCoche).getX()+incX, coches.get(idCoche).getY()+incY)){
            coches.get(idCoche).setX(coches.get(idCoche).getX()+incX);
            coches.get(idCoche).setY(coches.get(idCoche).getY()+incY);
        }
    }

    /**
     * Compruebo que las coordenadas indicadas estan libres de coches o semaforos
     * Es necesario conocer el id del coche que desea acceder para no tener en cuenta una
     * colision consigo mismo
     * @param idCoche   Id del coche que desea moverse a dicha coordenada
     * @param x Coordenada X
     * @param y Coordenada Y
     * @return  True si se puede mover
     */
    private boolean esAccesible(int idCoche, float x, float y){
        boolean accesible = true;
        accesible = semaforo.esAccesible(x, y);
        //compruebo que no colisiono con otro coche
        if (accesible) accesible = !puntoOcupado(idCoche, x, y);
        return accesible;
    }


    /**
     * Compruebo que las coordenadas indicadas estan libres de coches
     * Es necesario conocer el id del coche que desea acceder para no tener en cuenta una
     * colision consigo mismo
     * @param idCoche   Id del coche que desea moverse a dicha coordenada
     * @param x Coordenada X
     * @param y Coordenada Y
     * @return  True la posicion esta ocupada por otro coche
     */    private boolean puntoOcupado(int idCoche, float x, float y){
        int tamMargen = 100;
        boolean ocupado = false;
        for (int i=0;i<coches.size() && !ocupado;i++){
            if (coches.get(i).getX()>= x-tamMargen && coches.get(i).getX()<= x+tamMargen)
                if (coches.get(i).getY()>= y-tamMargen && coches.get(i).getY()<= y+tamMargen)
                    if (i != idCoche)
                        ocupado = true;
        }
        return ocupado;
    }

}
