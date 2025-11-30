#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <cstring>
using namespace std;

struct RegistroJugador {
    char alias[30];
    int puntuacionTotal;
    char fechaUltima[30];
};

class GuardadorDePartidas {
private:
    string nombreArchivo = "puntuaciones.dat";

    // Genera fecha actual formateada
    string fechaActual() {
        time_t now = time(0);
        tm *t = localtime(&now);

        char buffer[30];
        sprintf(buffer, "%02d/%02d/%04d %02d:%02d",
                t->tm_mday,
                t->tm_mon + 1,
                t->tm_year + 1900,
                t->tm_hour,
                t->tm_min);

        return string(buffer);
    }

public:

    /// Guarda o acumula la puntuación de un jugador
    void guardarPuntuacion(string alias, int puntuacion) {
        fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);

        RegistroJugador reg;
        bool encontrado = false;

        // Si el archivo no existe, créalo
        if (!archivo) {
            archivo.open(nombreArchivo, ios::binary | ios::out);
            archivo.close();
            archivo.open(nombreArchivo, ios::binary | ios::in | ios::out);
        }

        // Buscar alias en archivo
        while (archivo.read(reinterpret_cast<char*>(&reg), sizeof(reg))) {
            if (alias == reg.alias) {
                reg.puntuacionTotal += puntuacion;

                string f = fechaActual();
                strcpy(reg.fechaUltima, f.c_str());

                // Mover puntero atrás para sobrescribir
                archivo.seekp(-int(sizeof(reg)), ios::cur);
                archivo.write(reinterpret_cast<char*>(&reg), sizeof(reg));

                encontrado = true;
                break;
            }
        }

        // Si no existe → crear registro nuevo
        if (!encontrado) {
            RegistroJugador nuevo;

            strncpy(nuevo.alias, alias.c_str(), 29);
            nuevo.alias[29] = '\0';

            nuevo.puntuacionTotal = puntuacion;

            string f = fechaActual();
            strcpy(nuevo.fechaUltima, f.c_str());

            archivo.clear();
            archivo.seekp(0, ios::end);
            archivo.write(reinterpret_cast<char*>(&nuevo), sizeof(nuevo));
        }

        archivo.close();
    }

    /// Muestra el reporte ordenado de mayor a menor puntuación
    void mostrarReporte() {
        ifstream archivo(nombreArchivo, ios::binary);
        if (!archivo) {
            cout << "No hay registros.\n";
            return;
        }

        vector<RegistroJugador> lista;
        RegistroJugador reg;

        while (archivo.read(reinterpret_cast<char*>(&reg), sizeof(reg))) {
            lista.push_back(reg);
        }
        archivo.close();

        // Ordenar descendentemente por puntuación
        sort(lista.begin(), lista.end(),
            [](const RegistroJugador &a, const RegistroJugador &b) {
                return a.puntuacionTotal > b.puntuacionTotal;
            });

        cout << "\n====== REPORTE DE JUGADORES ======\n";
        for (auto &r : lista) {
            cout << "Alias: " << r.alias
                << " | Puntos: " << r.puntuacionTotal
                << " | Última vez: " << r.fechaUltima << "\n";
        }
        cout << "=================================\n";
    }
};