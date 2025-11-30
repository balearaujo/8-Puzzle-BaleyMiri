#ifndef PUNTAJES_HPP
#define PUNTAJES_HPP
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <ctime>
using namespace std;

struct Jugador {
    char alias[30];
    int puntos;
    char fecha[20];
};

string obtenerFecha() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    char buffer[20];
    strftime(buffer, 20, "%Y-%m-%d", now);
    return string(buffer);
}

void guardarPuntuacion(string alias, int puntos, string fecha) {
    vector<Jugador> lista;
    Jugador j;

    ifstream fileIn("jugadores.dat", ios::binary);
    if (fileIn) {
        while (fileIn.read((char*)&j, sizeof(Jugador)))
            lista.push_back(j);
        fileIn.close();
    }

    bool existe = false;
    for (auto &jug : lista) {
        if (strcmp(jug.alias, alias.c_str()) == 0) {
            jug.puntos += puntos;
            strcpy(jug.fecha, fecha.c_str());
            existe = true;
            break;
        }
    }

    if (!existe) {
        Jugador nuevo;
        strcpy(nuevo.alias, alias.c_str());
        nuevo.puntos = puntos;
        strcpy(nuevo.fecha, fecha.c_str());
        lista.push_back(nuevo);
    }

    ofstream fileOut("jugadores.dat", ios::binary | ios::trunc);
    for (auto &jug : lista)
        fileOut.write((char*)&jug, sizeof(Jugador));

    fileOut.close();
}

void mostrarReporte() {
    vector<Jugador> lista;
    Jugador j;

    ifstream fileIn("jugadores.dat", ios::binary);
    if (!fileIn) {
        cout << "No hay registros aun.\n";
        return;
    }

    while (fileIn.read((char*)&j, sizeof(Jugador))) {
        lista.push_back(j);
    }
    fileIn.close();

    sort(lista.begin(), lista.end(), [](const Jugador &a, const Jugador &b){
        return a.puntos > b.puntos;
    });

    cout << "\n===== REPORTE DE JUGADORES =====\n";
    for (auto &jug : lista) {
        cout << "Alias: " << jug.alias << "\n";
        cout << "Puntos: " << jug.puntos << "\n";
        cout << "Ultima fecha: " << jug.fecha << "\n";
        cout << "--------------------------------\n";
    }
}

#endif
