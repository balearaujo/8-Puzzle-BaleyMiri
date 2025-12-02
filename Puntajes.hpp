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

string obtenerFecha() { // Obtiene la fecha actual 
    time_t t = time(nullptr); // Obtiene el tiempo actual
    tm* now = localtime(&t); // Convierte a estructura tm
    char buffer[20]; // Buffer para la fecha
    strftime(buffer, 20, "%Y-%m-%d", now); // Formatea la fecha
    return string(buffer);
}

void guardarPuntuacion(string alias, int puntos, string fecha) { // Guarda los puntos de un jugador
    // Lee el archivo 
    vector<Jugador> lista;
    Jugador j;

    ifstream fileIn("jugadores.dat", ios::binary); // Abre el archivo en modo binario

    if (fileIn) { // Si el archivo existe
        while (fileIn.read((char*)&j, sizeof(Jugador))) // Lee cada jugador
            lista.push_back(j);
        fileIn.close();
    }

    bool existe = false; //checa si el jugador ya existe

    for (auto &jug : lista) { // Busca el jugador
        if (strcmp(jug.alias, alias.c_str()) == 0) { // Si existe
            jug.puntos += puntos; // Suma los puntos
            strcpy(jug.fecha, fecha.c_str());
            existe = true; //cambia su estado
            break;
        }
    }

    if (!existe) { // Si no existe, lo agrega
        Jugador nuevo; // Crea un nuevo jugador
        strcpy(nuevo.alias, alias.c_str());
        nuevo.puntos = puntos; // Le guarda los puntos
        strcpy(nuevo.fecha, fecha.c_str()); // Le guarda la fecha
        lista.push_back(nuevo); // Lo agrega a la lista
    }

    ofstream fileOut("jugadores.dat", ios::binary | ios::trunc); // Abre el archivo para escribir
    for (auto &jug : lista)
        fileOut.write((char*)&jug, sizeof(Jugador)); // Escribe cada jugador

    fileOut.close(); // Cierra el archivo
}

void mostrarReporte() { // Muestra el reporte de jugadores
    vector<Jugador> lista;
    Jugador j;

    ifstream fileIn("jugadores.dat", ios::binary); // Abre el archivo 
    if (!fileIn) { //Revisa el archivo
        cout << "No hay registros aun.\n";
        return;
    }

    while (fileIn.read((char*)&j, sizeof(Jugador))) {
        lista.push_back(j);
    }
    fileIn.close();

    sort(lista.begin(), lista.end(), [](const Jugador &a, const Jugador &b){ //Los ordena por puntos
        return a.puntos > b.puntos; //El que tiene mas puntos primero
    });

    //Impresion bonita del reporte
    cout << "\n===== REPORTE DE JUGADORES =====\n";
    for (auto &jug : lista) {
        cout << "Alias: " << jug.alias << "\n";
        cout << "Puntos: " << jug.puntos << "\n";
        cout << "Ultima fecha: " << jug.fecha << "\n";
        cout << "--------------------------------\n";
    }
}

#endif