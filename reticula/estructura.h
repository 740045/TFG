#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Matriz
{
public:
    string formato;
    string comentario;
    unsigned int filas;
    unsigned int columnas;
    unsigned int maxVal; // máximo tono de gris [255 = blanco] [0 = negro]
    int **matrix;        // puntero al primer elemento de la matriz

    bool cargaMatriz(string name); // genera una celda del tamaño de la matriz para posteriormente ser subdividida
    Matriz binariza (int umbral);
};

class Segmento
{
public:
    unsigned int inicio_x;
    unsigned int inicio_y;
    unsigned int fin_x;
    unsigned int fin_y;
};

class Adyacente
{
public:
    int identificador;
    Segmento comun;
};

class Celda
{
public:
    unsigned int identificador;
    bool obstaculo;
    unsigned int x_min, y_min, x_max, y_max;
    vector<Adyacente> ady;

    bool estaOcupada(Matriz mat);
    Segmento segmentoHorizComun(Celda c);
    Segmento segmentoVertiComun(Celda c);
};

class MapaRejilla
{
public:
    unsigned int cols, rows;
    vector<Celda> map;
    unsigned int dimension;

    void particion(Matriz mat, unsigned int dim); // genera un mapa de cuadrículas a aprtir de los datos contenidos en la imagen y una definición para las cuadrículas
    void calculaAdyacentes();                     // calcula las celdas adyacentes a cada una de las celdas de la cuadrícula
    void escribirArchivo(Matriz mat);             // genera (o sobreescribe) un archivo de texto en el que se codifica el grafo a partir de la partición
};
