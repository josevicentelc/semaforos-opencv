rm control-semaforos
g++ -o control-semaforos main.cpp controlador.cpp GPIOClass.cpp estadoSemaforo.cpp Barrera.cpp `pkg-config opencv --libs --cflags` 
./control-semaforos