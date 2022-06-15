#include <stdio.h>
#include <iostream>
#include <vector>

using namespace std;
typedef int element;

vector<vector<element>> dilatacion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
vector<vector<element>> erosion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
vector<vector<element>> apertura(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
vector<vector<element>> cierre(vector<vector<element>> image,vector<vector<element>> elementoEstructurante);
element maximo(vector<element> elements);
element minimo(vector<element> elements);

int main(int argc, char** argv ){
    vector<vector<int>> data {{48, 2, 13, 190, 220, 41, 15},
                              {12, 51, 96, 170, 210, 35, 51},
                              {96, 84, 92, 150, 250, 10, 53},
                              {41, 57, 3, 180, 90, 60, 20},
                              {55, 98, 6, 12, 20, 250, 65},
                              {17, 9, 44, 10, 22, 180, 52}};

    //creando elemento estructurante
    vector<vector<element>> elementoEstructurante{{ 0, 1, 0},
                                                  { 1, 1, 1},
                                                  { 0, 1, 0}};

    data = erosion(data, elementoEstructurante);
    //data = dilatacion(data, elementoEstructurante);
    //data = apertura(data, elementoEstructurante);
    //data = cierre(data, elementoEstructurante);
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
   element min = 1000000;
   //   Order elements (only half of them)
   for (int i = 0; i < elements.size(); ++i)
   {
      if(elements[i]<min){
          min = elements[i];
      }
   }
   return min;
}

vector<vector<element>> dilatacion(vector<vector<element>> image, vector<vector<element>> elementoEstructurante)
{
    vector<vector<element>> img_result = image;
    int x,y,i,j;
    
    //N width, M height
    element N=image[0].size();
    element M=image.size();
    element Nestructurante=elementoEstructurante[0].size();
    element Mestructurante=elementoEstructurante.size();

    //m y n de estructurante, es decir, del kernel cuanto vale M y N
    int m=Mestructurante/2;
    int n=Nestructurante/2;

    //Move window through all elements of the image
    for(y=m; y<M-m ; ++y)
      for(x=n; x<N-n; ++x)
      {
         vector<element> window;
         int xx=x-n; int yy=y-m;
         for(i=0; i<Mestructurante; i++){
            for(j=0; j<Nestructurante; j++){
                if(elementoEstructurante[i][j]==1){
                    window.push_back(image[yy][xx]);
                }
                xx++;
            }
            xx=x-n;
            yy++;
         }
         img_result[y][x]=maximo(window);
      }
   return img_result;
}

vector<vector<element>> erosion(vector<vector<element>> image,vector<vector<element>> elementoEstructurante)
{
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

    //Move window through all elements of the image
    for (y = m; y < M - m ; ++y)
      for (x = n; x < N - n; ++x)
      {
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
    return img_result;
}

vector<vector<element>> apertura(vector<vector<element>> image,vector<vector<element>> elementoEstructurante)
{
    vector<vector<element>> img_result;
    img_result = erosion(image, elementoEstructurante);
    img_result = dilatacion(img_result, elementoEstructurante);
    return img_result;
}

vector<vector<element>> cierre(vector<vector<element>> image,vector<vector<element>> elementoEstructurante)
{
    vector<vector<element>> img_result;
    img_result = dilatacion(image, elementoEstructurante);
    img_result = erosion(img_result, elementoEstructurante);
    return img_result;
}
