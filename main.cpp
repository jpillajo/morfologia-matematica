#include <stdio.h>
#include <iostream>
#include <ctime>
#include <vector>
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "sys/times.h"
#include "sys/vtimes.h"
#include <opencv2/opencv.hpp> //Include OpenCV header file
#include <omp.h>




using namespace cv;
using namespace std;

typedef int element;

vector<vector<element>> cvMat_a_vectorInt(Mat img);
Mat vectorInt_a_cvMat(vector<vector<element>> vect);
vector<vector<element>> dilatacion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
vector<vector<element>> erosion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
element maximo(vector<element> elements);
element minimo(vector<element> elements);
void medirErrorEntreImgs(string src_img_original, string src_img_filtro, int MN_vecindad);


int getRamUsage();
int parseLine(char* line);

double getCurrentValue();
void init();

int main(int argc, char** argv ){

    //definiendo numero de hilos, si no se definen tienen 6 por defecto
    omp_set_num_threads(1);

    //variables para medir tiempo
    double tbegin = omp_get_wtime();


        Mat src_img;
        string path = "/home/alomas/Escritorio/Proyecto/build-Proyecto-Desktop_Qt_6_2_4_GCC_64bit-Debug/MasaMicro1.jpg";

        src_img = imread(path,0); //Default image

        if ( !src_img.data )
        {
            printf("No image data \n");
        }

        //cambiando de Mat a vector<vector<element>>
        vector<vector<element> > vect=cvMat_a_vectorInt(src_img);

        //creando elemento estructurante
        vector<vector<element>> elementoEstructurante{{ 0, 1, 0 },
                                                      { 1, 1, 1},
                                                      { 0, 1, 0}};;

        //cout<<"minimo "<<minimo(elementoEstructurante[0])<<endl;

        vector<vector<element>> vectDilatacion=dilatacion(vect, elementoEstructurante);
        vector<vector<element>> vectErosion=erosion(vect, elementoEstructurante);

        //cambiando de vector<vector<element>> a Mat
        Mat new_img = vectorInt_a_cvMat(vectDilatacion);
        //guardar imagen
        imwrite("/home/alomas/Escritorio/Proyecto/build-Proyecto-Desktop_Qt_6_2_4_GCC_64bit-Debug/Experimento_Masa_Micro_1/Kernel_Cruzado/Dilatacion1.jpg",new_img);

        new_img = vectorInt_a_cvMat(vectErosion);
        //guardar imagen
        imwrite("/home/alomas/Escritorio/Proyecto/build-Proyecto-Desktop_Qt_6_2_4_GCC_64bit-Debug/Experimento_Masa_Micro_1/Kernel_Cruzado/Erosion1.jpg",new_img);

        string img_original = "/home/alomas/Escritorio/Proyecto/build-Proyecto-Desktop_Qt_6_2_4_GCC_64bit-Debug/MasaMicro1.jpg";
        string img_filtro_dilatacion = "/home/alomas/Escritorio/Proyecto/build-Proyecto-Desktop_Qt_6_2_4_GCC_64bit-Debug/Experimento_Masa_Micro_1/Kernel_Cruzado/Dilatacion1.jpg";
        string img_filtro_erosion = "/home/alomas/Escritorio/Proyecto/build-Proyecto-Desktop_Qt_6_2_4_GCC_64bit-Debug/Experimento_Masa_Micro_1/Kernel_Cruzado/Erosion1.jpg";


        int MN_vecindad=1;
        medirErrorEntreImgs(img_original, img_filtro_erosion, MN_vecindad);
        medirErrorEntreImgs(img_original, img_filtro_dilatacion, MN_vecindad);
        medirErrorEntreImgs(img_original, img_original, MN_vecindad);



    //tomar tiempo final y calcularlo
    double time = omp_get_wtime() - tbegin;
    cout << "Execution Time Total: " << time << endl;


    return 0;
}



