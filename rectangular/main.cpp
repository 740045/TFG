// Código principal con el empleo de las clases creadas en el archivo de cabecera pgm.h, así como de las funciones implementadas en el archivo pgm.cpp
//
// main.cpp
// grafo.txt es genereado a partir de su uso
//
//  Creado por Pedro Cabello Díaz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "pgm.h"

using namespace std;

int main()
{
    // VARIABLES
    MatrixImage imagen;              // contendrá los datos que nos da el archivo pgm que cintiene el mapa
    Celda cell;                      // será la celda que emplearemos como herramienta de navegación
    SubDivision subdiv;              // se va a emplear para las recursivas subparticiones
    int precision = 1;               // representa la mínima tolerancia en la partición, como mínimo será de 1 píxel
    int orden = 1;                   // representa el orden de entrada en la lista de celdas, la primera celda será la posición 1
    int completas = 0;               // representa las celdas estudiadas y posteriormente tratadas, parte de 0 ya que no se conoce nada en un principio
    int totales = 1;                 // representa el numero de celdas totales, parte de 1 ya que esta sería la celda que representa el mapa
    bool fin = false;                // indica el final del recorrido a traves de las celdas
    string name = "mapa.pgm";        // nombre de la imagen, ha de ser almacenada en la misma carpeta que los archivos
    vector<Celda> celdasCompletas;   // lista que contendrá a las celdas definitivas. Es un vector ya que permite long indeterminada
    vector<Celda> celdasIncompletas; // lista que contendrá a las celdas incompletas. Es un vector ya que permite long indeterminada

    // vector<Celda> *lista = &listaCeldas; //puntero al vector que contiene la lista de celdas

    // imagen.cargapgm(name)  carga la imagen en el objeto MatrixImage y devuelve true si se ha cargado adecuadamente

    if (imagen.cargapgm(name)) // si se ha cargado adecuadamente hace funcionar el resto del programa
    {
        // imagen.mostrar();                     //verifico que la imagen ha sido leida adecuadamente
        imagen.binariza(0.7 * imagen.maxVal); // establezco como umbral un 70% del valor máximo
        imagen.mostrar();                     // verifico que la imagen ha sido binarizada adecuadamente

        cell.imgtocell(imagen); // genero una celda a partir de la imagen, que posteriormente iré subdividiendo en caso de estar ocupada

        // borramos el contenido de las listas de celdas
        celdasCompletas.clear();
        celdasIncompletas.clear();

        celdasIncompletas.push_back(cell); // introducimos la celda en la lista de celdas incompletas
        // celdasIncompletas.shrink_to_fit(); //reduce la capacidad al tamaño

        // do
        // {
        //     cout << "Introduce la precisión deseada como un número entero positivo: ";
        //     cin >> precision;
        // } while (precision < 1);

        while (completas != totales)
        {
            cell = celdasIncompletas.back(); // toma como puntero de celda la deirecciónn del último miembro de la lista de celdas incompletas
            cell.checkocupada(imagen);       // hace que el puntero a celda auxiliar apunte al último elemento del vector de celdas incompletas
            cell.minimadimension();          // calcula la minima dimensión (depués será compararada con la precisión) y la almacena en minDim

            if (!cell.ocupado || (cell.minDim == precision) || cell.todaOcupada(imagen)) // si la celda está libre de obstáculos o si es más pequeña que la precisión
            {

                // la celda reune las condiciones para pasar a la lista definitiva
                cell.identificador = completas + 1; // se introduce el orden con que ha entrado en la lista
                celdasCompletas.push_back(cell);    // añadimos a la lista de celdas el elemento apuntado por el puntero cel
                completas++;                        // el numero de celdas completas aumenta en una
                celdasIncompletas.pop_back();       // elimino la última celda de la lesta de celdas incompletas al haber actuado sobre ella
            }
            else
            {
                // la celda es mixta y ha de ser subdividida
                subdiv.split(cell);
                celdasIncompletas.pop_back(); // elimino la última celda de la lesta de celdas incompletas al haber actuado sobre ella
                totales = totales + 3;        // la celda que había y tres divisiones extra
                celdasIncompletas.push_back(subdiv.celdaDR);
                celdasIncompletas.push_back(subdiv.celdaDL);
                celdasIncompletas.push_back(subdiv.celdaTR);
                celdasIncompletas.push_back(subdiv.celdaTL);
            }
        }

        // muestra por pantalla el numero de celdas completas
        cout << "definitivas; \n"
             << celdasCompletas.size() << " celdas" << endl;
        // for (Celda c : celdasCompletas)
        // {
        //     cout << c.identificador << "\t";
        // }
        // cout << endl;
        // for (Celda c : celdasCompletas)
        // {
        //     c.adyacentes(celdasCompletas);
        //     cout << c.identificador << "\t" << c.ocupado << "\t" << c.minDim << "\t" << c.minF << "," << c.minC << "\t" << c.maxF << "," << c.maxC << "\t";
        //     cout << c.ady.size() << " celdas adyacentes: ";
        //     for (auto k : c.ady)
        //     {
        //         cout << k.ident << ": " << k.seg.inicio.x << "," << k.seg.inicio.y << " " << k.seg.final.x << "," << k.seg.final.y << "; \t";
        //     }
        //     cout << endl;
        // }
        // cout << endl;

        // Escritura por archivo del programa
        escribirArchivo(celdasCompletas);

        // borramos el contenido de las listas de celdas y borramos los punteros
        celdasCompletas.clear();
        celdasIncompletas.clear();

        return 0;
    }
    else
    {
        return 1;
    }
}
