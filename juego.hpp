#ifndef JUEGO_HPP
#define JUEGO_HPP

#include <iostream>
#include <string>
#include "Puntajes.hpp" // Incluye el archivo de puntajes
#include "algoritmo.hpp" // Incluye el archivo del algoritmo A*

using namespace std;

class MatrizJugable {
private:
    int matriz[3][3]; // Matriz del juego

    //posicion del espacio vacio
    int posCeroX = -1; 
    int posCeroY = -1;

public:
    //Patrones de los 2 distintos niveles
    int matNivel1[3][3] = {
        {1,2,3},
        {4,5,6},
        {7,8,0}
    };

    int matNivel2[3][3] = {
        {1,2,3},
        {8,0,4},
        {7,6,5}
    };
    //Matriz donde se guarda la meta segun el nivel
    int matMeta[3][3];

    // Constructor
    MatrizJugable() {
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++)
                matriz[i][j] = 0;
        posCeroX = posCeroY = -1;
    }

    // Imprimie la matriz de manera bonita
    void imprimirMatriz() {
        //Calculo de medidas
        int ancho = 10; 
        int altura = 5;

        //Imprime bordes de la matriz
        for (int i=0; i<3; i++) {
            for (int j = 0; j < 3; j++) {
                cout << "+";
                for (int k = 0; k < ancho; k++) cout << "-";
            }
            cout << "+\n";

            //Imprime filas de la matriz
            for (int h = 0; h < altura; h++) {
                for (int j=0; j<3; j++) {
                    cout << "|";
                    cout << "\033[105m"; //inicio rosa

                    //Centrar numero en la celda
                    if (h == altura/2) {
                        int num = matriz[i][j]; // Numero a imprimir

                        int espacioDer = (ancho-1)/2;
                        int espacioIzq = ancho-1-espacioDer; // Espacios para centrar

                        for (int p=0;p<espacioIzq;p++) cout<<" ";

                        if (num == 0) cout << " "; //Dejar espacio en blanco para el 0
                        else cout << num;

                        for (int p=0;p<espacioDer;p++) cout<<" ";

                    } else {
                        for (int p=0;p<ancho;p++) cout<<" ";
                    }

                    cout << "\033[0m"; //fin del color
                }
                cout << "|\n";
            }
        }

        //Imprime ultima linea
        for (int j=0;j<3;j++) {
            cout << "+";
            for (int k=0;k<ancho;k++) cout << "-";
        }
        cout << "+\n";
    }

    
    void MoverFicha(int nivel) {
        cout << "Que ficha mover (0= Sugerencia, -1= Rendirse): "; //Pide la ficha a mover
        int ficha;
        if(!(cin >> ficha)){
            cin.clear();
            string tmp; getline(cin,tmp);
            cout << "Entrada invalida\n"; 
            return;
        }

        // Si te rindes usas -1 para salir
        if(ficha == -1) {
            throw -1; 
        }

        // Pistas de movimientos
        if(ficha == 0){
            if(nivel == 2){ // nivel dificil, sin pistas
                cout << "Es nivel dificil, no hay sugerencias.\n";
            } else {
                // usar A* y tu get_movimiento existente
                auto ruta = solucion(tablero(matriz), tablero(matMeta)); // Obtiene la ruta desde el estado actual hasta la meta
                if(ruta.size() < 2){
                    cout << "No hay sugerencia (ya en meta o sin solucion)\n";
                } else {
                    int respuesta = tablero(matriz).get_movimiento(ruta[1]); // Obtiene el siguiente movimiento recomendado
                    if(respuesta > 0)
                        cout << "Recomiendo mover la ficha: " << respuesta << "\n"; // Muestra la sugerencia
                    else
                        cout << "No se pudo determinar la sugerencia.\n";
                }
            }
            return;
        }

        // Movimiento normal
        if (ficha < 0 || ficha > 8) {
            cout << "Ficha invalida\n"; 
            return;
        }

        // Buscar la ficha en la matriz
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                if(matriz[i][j] == ficha){ // Si encuentra la ficha
                    if ((abs(posCeroX - i) == 1 && posCeroY == j) ||
                        (abs(posCeroY - j) == 1 && posCeroX == i)) { // Verifica si es adyacente al 0

                        matriz[posCeroX][posCeroY] = ficha;
                        matriz[i][j] = 0; // Mueve la ficha

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

    bool esMeta() { // Verifica si la matriz actual es igual a la meta
        for (int i=0;i<3;i++)
            for (int j=0;j<3;j++)
                if (matriz[i][j] != matMeta[i][j]) //Compara cada uno
                    return false;
        return true;
    }

    void imprimirPatron(int patron[3][3]) { //Muestra al jugador el patron que debe seguir
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

    //Llena la matriz meta segun el nivel seleccionado
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
                matMeta[i][j] = patron[i][j]; // Copia el patron a la matriz meta

        imprimirPatron(patron);
    }
    
    //funcion para encontrar la posicion del 0
    void buscarCero() {
        for(int i = 0; i < 3; i++){
            for(int j = 0; j < 3; j++){
                if(matriz[i][j] == 0){ // Si encuentra el 0
                    posCeroX = i;
                    posCeroY = j;
                    return;
                }
            }
        }
    }

    vector<MatrizJugable> movimientosPosibles() { // Genera todos los movimientos posibles desde la posicion actual
        vector<MatrizJugable> movs; // Vector para almacenar los movimientos posibles
        // Direcciones posibles: arriba, abajo, izquierda, derecha
        int dx[4] = {-1,1,0,0}; 
        int dy[4] = {0,0,-1,1};

        for(int k=0; k<4; k++){
            int nx = posCeroX + dx[k];
            int ny = posCeroY + dy[k];

            if(nx >= 0 && nx < 3 && ny >= 0 && ny < 3){ // Verifica que la nueva posicion este dentro de los limites
                MatrizJugable copia = *this;
                swap(copia.matriz[posCeroX][posCeroY], copia.matriz[nx][ny]); // Intercambia el 0 con la ficha adyacente
                copia.posCeroX = nx;
                copia.posCeroY = ny; // Actualiza la posicion del 0
                movs.push_back(copia);
            }
        }

        return movs;
    }

    void generarNivel(int nivel) {
        for(int i=0;i<3;i++)
            for(int j=0;j<3;j++) 
                matriz[i][j] = matMeta[i][j]; // Inicializa la matriz con la meta

        buscarCero();

        int pasos = (nivel == 1 ? 10 : 40); // Cantidad de movimientos para mezclar segun el nivel

        for(int i=0;i<pasos;i++){
            auto movs = movimientosPosibles(); // Obtiene los movimientos posibles
            if(movs.empty()) break;
            int r = rand() % movs.size();
            *this = movs[r];
        }

        buscarCero();
    }
    void imprimirMatrizSimple() { //imprime la matriz normal
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++) cout << matriz[i][j] << ' ';
            cout << '\n';
        }
    }

};


