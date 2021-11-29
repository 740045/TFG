// Archivo con de cabecera con la designación de cada clase a emplear
//
//  pgm.cpp
//
//  Creado por Pedro Cabello Díaz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Coordenadas
{
public:
    int x; // representa la columna
    int y; // representa la fila
};

class Segmento
{
public:
    Coordenadas inicio; // representa las coordenadas del inicio del segmento de intersección
    Coordenadas final;  // representa las coordenadas del final del segmento de intersección
};

class CelAdy
{
public:
    int ident;
    Segmento seg;
};

class MatrixImage
{
public:
    int rows;   // numero de filas
    int cols;   // numero de columnas
    int maxVal; // maximo color en la escala de grises

    string format;  // permite guardar formato del archivo
    string comment; // permite guardar comentarios de los datos
    int *arr;       // puntero al primer elemento del vector que almacene los valores grises de la imagen

    bool cargapgm(string fileName); // lee y carga la imagen pgm en una matriz
    void binariza(int umbral);      // binariza los colores de la imagen para facilitar su procesado
    void mostrar();                 // muestra por pantalla los valores de la matriz de imagen
};

// tiene que contener la siguiente información:
// ocupado;minY;maxY;minX;maxX;
class Celda
{
public:
    int identificador;  // identifica cada celda numericamente, sera llenado finalmente
    int minF;           // mínimo píxel (fila)
    int maxF;           // máximo píxel (fila)
    int minC;           // mínimo píxel (columna)
    int maxC;           // máximo píxel (columna)
    int minDim;         // dimensión mínima de la celda
    bool ocupado;       // es true si la celda contiene un pixel negro (0)
    vector<CelAdy> ady; // continene las referencias de las celdas adycentes de la lista de adyacentes

    void imgtocell(MatrixImage img);          // genera una celda del tamaño de la matriz para posteriormente ser subdividida
    void mostrar(MatrixImage mat);            // muestra por pantalla los valores de la celda
    void checkocupada(MatrixImage mat);       // verifica si la celda está ocupada y da valor al parametro 'ocupado', que en principio será false
    bool todaOcupada(MatrixImage mat);        // devolverá true si toda la celda son obstaculos
    void minimadimension();                   // guarda en minDim la minima dimensión que será comparada con la precisión
    Segmento segmentoHorizComun(Celda c);     // devuelve el segmento horizontal en común que tiene con otra celda
    Segmento segmentoVertiComun(Celda c);     // devuelve el segmento vertical en común que tiene con otra celda
    void adyacentes(vector<Celda> listaCell); // escribe por archivo el número de puntos que coinciden con cada celda adyacente
};

class SubDivision
{
public:
    // T significa ARRIBA
    // D significa ABAJO
    // L significa IZQUIERDA
    // R significa DERECHA
    Celda celdaTL;
    Celda celdaTR;
    Celda celdaDL;
    Celda celdaDR;

    void split(Celda cell); // divide la celda en cuatro
};

void escribirArchivo(vector<Celda> listaCeldas); // pasa la lista de celdas a un archivo con el formato preestablecido