// Archivo con implelmentación de las funciones de cada clase
//
//  pgm.cpp
//
//  Creado por Pedro Cabello Díaz

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "pgm.h"

using namespace std;

// lee y carga la imagen pgm en una matriz
bool MatrixImage::cargapgm(string fileName)
{
    // declaro las variables
    ifstream inFile;
    char c;     // para ver cuantas lineas de comentarios tengo
    string aux; // para añadir lineas de comentario extra

    // abro el archivo y verifico que se ha abierto adecuadamente
    inFile.open(fileName);
    if (!inFile.is_open())
    {
        cout << "El archivo no ha podido ser abierto" << endl;
        return false;
    }

    // FORMATO

    // tomo los valores del archivo pgm y los escribo en la matriz
    getline(inFile, format);
    if (format != "P2")
    {
        cout << "El formato no es válido" << std::endl;
        return false;
    }

    // COMENTARIOS, COLUMNAS, FILAS Y VALOR MAXIMO EN LA ESCALA DE GRISES[0-255]

    // leo linea de comentarios y columnas
    inFile >> c;
    if (c == '#')
    {
        getline(inFile, comment); // comentario de la imagen
        inFile >> c;              // vuelvo a leer el siguiente caracter
    }
    while (c == '#')
    {
        getline(inFile, aux); // comentario de la imagen
        comment.append("\n");
        comment.append(aux);
        inFile >> c; // vuelvo a leer el siguiente caracter
    }
    inFile.putback(c); // retira la lectura de un caracter

    inFile >> cols; // obtiene el número de columnas
    inFile >> rows; // obtiene el numero de filas

    // establezco el tamaño del array que contendrá los valores
    arr = new int[rows * cols];

    inFile >> maxVal; // maximo valor en la escala de grises

    // VALORES DE LOS PÍXELS

    // hago un barrido por el documento para obtener los colores de los píxeles
    for (size_t i = 0; i < rows; i++)
        for (size_t j = 0; j < cols; j++)
            inFile >> arr[j + i * cols];

    return true;
    inFile.close();
}

// binariza los colores de la imagen para facilitar su procesado
void MatrixImage::binariza(int umbral)
{

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            if (arr[j + i * cols] < umbral)
                arr[j + i * cols] = 0;

            else
                arr[j + i * cols] = 255;
        };
    };
    return;
}

// funcion que muestra por pantalla los valores de la matriz de imagen
void MatrixImage::mostrar()
{
    int index;

    cout << format << std::endl
         << comment << std::endl
         << cols << ' ' << rows << std::endl
         << maxVal << std::endl;

    index = 0;
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            cout << arr[index] << '\t';
            index++;
        }
        cout << '\n';
    };
    return;
}

// genera una celda del tamaño de la matriz para posteriormente ser subdividida y devuelvw el puntero a esta celda
void Celda::imgtocell(MatrixImage img)
{
    minC = 0;
    maxC = img.cols - 1; // le resto uno ya que el primer elemento es 0
    minF = 0;
    maxF = img.rows - 1; // le resto uno ya que el primer elemento es 0

    return;
}

// muestra por pantalla los valores de la celda
void Celda::mostrar(MatrixImage mat)
{
    int index = 0;
    for (size_t i = minF; i < maxF; i++)
    {
        for (size_t j = minC; j < maxC; j++)
            cout << mat.arr[j + i * mat.cols] << '\t';
        cout << '\n';
    }
    return;
}

// verifica si la celda está pcupada y da valor al parametro 'ocupado'
void Celda::checkocupada(MatrixImage mat)
{
    ocupado = false; // en principio suponemos que la celda no contiene obstáculos, si los tuviera pasa a ser true

    for (size_t i = minF; i <= maxF; i++)
    {
        for (size_t j = minC; j <= maxC; j++)
        {
            if (mat.arr[j + i * mat.cols] == 0)
            {
                ocupado = true;
                return; // con que ya exista un obstáculo en la celda ya es suficiente
            }
        };
    };
    return;
}

