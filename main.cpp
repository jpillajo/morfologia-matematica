#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;
typedef int element;

vector<vector<element>> dilatacion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
vector<vector<element>> erosion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
element maximo(vector<element> elements);
element minimo(vector<element> elements);

int main(int argc, char** argv ){
        vector<vector<int>> data
                {
                    {1, 2, 3, 100, 210, 40, 50},
                    {4, 5, 6, 110, 230, 30, 55},
                    {7, 8, 9, 140, 210, 20, 52},
                    {4, 27, 36, 160, 20, 60, 50},
                    {5, 58, 65, 10, 240, 70, 75},
                    {7, 89, 94, 40, 220, 80, 82}
                };
        //creando elemento estructurante
        vector<vector<element>> elementoEstructurante{{ 0, 1, 0 },
                                                      { 1, 1, 1},
                                                      { 0, 1, 0}};;

        data = erosion(data, elementoEstructurante);
        cout << "Filas:"<<data.size()<<"; Columnas:"<<data[0].size() << endl;
    return 0;
}

/*METODOS*/
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
    vector<vector<element>> img_result = image;
    element N = image[0].size();
    element M = image.size();
    element Nestructurante = elementoEstructurante[0].size();
    element Mestructurante = elementoEstructurante.size();

    int x,y,i,j;

    int m = Mestructurante/2;
    int n = Nestructurante/2;

   //   Move window through all elements of the image
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
   return img_result;
}




vector<vector<element>> erosion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante)
{
    vector<vector<element>> img_result = image;
    element N = image[0].size();
    element M = image.size();
    element Nestructurante = elementoEstructurante[0].size();
    element Mestructurante = elementoEstructurante.size();
    //tamano de los elementos de la ventana
    int tW = Nestructurante * Mestructurante;

    int x,y,i,j;

    int m = Mestructurante/2;
    int n = Nestructurante/2;

   //   Move window through all elements of the image
   for (y = m; y < M - m ; ++y)
      for (x = n; x < N - n; ++x)
      {
        //cout<<y<<" "<<x<<endl;
         vector<element> window;
         int xx = x-n; int yy= y-m;
         for(i=0; i<Mestructurante; i++){
            for(j=0; j<Nestructurante; j++){
                if(elementoEstructurante[i][j] == 1){
                    cppwindow.push_back(image[yy][xx]);
                }
                xx++;
            }
            xx = x-n;
            yy++;
         }

         img_result[y][x] = minimo(window);
      }
   return img_result;
}
