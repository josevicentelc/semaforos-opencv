package com.jvlc.trafic;

import com.badlogic.gdx.ApplicationAdapter;
import com.badlogic.gdx.ApplicationListener;
import com.badlogic.gdx.Gdx;
import com.badlogic.gdx.graphics.GL20;
import com.badlogic.gdx.graphics.Texture;
import com.badlogic.gdx.graphics.g2d.SpriteBatch;

import static com.badlogic.gdx.Gdx.input;

public class trafic extends ApplicationAdapter {
	SpriteBatch batch;

	Carril carrilA1;
	Carril carrilB1;
	Carril carrilA2;
	Carril carrilB2;

	/** Estado de los semaforos [0..3]*/
	private int estado = 0;

	/** Tiempo del sistema en que los ssemaforos cambiaron de estado por ultima vez*/
	private long ultimoCambio = 0;

	/** Tiempo predefinido que dura el estado de Ambar al cambia de estado*/
	private int tiempoAmbar = 2500;

	/** Variable auxiliar para saber que estoy cambiando de estado entre semaforos*/
	private boolean cambioDeEstado = false;

	/** Clase que se ocupa de capturar los eventos de teclado*/
	private KeyListener listener;

	/**
	 * CInversion del estado de los semaforos
	 */
	public void change(){
		cambioDeEstado = true;
	}

	@Override
	public void create () {

		listener = new KeyListener(this);
		input.setInputProcessor(listener);

		batch = new SpriteBatch();
		int ancho = Gdx.graphics.getWidth();
		int alto = Gdx.graphics.getHeight();

		carrilA1 = new Carril(-150, alto/2 -35, ancho, alto/2-35, ancho/2-180, alto/2-13, 5, ancho/2-130, alto/2-130 );
		carrilA1.setScalaCoches(0.5f);
		carrilA2 = new Carril(ancho+150, alto/2 +35, -150, alto/2+35, ancho/2+150, alto/2+62, 5, ancho/2+150, alto/2+100 );
		carrilA2.setScalaCoches(0.5f);
		carrilB1 = new Carril(ancho/2+50, -150, ancho/2+50, alto, ancho/2+70, alto/2-163, 18, ancho/2+130, alto/2-163 );
		carrilB1.setScalaCoches(0.5f);
		carrilB2 = new Carril(ancho/2-40, alto+150, ancho/2-40, -150, ancho/2-15, alto/2+150, 19, ancho/2-90, alto/2+130 );
		carrilB2.setScalaCoches(0.5f);
	}

	@Override
	public void render () {
		Gdx.gl.glClearColor(1, 0, 0, 1);
		Gdx.gl.glClear(GL20.GL_COLOR_BUFFER_BIT);
		cambiaEstados();
		batch.begin();
			batch.draw(imageGestor.getImage("fondo"), 0, 0);
			carrilA1.draw(batch);
			carrilA2.draw(batch);
			carrilB1.draw(batch);
			carrilB2.draw(batch);
		batch.end();

	}

	/**
	 * Cambia al siguiente estado de los semaforos
	 * invocando al metodo correspondiente de cara carril
	 */
	private void cambiaEstados(){
			if (estado == 0 && cambioDeEstado){
				estado = 1;
				carrilA1.ambarSemaforo();
				carrilA2.ambarSemaforo();
				carrilB1.cerrarSemaforo();
				carrilB2.cerrarSemaforo();
				ultimoCambio = System.currentTimeMillis();
				cambioDeEstado = false;
			}
			else
			if (estado == 1 && System.currentTimeMillis() > ultimoCambio + tiempoAmbar){
				estado = 2;
				carrilA1.cerrarSemaforo();
				carrilA2.cerrarSemaforo();
				carrilB1.abrirSemaforo();
				carrilB2.abrirSemaforo();
				ultimoCambio = System.currentTimeMillis();
			}
			else
			if (estado == 2 && cambioDeEstado){
				estado = 3;
				carrilA1.cerrarSemaforo();
				carrilA2.cerrarSemaforo();
				carrilB1.ambarSemaforo();
				carrilB2.ambarSemaforo();
				ultimoCambio = System.currentTimeMillis();
				cambioDeEstado = false;
			}
			else
			if (estado == 3 && System.currentTimeMillis() > ultimoCambio + tiempoAmbar){
				estado = 0;
				carrilA1.abrirSemaforo();
				carrilA2.abrirSemaforo();
				carrilB1.cerrarSemaforo();
				carrilB2.cerrarSemaforo();
				ultimoCambio = System.currentTimeMillis();
			}
	}


	@Override
	public void dispose () {
		batch.dispose();
	}
}