// devolverá true si toda la celda son obstaculos
bool Celda::todaOcupada(MatrixImage mat)
{
    bool enteraOcupada = true;
    for (size_t i = minF; i <= maxF; i++)
    {
        for (size_t j = minC; j <= maxC; j++)
        {
            if (mat.arr[j + i * mat.cols] != 0) // 0 es el color negro, que tendrán los obstáculos
            {
                enteraOcupada = false;
                return enteraOcupada; // con que no exista un obstáculo en la celda ya es suficiente
            }
        };
    };
    return enteraOcupada;
}

// calcula la dimensión mínima de la celda
void Celda::minimadimension()
{

    if ((maxF - minF) <= (maxC - minC))
        minDim = maxF - minF;
    else
        minDim = maxC - minC;
    minDim++;
    return;
}

// devuelve el segmento horizontal en común en que tiene con otra celda
Segmento Celda::segmentoHorizComun(Celda c)
{
    Segmento comun;

    if ((minC <= c.minC) && (maxC >= c.maxC)) // la celda actual es mayor o igual que la celda tomada por parametro
    {
        if (maxF < c.minF) // la celda actual tiene como segmento comun con su celda adyacente el lado superior de la celda tomada como parametro
        {
            comun.inicio.x = c.minC;
            comun.inicio.y = maxF;
            comun.final.x = c.maxC;
            comun.final.y = maxF;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente el lado inferior de la celda tomada como parametro
        {
            comun.inicio.x = c.minC;
            comun.inicio.y = minF;
            comun.final.x = c.maxC;
            comun.final.y = minF;
        }
    }
    else // la celda actual es menor que la celda tomada por parametro
    {
        if (maxF < c.minF) // la celda actual tiene como segmento comun con su celda adyacente su lado inferior
        {
            comun.inicio.x = minC;
            comun.inicio.y = maxF;
            comun.final.x = maxC;
            comun.final.y = maxF;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente su lado superior
        {
            comun.inicio.x = minC;
            comun.inicio.y = minF;
            comun.final.x = maxC;
            comun.final.y = minF;
        }
    }
    return comun;
}

// devuelve el segmento vertical en común en que tiene con otra celda
Segmento Celda::segmentoVertiComun(Celda c)
{
    Segmento comun;
    // bool ve = false;

    if ((minF <= c.minF) && (maxF >= c.maxF)) // la celda actual es mayor o igual que la celda tomada por parametro
    {
        if (maxC < c.minC) // la celda actual tiene como segmento comun con su celda adyacente el lado derecho de la celda tomada como parametro
        {
            comun.inicio.x = maxC;
            comun.inicio.y = c.minF;
            comun.final.x = maxC;
            comun.final.y = c.maxF;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente el lado izquierdo de la celda tomada como parametro
        {
            comun.inicio.x = minC;
            comun.inicio.y = c.minF;
            comun.final.x = minC;
            comun.final.y = c.maxF;
        }
    }
    else // la celda actual es menor que la celda tomada por parametro
    {
        if (maxC < c.minC) // la celda actual tiene como segmento comun con su celda adyacente su lado derecho
        {
            comun.inicio.x = maxC;
            comun.inicio.y = minF;
            comun.final.x = maxC;
            comun.final.y = maxF;
        }
        else // la celda actual tiene como segmento comun con su celda adyacente su lado izquierdo
        {
            comun.inicio.x = minC;
            comun.inicio.y = minF;
            comun.final.x = minC;
            comun.final.y = maxF;
        }
    }

    return comun;
}

// añade en ady{} las celdas adyacentes de todas las que aparecen en la lista total
void Celda::adyacentes(vector<Celda> listaCell)
{
    CelAdy celdaAdyacente;

    for (const auto i : listaCell)
    {
        if (i.identificador != identificador) // no evaluamos la propia celda como adyacente de si misma
        {
            if ((i.maxF == (minF - 1)) || (i.minF == (maxF + 1))) // celdas por debajo y por encima, respectivamente
            {
                if (((i.maxC <= maxC) && (i.minC >= minC)) || ((maxC <= i.maxC) && (minC >= i.minC)))
                {
                    celdaAdyacente.ident = i.identificador;
                    celdaAdyacente.seg = segmentoHorizComun(i);
                    ady.push_back(celdaAdyacente);
                }
            }
            if ((i.maxC == (minC - 1)) || (i.minC == (maxC + 1))) // celdas a la izquierda y derecha
            {
                if (((i.maxF <= maxF) && (i.minF >= minF)) || ((maxF <= i.maxF) && (minF >= i.minF)))
                {
                    celdaAdyacente.ident = i.identificador;
                    celdaAdyacente.seg = segmentoVertiComun(i);
                    ady.push_back(celdaAdyacente);
                }
            }
        }
    }
}

// función que divide una celda en cuatro
void SubDivision::split(Celda cell)
{
    // el recorrido a realizar en las subparticiones es:
    //  de izquierda a derecha
    //  de arriba a abajo
    // Representacion esquemática del sistema de partición
    //        |1TL  |2TR     |
    //        |3DL  |4LR     |
    //        |     |        |
    //    Como se puede observar, las celdas de las filas y columnas finales serán más grandes para asi poder aceptar mapas de cualquier tamaño

    int width = (cell.maxC - cell.minC + 1);
    int height = (cell.maxF - cell.minF + 1);

    // TL
    celdaTL.minF = cell.minF;
    celdaTL.minC = cell.minC;
    celdaTL.maxF = cell.minF + (height / 2) - 1;
    celdaTL.maxC = cell.minC + (width / 2) - 1;

    // TR
    celdaTR.minF = cell.minF;
    celdaTR.minC = cell.minC + (width / 2);
    celdaTR.maxF = cell.minF + (height / 2) - 1;
    celdaTR.maxC = cell.maxC;

    // DL
    celdaDL.minF = cell.minF + (height / 2);
    celdaDL.minC = cell.minC;
    celdaDL.maxF = cell.maxF;
    celdaDL.maxC = cell.minC + (width / 2) - 1;

    // DR
    celdaDR.minF = cell.minF + (height / 2);
    celdaDR.minC = cell.minC + (width / 2);
    celdaDR.maxF = cell.maxF;
    celdaDR.maxC = cell.maxC;

    return;
}

void escribirArchivo(vector<Celda> listaCeldas) // pasa la lista de celdas a un archivo con el formato preestablecido
{
    int ob = 0;
    // Escritura por archivo del programa
    string filename("grafo.txt");
    fstream file_out;

    file_out.open(filename, ios_base::out | ios_base::trunc); // abre el archivo para escribir en él eliminando el contenido anterior
    if (!file_out.is_open())
    {
        cout << "failed to open " << filename << '\n';
    }
    else
    {
        file_out << listaCeldas.size() << endl; // muestra el numero de celdas que hay
        for (auto cel : listaCeldas)
        {
            file_out << cel.identificador << ";";
            if (cel.ocupado)
            {
                file_out << 1 << ";";
                ob++;
            }

            else
                file_out << 0 << ";";
            file_out << 4 << ";"; // numero de vertices, en caso de la partición rectangular siempre sera 4
            file_out << 4 << ";"; // numero de coordenadas X de los vertices
            file_out << cel.minC << "," << cel.maxC << "," << cel.minC << "," << cel.maxC << ";";
            file_out << 4 << ";"; // numero de coordenadas Y de los vertices
            file_out << cel.minF << "," << cel.minF << "," << cel.maxF << "," << cel.maxF << ";";
            cel.adyacentes(listaCeldas);
            file_out << cel.ady.size() << ";";
            for (auto l : cel.ady)
            {
                // identificador_celda_adyacente(número_puntos_interseccion):(x_1,y_1)||(x_2,y_2);
                file_out << l.ident << "(2):(" << l.seg.inicio.x << "," << l.seg.inicio.y << ")(" << l.seg.final.x << "," << l.seg.final.y << ");";
            }

            file_out << endl;
        }
        cout << "\n obstáculos: " << ob;
        file_out.close(); // cierro el archivo
    }
}