/*METODOS*/
//metodo para pasar de Mat a vector<vector<int>>
vector<vector<element>> cvMat_a_vectorInt(Mat img){
    vector<vector<element> > vec;
    //Getting a pixel value (1 channel array)
    element x = img.cols-1; //last x position
    element y = img.rows-1; //last y position

    // Inserting elements into vector

        for (element i = 0; i <= y; i++) {
            // Vector to store column elements
            vector<element> v1;

            for (element j = 0; j<=x; j++) {
                element p = img.at<uchar>(Point(j,i)) ;
                v1.push_back(p);
            }

            // Pushing back above 1D vector
            // to create the 2D vector
            vec.push_back(v1);
        }

    return vec;
}

//metodo para pasar de vector<vector<int>> a Mat
Mat vectorInt_a_cvMat(vector<vector<element>> vect){
    //x width, y height
    element x = vect[0].size();
    element y = vect.size();
    //(filas,columnas,canales)
    Mat img = Mat::zeros(y,x,CV_8UC1);
    // Inserting elements into Mat
        //#pragma omp parallel for
        for (element i = 0; i < y; i++) {
            //#pragma omp parallel for
            for (element j = 0; j < x; j++) {
                img.at<uchar>(Point(j,i))=vect[i][j];
            }

        }
     return img;
}


//   MAXIMO y MINIMO calculation
//     elements - input elements
//     N        - number of input elements
element maximo(vector<element> elements)
{
   element max=-1;
   //   Order elements (only half of them)
   for (int i = 0; i < elements.size() ; ++i)
   {
      if(elements[i]>max){
          max=elements[i];
      }
   }
   return max;
}

element minimo(vector<element> elements)
{
   element min=1000000;
   //   Order elements (only half of them)
   for (int i = 0; i < elements.size(); ++i)
   {
      if(elements[i]<min){
          min=elements[i];
      }
   }
   return min;
}


vector<vector<element>> dilatacion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante)
{
    //variables para medir tiempo
    double tbegin = omp_get_wtime();

    vector<vector<element>> img_result = image;
    //N width, M height
    element N = image[0].size();
    element M = image.size();
    element Nestructurante = elementoEstructurante[0].size();
    element Mestructurante = elementoEstructurante.size();

    int x,y,i,j;

    //m y n de estructurante, es decir, del kernel cuanto vale M y N
    int m = Mestructurante/2;
    int n = Nestructurante/2;



   //   Move window through all elements of the image
   #pragma omp parallel for collapse(2) shared(img_result,image,m,n,M,N) private(x,y,i,j) schedule(static)
   for (y = m; y < M - m ; ++y)
      for (x = n; x < N - n; ++x)
      {
        //cout<<y<<" "<<x<<endl;
         vector<element> window;
         int xx = x-n; int yy= y-m;
         for(i=0; i<Mestructurante; i++){
            for(j=0; j<Nestructurante; j++){
                if(elementoEstructurante[i][j] == 1){
                    window.push_back(image[yy][xx]);
                }
                xx++;
            }
            xx = x-n;
            yy++;
         }

         img_result[y][x] = maximo(window);
      }

   //tomar tiempo final y calcularlo
   double time = omp_get_wtime() - tbegin;
   cout << "Execution Time Del Filtro: " << time <<" Max Threads:"<<omp_get_max_threads()<<endl;

   cout << "RAM " << getRamUsage() << " KB" << endl;
   //init();
   cout<< "CPU "<< getCurrentValue() << endl;

   return img_result;
}




