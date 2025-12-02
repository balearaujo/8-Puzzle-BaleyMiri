#ifndef ALGORITMO_HPP
#define ALGORITMO_HPP

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cstring>

using namespace std;

const int C = 362880;

bool esSoluble(int m[3][3]) { //Revisa si un tablero es soluble
    int arr[9], k = 0;
    for(int i=0;i<3;i++) // Llena un arreglo con los elementos de la matriz
        for(int j=0;j<3;j++)
            arr[k++] = m[i][j]; // Convierte la matriz 3x3 en un arreglo de 9 elementos

    int inv = 0;
    for(int i=0;i<9;i++){
        for(int j=i+1;j<9;j++){
            if(arr[i] && arr[j] && arr[i] > arr[j]) //Cuenta cuantas inversiones hay
                inv++; //agrega a conteo de inversiones
        }
    }

    return (inv % 2 == 0); // Si el numero de inversiones es par, es soluble
}

class tablero{ // Clase que representa un estado del tablero
    private:
        int matriz[3][3];
        int movimientos;
        int id=0;

    public:
        static int factorial[9];
    // Constructores
        tablero(){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++)matriz[i][j]=3*i+j;
            }
            calcular_id();
            movimientos = 0;
        }

        tablero(int _matriz[3][3], int mov=0){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    matriz[i][j]=_matriz[i][j];
                }
            }
            calcular_id();
            movimientos = mov;
        }

        int dist_manhattan(tablero objetivo)const{ // Calcula la distancia Manhattan al estado objetivo
            int distancia=0;

            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    for(int m=0;m<3;m++){
                        for(int r=0;r<3;r++){ // Busca la posicion del numero en el estado objetivo
                            if(matriz[i][j]==objetivo.matriz[m][r])distancia+=abs(i-m)+abs(j-r); // Suma la distancia
                        }
                    }
                }
            }

            return distancia;
        }

        void print() { // Imprime la matriz de manera bonita
            int ancho=10;   
            int altura=5;   

            cout<<"\n";

            for (int i=0; i<3; i++) {
                for (int j = 0; j < 3; j++) {
                    cout<<"+";
                    for (int k = 0; k < ancho; k++){
                        cout << "-";
                    }
                }
                cout << "+\n";

                for (int h=0; h<altura; h++) {
                    for (int j=0; j<3; j++) {
                        cout<<"|";
                        cout<<"\033[105m"; //COLOR ROSA

                        if (h==altura/2) {
                            int num=matriz[i][j];
                            //Centrar numero en la celda
                            int espacioDer=(ancho-1)/2;
                            int espacioIzq=ancho-1-espacioDer;

                            for (int p=0; p<espacioIzq; p++){
                                cout<<" ";
                            }

                            if (num==0){
                                cout<<" "; //Dejar espacio en blanco para el 0
                            } else {
                                cout<<num; // Numero a imprimir
                            }
                            for (int p=0; p<espacioDer; p++){
                                cout<<" ";
                            }
                        } else {
                            for (int p=0; p<ancho; p++){
                                cout<<" ";
                            }
                        }

                        cout<<"\033[0m";
                    }

                    cout<<"|\n";
                }

            }
        
            for (int j=0; j<3; j++) {
                cout<<"+";
                for (int k=0; k<ancho; k++){
                    cout << "-";
                }
            }
            cout << "+\n";

            cout<<"\n";

        }

        vector<tablero>calcular_movimientos(){ // Genera todos los movimientos posibles 
            vector<tablero>mov;
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(matriz[i][j]==0){
                        if(i){
                            swap(matriz[i-1][j],matriz[i][j]); // Mueve el 0 hacia arriba
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i-1][j],matriz[i][j]);
                        }
                        
                        if(i<2){
                            swap(matriz[i+1][j],matriz[i][j]); // Mueve el 0 hacia abajo
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i+1][j],matriz[i][j]);
                        }

                        if(j){
                            swap(matriz[i][j-1],matriz[i][j]); // Mueve el 0 hacia la izquierda
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i][j-1],matriz[i][j]);
                        }
                        
                        if(j<2){
                            swap(matriz[i][j+1],matriz[i][j]); // Mueve el 0 hacia la derecha
                            tablero estado(matriz,movimientos+1); // Crea un nuevo estado
                            mov.push_back(estado); // Agrega el nuevo estado a la lista
                            swap(matriz[i][j+1],matriz[i][j]); // Restaura la matriz
                        }
                    }
                }
            }

            return mov;
        }

        void calcular_id(){ // Calcula el ID 
            int arr[9];

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    arr[3*i+j] = matriz[i][j]; //Convierte de matriz a arreglo
                }
            }

            id = 0;
            for (int i = 0; i < 9; i++) {
                int c = 0;
                for (int j = i + 1; j < 9; j++) {
                    if (arr[j] < arr[i]) c++; // Cuenta cuantos numeros son menores que arr[i] despues de la posicion i
                }
                id += c * factorial[8 - i]; // Calcula el ID
            }
        }

        int get_movimiento( tablero meta){ // Obtiene la ficha que debe moverse para llegar al estado meta
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(matriz[i][j]==0)return meta.matriz[i][j]; // Devuelve el numero que esta en la posicion del 0
                }
            }
            return -1;
        }

        int get_id(){return id;}; // Obtiene el ID del estado

        int get_mov(){return movimientos;}; //Obtiene el movimiento
};

