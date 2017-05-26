
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

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <sys/time.h>
#include <iostream>
#include <unistd.h>
#include "controlador.h"

using namespace std;
using namespace cv;


Mat getMascara( Mat& , float , float , float , float , float , float , float, float, float);

string cascadeName;
string nestedCascadeName;
int thresh = 255;
int max_thresh = 255;
RNG rng(12345);

//Pinta una imagen dentro de otra en la posicion indicada por la fila y la columna
void putMatInMat(Mat& origin, Mat& dest, int row_, int col_){
    Vec3b color;
    for (int r = 0; r < origin.rows; r++)
        for(int c = 0; c < origin.cols; c++){
            color = origin.at<Vec3b>(r,c);
            dest.at<Vec3b>(r+row_, c+col_) = color;
        }
}

//Recibe una imagen, un vector de Areas(Rect) y un color
//Pinta los rect en la imagen del color indicado
void pintaAreas(Mat& src, vector<Rect>& areas, const Scalar& color){
   for (int i = 0; i< areas.size(); i++){
       rectangle(src, areas[i].tl(), areas[i].br(), color, 2, 8, 0);
   }
}

//Busca objetos sobre la mascara que tengan un area mayor que Area
//Despues llama a los metodos para pintar las areas encontradas en la imagen
//Retorna un vector de los Rects encontrados
vector<Rect> findObject(Mat& src, Mat& mascara, int area){
    Mat gray;
    Mat thres_output;
    cvtColor(mascara, mascara, CV_RGB2GRAY);
    vector<vector<Point> > contornos;
    vector<Vec4i> hierarchy;

    findContours(mascara, contornos, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

    vector<vector<Point> > contornos_poly(contornos.size());
    vector<Rect> boundRect(contornos.size());

    for (int i=0; i< contornos.size(); i++){
        approxPolyDP(Mat(contornos[i]), contornos_poly[i], 3, true);
        boundRect[i] = boundingRect(Mat(contornos_poly[i]));
    }


    vector<Rect> salida;
    for (int i = boundRect.size()-1; i>=0; i--){
        if (boundRect[i].width * boundRect[i].height >= area){
            salida.push_back(boundRect[i]);
        }
    }

    pintaAreas(src, salida, Scalar(0, 255, 0));

    return salida;
}

//Pinta soobre una imagen un texto en una posicion y de un cierto color
void drawText(Mat &vista, String &texto, const Point &p, const Scalar& color){
   RNG rng(0xFFFFFFFF);//Fuente
   putText( vista,
            texto.c_str(),
            p,
            rng.uniform(0,1),               //Tipo de letra
            rng.uniform(0,20)*0.05+0.1,    //Tamaño de la letra
            color,          //Color
            rng.uniform(1, 5),
            1);
}

//Pinta sobre la imagen todos los parametros del programa, estados, barreras... etc
void escribeValores(Mat &vista, controlador *control){
    stringstream s1;
    stringstream s2;
    stringstream s3;
    stringstream s4;
    String texto;

    s2 << control->getTimeToChange();
    texto = "Cambio en : " + s2.str() + " ms";
    drawText(vista, texto, Point(400, 50), Scalar(255, 255, 255));

    s3 << control->cochesContados();
    texto = "Contados : " + s3.str();
    drawText(vista, texto, Point(400, 110), Scalar(255, 255, 255));

    s4 << "Estados: " ;
    texto = s4.str();
    drawText(vista, texto, Point(400, 200), Scalar(255, 255, 255));
    for (int i = 0; i<control->getNumEstados(); i++){
        stringstream sa;
        Scalar color;

        if (i == control->getEstado())
            color = Scalar(0, 255, 0);
        else
            color = Scalar(255, 255, 255);

        sa << " "<< i <<": T = "<<control->getDurationToState(i) ;
        texto = sa.str()+ "ms";
        drawText(vista, texto, Point(400, 230 + 65*i), color);
        stringstream sb;
        sb << "    D = "<<control->getDensidadToState(i) ;
        texto = sb.str() + " C/min";
        drawText(vista, texto, Point(400, 230 + 65*i + 30), color);

    }
}

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image, inicial;
    Mat frame1, rojos, azules;
    Mat vista;
    vector<Rect> coches;
    vector<Rect> barreras;
    controlador* control;
    string inputName;

   int c = inputName.empty() ? 0 : inputName[0] - '0';
    if(!capture.open(c))
        cout << "La camara #" <<  c << " no funciona" << endl;

    if( capture.isOpened() )
    {
		capture.set(CV_CAP_PROP_FRAME_WIDTH,320);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT,240);

   	cout << "Capturando la camara " <<c<< endl;

        for(;;)
        {
            capture >> frame;

            if( !frame.empty() ){
                if (inicial.empty()){
                    inicial = frame.clone();
                    azules = getMascara( inicial, 0, 255, 0, 255, 0, 255, -255, -255, 50 );
                    barreras = findObject(inicial, azules, 100);    //Busco Objeto sobre el primer fotograma
                    control = new controlador(2, barreras, 30000, 8000);
                }

                frame1 = frame.clone();
                rojos = getMascara( frame1, 140, 255, 0, 110, 0, 130, 50, -255, -255 );
                vista = Mat::zeros(400, 800, frame1.type());   //Creo una pantalla de gran tamaño
                coches = findObject(frame1, rojos, 200);     //Enmarco los objetos rojos con area de 200px o mas

                putMatInMat(frame1, vista, 0, 0);  //Pongo en la pantalla la imagen tal como la ve la camara
                control->verify(coches, vista);
                escribeValores(vista, control);
                imshow("Vista", vista); //Muestro la pantalla
                int c = waitKey(10);
                if( c == 27 || c == 'q' || c == 'Q' )
                    break;
            }
        }
    }

    return 0;
}


//Genera una imagen binaria con aquellos pixels que cumplen los requisitos RGB
Mat getMascara( Mat& img, float minR, float maxR, float minG, float maxG, float minB, float maxB, float difR, float difG, float difB)
{
    Mat salida = Mat::zeros(img.size(), img.type());
    Vec3b color;
    for (int x = 0; x < img.rows; x++)
        for(int y = 0; y < img.cols; y++){
            color = img.at<Vec3b>(x,y);
            float B = color.val[0];
            float G = color.val[1];
            float R = color.val[2];

            if (B >= minB && B <= maxB)
            if (R >= minR && R <= maxR)
            if (G >= minG && G <= maxG)
            if (R-G >= difG && R-B >= difR)
            if (G-R >= difG && G-B >= difG)
            if (B-R >= difB && B-G >= difB)
            {
                salida.at<Vec3b>(x,y).val[0] = 255;
                salida.at<Vec3b>(x,y).val[1] = 255;
                salida.at<Vec3b>(x,y).val[2] = 255;
            }
        }

        return salida;
}














