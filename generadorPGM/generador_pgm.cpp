#include <iostream>
#include <fstream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <ctime>

using namespace std;

int numeroAleatorio(int numMax)
{
    // genero nueva semilla a la hora de crear números aleatorios
    srand((unsigned)time(0));
    time_t now = time(0);

    return rand() % numMax + 1;
}

void nuevoMapa(string filename)
{
    fstream file_out;
    int columnas, filas, max, stat, num;

    file_out.open(filename, ios_base::out | ios_base::trunc); // abre el archivo para escribir en él eliminando el contenido anterior
    if (!file_out.is_open())
    {
        cout << "failed to open " << filename << '\n';
    }
    else
    {
        time_t now = time(0);

        file_out << "P2" << endl; // Formato -siempre igual
        file_out << "# Archivo PGM aleatorio, por Pedro Cabello" << endl;
        tm *ltm = localtime(&now);
        file_out << "# Generado el día:" << ltm->tm_mday - 20 << "." << 1 + ltm->tm_mon << "." << 1900 + ltm->tm_year << endl;
        file_out << "# A las " << ltm->tm_hour << ":" << ltm->tm_min << ":" << ltm->tm_sec << " horas" << endl;
        columnas = rand() % 34 + 1; // Mapa A
        // columnas = 34 + rand() % 34; // Mapa B
        // columnas = 68 + rand() % 34; // Mapa C
        filas = rand() % 34 + 1; // Mapa A
        // filas = 34 + rand() % 34; // Mapa B
        // filas = 68 + rand() % 34; // Mapa C
        file_out << columnas << "\t" << filas << endl;
        file_out << 255 << endl;
        for (size_t i = 0; i < filas; i++)
        {
            for (size_t j = 0; j < columnas; j++)
            {
                if (rand() % 5 == 0) // 1/5 = 0,20 = 20% de probabilidad de que genere un obstáculo
                {
                    file_out << rand() % 52 << "\t";
                }
                else
                {
                    file_out << 230 + rand() % 26 << "\t";
                }

                // file_out << num << "\t";
            }
            file_out << endl;
        }

        file_out << endl;

        file_out.close(); // cierro el archivo

        return;
    }
}

int main()
{

    string filename("mapa.pgm");
    // string version_pgm_string = to_string(version_pgm);
    // filename.append(version_pgm_string);
    // filename.append(".pgm");
    srand((unsigned)time(0));
    nuevoMapa(filename);

    return 0;
}