int tablero::factorial[9] = {1,1,2,6,24,120,720,5040,40320}; // Factoriales de 0! a 8!

struct Nodo { // Nodo para el algoritmo A*
    tablero estado;
    int g, h, f;
    Nodo* padre = nullptr; 

    bool operator<(const Nodo &o) const { 
        return f > o.f; // Para la cola de prioridad (min-heap)
    }
};

bool vis[C];

vector<tablero> solucion(tablero inicial, tablero final = tablero()){ // Algoritmo A* para encontrar la solucion
    for(int i=0;i<C;i++)vis[i]=0; // arreglo de visitados en 0

    int id_objetivo = final.get_id(); // Obtiene el ID del estado objetivo

    if(inicial.get_id() == id_objetivo)return {};

    vector<tablero>sol; // Vector para almacenar la solucion

    priority_queue<Nodo*, vector<Nodo*>, function<bool(Nodo*,Nodo*)>> posiciones( // Min-heap basado en f = g + h
        [](Nodo* a, Nodo* b){ return a->f > b->f; } // Comparador
    );

    // Nodo inicial
    Nodo inicio;
    inicio.estado = inicial;
    inicio.g = inicial.get_mov();
    inicio.h = inicial.dist_manhattan(final);
    inicio.f = inicio.h + inicio.g;

    Nodo* inicio_ptr = new Nodo(inicio); // Puntero al nodo inicial
    vis[inicial.get_id()]=1;

    posiciones.push(inicio_ptr); // Agrega el nodo inicial a la cola

    Nodo fin; // Nodo para almacenar el nodo final

    while(!posiciones.empty()){ // Mientras haya nodos por explorar
        Nodo* top = posiciones.top();
        posiciones.pop();
        auto movimientos = top->estado.calcular_movimientos(); //Movimientos posibles desde donde estamos
        for(auto posibilidad: movimientos){
            if(vis[posibilidad.get_id()])continue; // Si ya fue visitado, salta
            vis[posibilidad.get_id()] = 1; // Marca como visitado

            if(posibilidad.get_id() == id_objetivo){ // Si es el objetivo
                fin.padre = top; /// Guarda el padre
                fin.estado = final; // Guarda el estado final
                break;
            } else {
                // Crear nuevo nodo
                Nodo* pos = new Nodo(); 
                pos->padre = top; // Asigna el padre
                pos->estado = posibilidad; // Asigna el estado
                pos->g = posibilidad.get_mov(); // Costo desde el inicio
                pos->h = posibilidad.dist_manhattan(final);//Llama a la funcion de distancia manhattan
                pos->f = pos->g + pos->h; // Costo total
                posiciones.push(pos);// Agrega a la cola
            }
        }
        if(vis[id_objetivo])break;// Si ya se encontro el objetivo, sale del ciclo
    }

    if(vis[id_objetivo]==0)return {}; // Si no se encontro solucion, retorna vacio

    while(true){
        sol.push_back(fin.estado); // Agrega el estado a la solucion

        if(fin.padre == nullptr)break;
        fin = *fin.padre; // Mueve al padre
    }

    reverse(sol.begin(),sol.end()); // Invierte la solucion para que vaya del inicio al fin

    return sol;

}



void simulacion(){
    //Matrices para el inicio y fin
    int averSiJala[3][3]; 
    int averSiJala2[3][3];


    cout << "Ingresar el estado inicial (que sea valido) y 0 para el espacio\n";

    for(int i=0;i<3;i++)for(int j=0;j<3;j++)cin>>averSiJala[i][j]; //llenar matriz inicial

    // validar solvencia inicial
    if(!esSoluble(averSiJala)){
        cout << "El tablero inicial NO es resoluble.\n";
        return;
    }

    cout << "Ingresar el estado final (que sea valido) y 0 para el espacio \n";

    for(int i=0;i<3;i++)for(int j=0;j<3;j++)cin>>averSiJala2[i][j]; //llenar matriz final

    // validar solvencia final
    if(!esSoluble(averSiJala2)){
        cout << "El tablero final NO es resoluble.\n";
        return;
    }

    tablero inicio(averSiJala);
    tablero final(averSiJala2);

    auto respuesta = solucion(inicio, final); // Obtener la solucion

    for(auto camino:respuesta)camino.print(); //imprime solucion

    if(respuesta.size()==0){ // Si no hay solucion
        cout << (inicio.get_id() == final.get_id() ? "Son iguales": "No hay solucion");
    } else {
        cout << "Solucion encontrada en " << respuesta.size() - 1 << " movimientos.\n";
    }
}

#endif
