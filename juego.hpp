#ifndef JUEGO_HPP
#define JUEGO_HPP

#include <iostream>
#include <string>
#include "Puntajes.hpp"
#include "algoritmo.hpp"

using namespace std;

class MatrizJugable {
private:
    int matriz[3][3];
    int posCeroX = -1;
    int posCeroY = -1;

public:

    // Patrones meta
    int matNivel1[3][3] = {
        {1,2,3},
        {4,5,6},
        {8,7,0}
    };

    int matNivel2[3][3] = {
        {1,2,3},
        {8,0,4},
        {7,6,5}
    };

    int matMeta[3][3];

    MatrizJugable() {
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                matriz[i][j] = 0;
        posCeroX = posCeroY = -1;
    }

    // -------------------------------
    // Impresión (idéntica a la tuya)
    // -------------------------------
    void imprimirMatriz() {
        int ancho = 10;
        int altura = 5;

        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << "+";
                for (int k = 0; k < ancho; k++) cout << "-";
            }
            cout << "+\n";

            for (int h = 0; h < altura; h++) {
                for (int j=0; j<3; j++) {
                    cout << "|";
                    cout << "\033[105m";

                    if (h == altura/2) {
                        int num = matriz[i][j];

                        int espacioDer = (ancho-1)/2;
                        int espacioIzq = ancho-1-espacioDer;

                        for (int p=0;p<espacioIzq;p++) cout<<" ";

                        if (num == 0) cout << " ";
                        else cout << num;

                        for (int p=0;p<espacioDer;p++) cout<<" ";

                    } else {
                        for (int p=0;p<ancho;p++) cout<<" ";
                    }

                    cout << "\033[0m";
                }
                cout << "|\n";
            }
        }

        for (int j=0;j<3;j++) {
            cout << "+";
            for (int k=0;k<ancho;k++) cout << "-";
        }
        cout << "+\n";
    }

    // -------------------------------
    // Movimiento y sugerencias
    // -------------------------------
    void MoverFicha(int nivel) {

        cout << "Que ficha mover (0= Sugerencia, -1= Rendirse): ";
        int ficha;
        if(!(cin >> ficha)){
            cin.clear();
            string tmp; getline(cin,tmp);
            cout << "Entrada invalida\n";
            return;
        }

        // REGRESAR AL MENU
        if(ficha == -1) {
            throw -1; 
        }

        // SUGERENCIA
        if(ficha == 0){
            if(nivel == 2){
                cout << "Es nivel dificil, no hay sugerencias.\n";
            } else {
                // usar A* y tu get_movimiento existente
                auto ruta = solucion(tablero(matriz), tablero(matMeta));
                if(ruta.size() < 2){
                    cout << "No hay sugerencia (ya en meta o sin solucion)\n";
                } else {
                    int respuesta = tablero(matriz).get_movimiento(ruta[1]);
                    if(respuesta > 0)
                        cout << "Recomiendo mover la ficha: " << respuesta << "\n";
                    else
                        cout << "No se pudo determinar la sugerencia.\n";
                }
            }
            return;
        }

        // MOVIMIENTO NORMAL
        if (ficha < 0 || ficha > 8) {
            cout << "Ficha invalida\n";
            return;
        }

        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(matriz[i][j] == ficha){
                    if ((abs(posCeroX - i) == 1 && posCeroY == j) ||
                        (abs(posCeroY - j) == 1 && posCeroX == i)) {

                        matriz[posCeroX][posCeroY] = ficha;
                        matriz[i][j] = 0;

                        posCeroX = i;
                        posCeroY = j;
                        return;
                    } else {
                        cout << "Movimiento invalido\n";
                        return;
                    }
                }
            }
        }
    }

    // -------------------------------
    // Meta check e impresión patrón
    // -------------------------------
    bool esMeta() {
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++)
                if (matriz[i][j] != matMeta[i][j])
                    return false;
        return true;
    }

    void imprimirPatron(int patron[3][3]) {
        cout << "\nPatron que debes seguir:\n";
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(patron[i][j] == 0) cout << "   ";
                else cout << patron[i][j] << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    void seleccionarNivel(int nivel) {
        int (*patron)[3];

        if(nivel == 1) patron = matNivel1;
        else if(nivel == 2) patron = matNivel2;
        else {
            cout << "Nivel invalido, usando Nivel 1.\n";
            patron = matNivel1;
        }

        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                matMeta[i][j] = patron[i][j];

        imprimirPatron(patron);
    }

    // -------------------------
    // Funciones para generar nivel REAL desde la meta (garantiza solvencia)
    // -------------------------
    void buscarCero() {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(matriz[i][j] == 0){
                    posCeroX = i;
                    posCeroY = j;
                    return;
                }
            }
        }
    }

    vector<MatrizJugable> movimientosPosibles() {
        vector<MatrizJugable> movs;
        int dx[4] = {-1,1,0,0};
        int dy[4] = {0,0,-1,1};

        for(int k=0; k<4; k++){
            int nx = posCeroX + dx[k];
            int ny = posCeroY + dy[k];

            if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3){
                MatrizJugable copia = *this;
                swap(copia.matriz[posCeroX][posCeroY], copia.matriz[nx][ny]);
                copia.posCeroX = nx;
                copia.posCeroY = ny;
                movs.push_back(copia);
            }
        }

        return movs;
    }

    void generarNivel(int nivel) {
        // Copiar la meta al tablero inicial
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                matriz[i][j] = matMeta[i][j];

        buscarCero();

        int pasos = (nivel == 1 ? 10 : 40);

        for(int i=0;i<pasos;i++){
            auto movs = movimientosPosibles();
            if(movs.empty()) break;
            int r = rand() % movs.size();
            *this = movs[r];
        }

        // actualizar posCero por si acaso
        buscarCero();
    }

    // función auxiliar pública para pruebas (opcional)
    void imprimirMatrizSimple() {
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++) cout << matriz[i][j] << ' ';
            cout << '\n';
        }
    }

};



