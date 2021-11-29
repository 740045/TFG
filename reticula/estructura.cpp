#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "estructura.h"

using namespace std;

// genera una celda del tamaño de la matriz para posteriormente ser subdividida
bool Matriz::cargaMatriz(string name)
{
    // declaro las variables
    ifstream inFile;
    char c;     // para ver cuantas lineas de comentarios tengo
    string aux; // para añadir lineas de comentario extra

    // abro el archivo y verifico que se ha abierto adecuadamente
    inFile.open(name);
    if (!inFile.is_open())
    {
        cout << "El archivo no ha podido ser abierto" << endl;
        return false;
    }

    // FORMATO

    // tomo los valores del archivo pgm y los escribo en la matriz
    getline(inFile, formato);
    if (formato != "P2")
    {
        cout << "El formato no es válido" << std::endl;
        return false;
    }

    // COMENTARIOS, COLUMNAS, FILAS Y VALOR MAXIMO EN LA ESCALA DE GRISES[0-255]

    // leo linea de comentarios y columnas
    inFile >> c;
    if (c == '#')
    {
        getline(inFile, comentario); // comentario de la imagen
        inFile >> c;                 // vuelvo a leer el siguiente caracter
    }
    while (c == '#')
    {
        getline(inFile, aux); // comentario de la imagen
        comentario.append("\n");
        comentario.append(aux);
        inFile >> c; // vuelvo a leer el siguiente caracter
    }
    inFile.putback(c); // retira la lectura de un caracter

    inFile >> columnas; // obtiene el número de columnas
    inFile >> filas;    // obtiene el numero de filas

    // inicializo la matriz[filas][columnas]
    matrix = new int *[columnas];
    for (int i = 0; i < filas; ++i)
        matrix[i] = new int[filas];

    inFile >> maxVal; // maximo valor en la escala de grises

    // hago un barrido por el documento para obtener los colores de los píxeles
    for (size_t i = 0; i < filas; i++)
    {
        for (size_t j = 0; j < columnas; j++)
        {
            inFile >> matrix[i][j]; // matrix[fila][columna]
        }
    }

    return true;
}

Matriz Matriz::binariza(int umbral)
{
    Matriz binarizada;
    for (size_t i = 0; i < filas; i++)
    {
        for (size_t j = 0; j < columnas; j++)
        {
            if (matrix[i][j] < umbral)
            {
                binarizada.matrix[i][j] = 0;
            }
            else
            {
                binarizada.matrix[i][j] = maxVal;
            }
        }
    }
    return binarizada;
}

// estudia si la celda está ocupada o no
bool Celda::estaOcupada(Matriz mat)
{

    for (size_t i = y_min; i <= y_max; i++)
    {
        for (size_t j = x_min; j <= x_max; j++)
        {
            if (mat.matrix[i][j] == 0)
                return true;
        }
    }
    return false;
}