void jugarManualmente(){
    MatrizJugable juego;
    // Seleccion de nivel
    cout << "Seleccionar Nivel:\n";
    cout << "1. Facil\n";
    cout << "2. Dificil\n";

    int nivel;
    if(!(cin >> nivel)){
        cin.clear(); string t; getline(cin,t);
        cout << "Entrada invalida\n";
        return;
    }

    juego.seleccionarNivel(nivel); // Configura la matriz meta segun el nivel
    //Ingreso de datos del jugador
    cout << "Ingresa tu alias: ";
    string alias, fecha;
    cin >> alias;

    fecha = obtenerFecha();
    int puntos = 0;

    // Usar generador de tablero resoluble según nivel
    juego.generarNivel(nivel);

    juego.imprimirMatriz();

    //bucle principal del juego
    while(true){
        try {
            juego.MoverFicha(nivel);
        }
        catch(int e){
            if(e == -1){
                cout << "\nBUUUU, el perdedor se rindio. Regresando al menu...\n";
                return;
            }
        }

        puntos++; // Incrementa puntos por cada movimiento
        juego.imprimirMatriz();

        if(juego.esMeta()){
            cout << "Ganaste, eres super proo!\n";
            guardarPuntuacion(alias, puntos, fecha);
            break;
        }
    }

    // Preguntar si quiere continuar jugando
    cout << "¿Jugar otra vez? (1 = si / 0 = no): ";
    int ans;
    cin >> ans;

    //Misma logica de juego en un bucle
    while(ans != 0){ // Mientras quiera seguir jugando

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
                    cout << "\nBUUUU, el perdedor se rindio. Regresando al menu...\n";
                    return;
                }
            }

            puntos++;
            juego.imprimirMatriz();

            if(juego.esMeta()){
                cout << "Ganaste, eres super proo!\n";
                guardarPuntuacion(alias, puntos, fecha);
                break;
            }
        }

        cout << "¿Jugar otra vez? (1/0): ";
        cin >> ans;
    }
}

#endif