// ==============================================
//    JUGAR MANUALMENTE
// ==============================================

void jugarManualmente(){
    MatrizJugable juego;

    cout << "Seleccionar Nivel:\n";
    cout << "1. Facil\n";
    cout << "2. Dificil\n";

    int nivel;
    if(!(cin >> nivel)){
        cin.clear(); string t; getline(cin,t);
        cout << "Entrada invalida\n";
        return;
    }

    juego.seleccionarNivel(nivel);

    cout << "Ingresa tu alias: ";
    string alias, fecha;
    cin >> alias;

    fecha = obtenerFecha();
    int puntos = 0;

    // Usar generador de tablero resoluble según nivel
    juego.generarNivel(nivel);

    juego.imprimirMatriz();

    while(true){
        try {
            juego.MoverFicha(nivel);
        }
        catch(int e){
            if(e == -1){
                cout << "\nTe rendiste. Regresando al menu...\n";
                return;
            }
        }

        puntos++;
        juego.imprimirMatriz();

        if(juego.esMeta()){
            cout << "Ganaste!\n";
            guardarPuntuacion(alias, puntos, fecha);
            break;
        }
    }

    // MULTIJUEGO
    cout << "¿Jugar otra vez? (1 = si / 0 = no): ";
    int ans;
    cin >> ans;

    while(ans != 0){

        puntos = 0;

        juego.generarNivel(nivel);
        juego.imprimirMatriz();
        fecha = obtenerFecha();

        while(true){
            try{
                juego.MoverFicha(nivel);
            }
            catch(int e){
                if(e == -1){
                    cout << "\nTe rendiste. Regresando al menu...\n";
                    return;
                }
            }

            puntos++;
            juego.imprimirMatriz();

            if(juego.esMeta()){
                cout << "Ganaste!\n";
                guardarPuntuacion(alias, puntos, fecha);
                break;
            }
        }

        cout << "¿Jugar otra vez? (1/0): ";
        cin >> ans;
    }
}

#endif