Segmento Celda::segmentoHorizComun(Celda c)
{
    Segmento comun;

    if ((x_min <= c.x_min) && (x_max >= c.x_max)) // la celda actual es mayor o igual que la celda tomada por parametro
    {
        if (y_max > c.y_min) // la celda actual tiene como segmento comun con su celda adyacente el lado inferior de la celda tomada como parametro
        {
            comun.inicio_x = c.x_min;
            comun.inicio_y = y_min;
            comun.fin_x = c.x_max;
            comun.fin_y = y_min;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente el lado superior de la celda tomada como parametro
        {
            comun.inicio_x = c.x_min;
            comun.inicio_y = y_max;
            comun.fin_x = c.x_max;
            comun.fin_y = y_max;
        }
    }
    else // la celda actual es menor que la celda tomada por parametro
    {
        if (y_max < c.y_min) // la celda actual tiene como segmento comun con su celda adyacente su lado inferior
        {
            comun.inicio_x = x_min;
            comun.inicio_y = y_max;
            comun.fin_x = x_max;
            comun.fin_y = y_max;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente su lado superior
        {
            comun.inicio_x = x_min;
            comun.inicio_y = y_min;
            comun.fin_x = x_max;
            comun.fin_y = y_min;
        }
    }
    return comun;
}

// devuelve el segmento vertical en común en que tiene con otra celda
Segmento Celda::segmentoVertiComun(Celda c)
{
    Segmento comun;

    if ((y_min <= c.y_min) && (y_max >= c.y_max)) // la celda actual es mayor o igual que la celda tomada por parametro
    {
        if (x_min < c.x_min) // la celda actual tiene como segmento comun con su celda adyacente el lado derecho de la celda tomada como parametro
        {
            comun.inicio_x = x_max;
            comun.inicio_y = c.y_min;
            comun.fin_x = x_max;
            comun.fin_y = c.y_max;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente el lado izquierdo de la celda tomada como parametro
        {
            comun.inicio_x = x_min;
            comun.inicio_y = c.y_min;
            comun.fin_x = x_min;
            comun.fin_y = c.y_max;
        }
    }
    else // la celda actual es menor que la celda tomada por parametro
    {
        if (x_max < c.x_min) // la celda actual tiene como segmento comun con su celda adyacente su lado derecho
        {
            comun.inicio_x = x_max;
            comun.inicio_y = y_min;
            comun.fin_x = x_max;
            comun.fin_y = y_max;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente su lado izquierdo
        {
            comun.inicio_x = x_min;
            comun.inicio_y = y_min;
            comun.fin_x = x_min;
            comun.fin_y = y_max;
        }
    }

    return comun;
}

// genera un mapa de cuadrículas a aprtir de los datos contenidos en la imagen y una definición para las cuadrículas
void MapaRejilla::particion(Matriz mat, unsigned int dim)
{
    Celda n;
    unsigned int minimaX = 0;
    unsigned int minimaY = 0;
    unsigned int orden = 1;

    dimension = dim;

    if ((mat.columnas % dim) == 0)
        cols = mat.columnas / dim;
    else
        cols = mat.columnas / dim + 1;

    if ((mat.filas % dim) == 0)
        rows = mat.filas / dim;
    else
        rows = mat.filas / dim + 1;

    for (size_t i = 0; i < rows; i++)
    {

        for (size_t j = 0; j < cols; j++)
        {
            n.identificador = orden;
            n.x_min = minimaX;
            n.y_min = minimaY;

            minimaX = minimaX + dim;
            n.x_max = minimaX - 1; // porque cuenta la columna y fila 0
            if (n.x_max > (mat.columnas - 1))
            {
                // en caso de que supere las dimensiones, la última columna de celdas de la rejilla será de menor anchura
                n.x_max = mat.columnas - 1;
            }

            if ((int(minimaY) - 1) < 0)
            {
                n.y_max = dimension - 1;
            }
            else
            {
                n.y_max = minimaY + dim - 1;
            }

            if (n.y_max > (mat.filas - 1))
            {
                // en caso de que supere las dimensiones, la última fila de celds de la rejilla será de menor altura
                n.y_max = mat.filas - 1;
            }

            map.push_back(n);

            orden++;

            // map[orden - 1] = n; //map[fila][columna]
        }
        minimaX = 0;
        minimaY = minimaY + dimension;
    }

    return;
}

// calcula las celdas adyacentes a cada una de las celdas de la cuadrícula
void MapaRejilla::calculaAdyacentes()
{
    Adyacente adjacent;

    for (Celda &c : map)
    {
        for (Celda &celdaAdyacente : map)
        {
            if (c.identificador != celdaAdyacente.identificador)
            {
                if ((celdaAdyacente.y_max == (c.y_min - 1)) || (celdaAdyacente.y_min == (c.y_max + 1))) // celdas por debajo y por encima, respectivamente
                {
                    if (((celdaAdyacente.x_max <= c.x_max) && (celdaAdyacente.x_min >= c.x_min)) || ((c.x_max <= celdaAdyacente.x_max) && (c.x_min >= celdaAdyacente.x_min)))
                    {
                        adjacent.identificador = celdaAdyacente.identificador;
                        adjacent.comun = c.segmentoHorizComun(celdaAdyacente);
                        c.ady.push_back(adjacent);
                    }
                }
                if ((celdaAdyacente.x_max == (c.x_min - 1)) || (celdaAdyacente.x_min == (c.x_max + 1))) // celdas a la izquierda y derecha de la celda en cuestion
                {
                    if (((celdaAdyacente.y_max <= c.y_max) && (celdaAdyacente.y_min >= c.y_min)) || ((c.y_max <= celdaAdyacente.y_max) && (c.y_min >= celdaAdyacente.y_min)))
                    {
                        adjacent.identificador = celdaAdyacente.identificador;
                        adjacent.comun = c.segmentoVertiComun(celdaAdyacente);
                        c.ady.push_back(adjacent);
                    }
                }
            }
        }
    }
}

// genera (o sobreescribe) un archivo de texto en el que se codifica el grafo a partir de la partición
void MapaRejilla::escribirArchivo(Matriz mat)
{

    // Escritura por archivo del programa
    string filename(" grafo.txt");
    fstream file_out;
    int obstac = 0;

    file_out.open(filename, ios_base::out | ios_base::trunc); // abre el archivo para escribir en él eliminando el contenido anterior
    if (!file_out.is_open())
    {
        cout << "failed to open " << filename << '\n';
    }
    else
    {
        file_out << map.size() << endl; // muestra el numero de celdas que hay
        for (auto cel : map)
        {
            if (cel.estaOcupada(mat))
            {
                obstac++;
            }

            file_out << cel.identificador << ";";
            file_out << cel.estaOcupada(mat) << ";";
            file_out << 4 << ";"; // numero de vertices, en caso de la partición en rejilla siempre sera 4
            file_out << 4 << ";"; // numero de coordenadas X de los vertices
            file_out << cel.x_min << "," << cel.x_max << "," << cel.x_min << "," << cel.x_max << ";";
            file_out << 4 << ";"; // numero de coordenadas Y de los vertices
            file_out << cel.y_min << "," << cel.y_min << "," << cel.y_max << "," << cel.y_max << ";";
            // calculaAdyacentes(cel);
            file_out << cel.ady.size() << ";\t";
            for (auto l : cel.ady)
            {
                // identificador_celda_adyacente(número_puntos_interseccion):(x_1,y_1)||(x_2,y_2);
                file_out << l.identificador << "(2):(" << l.comun.inicio_x << "," << l.comun.inicio_y << ")(" << l.comun.fin_x << "," << l.comun.fin_y << ");\t";
            }

            file_out << endl;
        }
        cout << obstac << " obstaculos \n";

        file_out.close(); // cierro el archivo
    }
}