vector<vector<element>> erosion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante)
{
    //variables para medir tiempo
    double tbegin = omp_get_wtime();

    vector<vector<element>> img_result = image;
    //N width, M height
    element N = image[0].size();
    element M = image.size();
    element Nestructurante = elementoEstructurante[0].size();
    element Mestructurante = elementoEstructurante.size();
    //tamano de los elementos de la ventana
    int tW = Nestructurante * Mestructurante;


    int x,y,i,j;

    //m y n de estructurante, es decir, del kernel cuanto vale M y N
    int m = Mestructurante/2;
    int n = Nestructurante/2;

   //   Move window through all elements of the image
   #pragma omp parallel for collapse(2) shared(img_result,image,m,n,M,N) private(x,y,i,j) schedule(static)
   for (y = m; y < M - m ; ++y)
      for (x = n; x < N - n; ++x)
      {
        //cout<<y<<" "<<x<<endl;
         vector<element> window;
         int xx = x-n; int yy= y-m;
         for(i=0; i<Mestructurante; i++){
            for(j=0; j<Nestructurante; j++){
                if(elementoEstructurante[i][j] == 1){
                    window.push_back(image[yy][xx]);
                }
                xx++;
            }
            xx = x-n;
            yy++;
         }

         img_result[y][x] = minimo(window);
      }

   //tomar tiempo final y calcularlo
   double time = omp_get_wtime() - tbegin;
   cout << "Execution Time Del Filtro: " << time <<" Max Threads:"<<omp_get_max_threads()<<endl;

   cout << "RAM " << getRamUsage() << " KB" << endl;
   //init();
   cout<< "CPU "<< getCurrentValue() << endl;

   return img_result;
}




void medirErrorEntreImgs(string src_img_original, string src_img_filtro, int MN_vecindad){

    //imgO (imagen Original), imgF (imagen con filtro)
    Mat imgO, imgF;
    string pathO = src_img_original;
    string pathF = src_img_filtro;
    //cout << path << endl;

    imgO = imread(pathO,0); //Default image
    imgF = imread(pathF,0); //Default image


    if ( !imgO.data || !imgO.data)
    {
        printf("No image data \n");
    }

    //cambiando de Mat a vector<vector<element>>
    vector<vector<element> > image=cvMat_a_vectorInt(imgO);
    vector<vector<element> > imageFiltro=cvMat_a_vectorInt(imgF);

    //N width, M height
    element N = image[0].size();
    element M = image.size();
    //tamano de los elementos de la ventana
    int tW = (MN_vecindad*4) + 1;

    double rmse=0; double mae=0;

   //   Mover la ventana a traves de todos los elementos de la imagen
   for (int y = MN_vecindad; y < M - MN_vecindad; ++y)
      for (int x = MN_vecindad; x < N - MN_vecindad; ++x)
      {
         double error = (image[y][x] - imageFiltro[y][x]);
         double errorCuadratico = pow(error,2);
         rmse=rmse+errorCuadratico;

         mae = mae + abs(error);
      }

   double n = (M-MN_vecindad)*(N-MN_vecindad);
   rmse = rmse/n;
   rmse = sqrt(rmse);

   mae = mae/n;

  cout<<"RMSE = "<<rmse<<endl;
  cout<<"MAE = "<<mae<<endl;
}








int parseLine(char* line){
    int i = strlen(line);
    while (*line < '0' || *line > '9') line++; line[i-3] = '\0'; i = atoi(line);
    return i;
}



int getRamUsage(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

static unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

void init(){ FILE* file = fopen("/proc/stat", "r"); fscanf(file, "cpu %Ld %Ld %Ld %Ld", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle); fclose(file); }

double getCurrentValue(){ double percent; FILE* file; unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total; file = fopen("/proc/stat", "r"); fscanf(file, "cpu %Ld %Ld %Ld %Ld", &totalUser, &totalUserLow, &totalSys, &totalIdle); fclose(file); if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys || totalIdle < lastTotalIdle){
        percent = -1.0; } else{ total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys); percent = total; total += (totalIdle - lastTotalIdle); percent /= total; percent *= 100; } lastTotalUser = totalUser; lastTotalUserLow = totalUserLow; lastTotalSys = totalSys; lastTotalIdle = totalIdle; return percent; }




