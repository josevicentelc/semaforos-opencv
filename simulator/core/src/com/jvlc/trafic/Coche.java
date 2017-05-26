package com.jvlc.trafic;

import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.Batch;
import com.badlogic.gdx.graphics.g2d.Sprite;

/**
 * Created by Josev on 22/11/2016.
 */
public class Coche {

    private Texture imagen;

    private float x;

    private float y;

    private float escala = 1;

    public Coche(int x_, int y_, Texture imagen_){
        imagen = imagen_;
        setX(x_);
        setY(y_);
    }

    public void setScale(float s){
        escala = s;
    }

    public void setX(float x_){
        x = x_;
    }

    public void setY(float y_){
        y = y_;
    }

    public float getX(){
        return x;
    }

    public float getY(){
        return y;
    }

    public void draw(Batch batch){
        batch.draw(imagen, x, y, imagen.getWidth() * escala, imagen.getHeight() * escala);
    }

}
