#ifndef ESTADOSEMAFORO_H
#define ESTADOSEMAFORO_H


class estadoSemaforo
{
    public:
        estadoSemaforo();
        virtual ~estadoSemaforo();
        void enable();
        void disable();
        int getDensidad();
        int cochesContados();
        void setDuracion(int d);
        int getDuracion();
        void sumaCoche();
    protected:
    private:
        int cuentaCoches;
        long tiempoEnable;
        int densidad;
        int duracionAsignada;
        long currentTimeMilis();
};


#endif // ESTADOSEMAFORO_H
