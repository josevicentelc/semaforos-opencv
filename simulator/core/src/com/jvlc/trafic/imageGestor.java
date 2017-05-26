package com.jvlc.trafic;

import com.badlogic.gdx.graphics.Texture;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

/**
 * Created by Josev on 22/11/2016.
 */

public class imageGestor {

    private static HashMap<String, Texture> imagenes;

    private static void loadImages(){
        imagenes = new HashMap<String, Texture>();
        imagenes.put("fondo", new Texture("cruceFondo.png"));
        imagenes.put("cocheAzul", new Texture("azulCar.png"));
        imagenes.put("cocheDerecha", new Texture("rojoCarDerecha.png"));
        imagenes.put("cocheIzquierda", new Texture("rojoCarIzquierda.png"));
        imagenes.put("cocheAbajo", new Texture("rojoCarAbajo.png"));
        imagenes.put("cocheArriba", new Texture("rojoCarArriba.png"));
        imagenes.put("semRojo", new Texture("semRojo.png"));
        imagenes.put("semAmbar", new Texture("semAmbar.png"));
        imagenes.put("semVerde", new Texture("semVerde.png"));
        imagenes.put("prohibido", new Texture("prohibido.png"));
    }


    public static Texture getImage(String n_){
        if (imagenes == null) loadImages();
        return imagenes.get(n_);
    }

}
