package com.jvlc.trafic;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Batch;

/**
 * Created by Josev on 22/11/2016.
 */
public class Semaforo {

    private enum Estado {Rojo, Ambar, Verde}

    private int lockX = 0;
    private int lockY = 0;

    private int drawx = 0;
    private int drawy = 0;

    Estado estado = Estado.Rojo;

    private int rangoSemaforo = 30;

    private long deboCambiarARojoEnTiempo = 1500;

    private int duracionAmbar = 0;

    public Semaforo(int xlock_, int ylock_, int xdraw_, int ydraw_){
        lockX = xlock_;
        lockY = ylock_;
        drawx = xdraw_;
        drawy = ydraw_;
    }

    public void abrir(){
        estado = Estado.Verde;
    }

    public void cerrar(){
        estado = Estado.Rojo;
    }

    public void ambar(){
        estado = Estado.Ambar;
    }

    public boolean esAccesible(float x, float y){
        boolean accesible = true;
        if (estado == Estado.Ambar || estado == Estado.Rojo)
            if (x >= lockX - rangoSemaforo && x <= lockX+rangoSemaforo)
                if (y >= lockY -rangoSemaforo && y <= lockY+rangoSemaforo)
                    accesible = false;
        return accesible;
    }

    public void draw(Batch batch){

        Texture img;
        if (estado == Estado.Rojo) img = imageGestor.getImage("semRojo");
        else
            if (estado == Estado.Ambar) img = imageGestor.getImage("semAmbar");
                else  img = imageGestor.getImage("semVerde");

        batch.draw(img ,drawx, drawy, img.getWidth()*0.5f, img.getHeight()*0.5f);
    }
}
