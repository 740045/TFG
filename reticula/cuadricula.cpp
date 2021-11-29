// Código principal con el empleo de las clases creadas en el archivo de cabecera pgm.h, así como de las funciones implementadas en el archivo pgm.cpp
//
// cuadrícula.cpp
// grafo.txt es genereado a partir de su uso con la lista de celdas generada
//
//  Creado por Pedro Cabello Díaz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "estructura.h"
#include <chrono>

using namespace std;

class Timer
{
public:
    Timer()
    {
        m_StartTimepoint = chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        Stop();
    }

    void Stop()
    {
        auto endTimepoint = chrono::high_resolution_clock::now();

        auto start = chrono::time_point_cast<chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto stop = chrono::time_point_cast<chrono::microseconds>(endTimepoint).time_since_epoch().count();

        auto duration = stop - start;
        double ms = duration * 0.001; // duracion en microsegundos

        cout << duration << "us (" << ms << "ms)\n";
    }

private:
    chrono::time_point<chrono::high_resolution_clock> m_StartTimepoint;
};

int main()
{

    Matriz original, mat;
    MapaRejilla mapa;
    string fileName = {"mapa.pgm"};

    {
        Timer timer;
        if (original.cargaMatriz(fileName)) // la función de cargar la imagen ha funcionado adecuadamente
        {
            mat = original.binariza(0.7 * original.maxVal);

            mapa.particion(mat, 2);
            mapa.calculaAdyacentes();

            // cout<< "\n" <<mapa.map[7].ady[0].identificador<< "\n";

            // for (size_t i = 0; i < mapa.cols * mapa.rows; i++)
            // {

            //     cout << mapa.map[i].identificador << "~" << mapa.map[i].estaOcupada(mat) << "(";
            //     cout << mapa.map[i].x_min << "," << mapa.map[i].y_min << ")(";
            //     cout << mapa.map[i].x_max << "," << mapa.map[i].y_max << ")\t";

            //     if ((i + 1) % mapa.cols == 0)
            //     {
            //         cout << endl;
            //     }
            // }

            // Escritura por archivo del programa
            mapa.escribirArchivo(mat);

            for (Celda &c : mapa.map)
            {
                c.ady.clear();
            }
            mapa.map.clear();
        }
        else
        {
            // el programa ha tenido un problema
            return 1;
        }
    }
}
