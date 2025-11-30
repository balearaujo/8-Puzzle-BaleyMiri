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

class tablero{
    private:
        int matriz[3][3];
        int movimientos;
        int id=0;

    public:
        static int factorial[9];

        tablero(){
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++)matriz[i][j]=3*i+j;
            }
            calcular_id();
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

        int dist_manhattan(tablero objetivo)const{
            int distancia=0;

            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    for(int m=0;m<3;m++){
                        for(int r=0;r<3;r++){
                            if(matriz[i][j]==objetivo.matriz[m][r])distancia+=abs(i-m)+abs(j-r);
                        }
                    }
                }
            }

            return distancia;
        }

        void print() {
            int ancho=10;   
            int altura=5;   

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
                        cout<<"\033[105m"; 

                        if (h==altura/2) {
                            int num=matriz[i][j];

                            int espacioDer=(ancho-1)/2;
                            int espacioIzq=ancho-1-espacioDer;

                            for (int p=0; p<espacioIzq; p++){
                                cout<<" ";
                            }

                            if (num==0){
                                cout<<" ";
                            } else {
                                cout<<num;
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
    }

        vector<tablero>calcular_movimientos(){
            vector<tablero>mov;
            for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    if(matriz[i][j]==0){
                        if(i){
                            swap(matriz[i-1][j],matriz[i][j]);
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i-1][j],matriz[i][j]);
                        }
                        
                        if(i<2){
                            swap(matriz[i+1][j],matriz[i][j]);
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i+1][j],matriz[i][j]);
                        }

                        if(j){
                            swap(matriz[i][j-1],matriz[i][j]);
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i][j-1],matriz[i][j]);
                        }
                        
                        if(j<2){
                            swap(matriz[i][j+1],matriz[i][j]);
                            tablero estado(matriz,movimientos+1);
                            mov.push_back(estado);
                            swap(matriz[i][j+1],matriz[i][j]);
                        }
                    }
                }
            }

            return mov;
        }

        void calcular_id(){
            int arr[9];

            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    arr[3*i+j] = matriz[i][j];
                }
            }

            id = 0;
            for (int i = 0; i < 9; i++) {
                int c = 0;
                for (int j = i + 1; j < 9; j++) {
                    if (arr[j] < arr[i]) c++;
                }
                id += c * factorial[8 - i];
            }
        }

        int get_id(){return id;};

        int get_mov(){return movimientos;};
};

int tablero::factorial[9] = {1,1,2,6,24,120,720,5040,40320};

struct Nodo {
    tablero estado;
    int g, h, f;
    Nodo* padre = nullptr; 

    // operador invertido para mantener compatibilidad mínima si se usa heap por valor
    bool operator<(const Nodo &o) const {
        return f > o.f;
    }
};

bool vis[C];

vector<tablero> solucion(tablero inicial, tablero final = tablero()){
    for(int i=0;i<C;i++)vis[i]=0;

    int id_objetivo = final.get_id();

    if(inicial.get_id() == id_objetivo)return {};

    vector<tablero>sol;

    // priority_queue de punteros con comparator lambda (min-heap por f)
    priority_queue<Nodo*, vector<Nodo*>, function<bool(Nodo*,Nodo*)>> posiciones(
        [](Nodo* a, Nodo* b){ return a->f > b->f; }
    );

    Nodo inicio;
    inicio.estado = inicial;
    inicio.g = inicial.get_mov();
    inicio.h = inicial.dist_manhattan(final);
    inicio.f = inicio.h + inicio.g;

    // insertamos el nodo inicial como puntero en el heap
    Nodo* inicio_ptr = new Nodo(inicio);
    vis[inicial.get_id()]=1;

    posiciones.push(inicio_ptr);

    Nodo fin;

    while(!posiciones.empty()){
        Nodo* top = posiciones.top();
        posiciones.pop();
        auto movimientos = top->estado.calcular_movimientos();
        for(auto posibilidad: movimientos){
            if(vis[posibilidad.get_id()])continue;
            vis[posibilidad.get_id()] = 1;

            if(posibilidad.get_id() == id_objetivo){
                fin.padre = top;
                fin.estado = final;
                break;
            } else {
                Nodo* pos = new Nodo(); 
                pos->padre = top;
                pos->estado = posibilidad;
                pos->g = posibilidad.get_mov();
                pos->h = posibilidad.dist_manhattan(final);
                pos->f = pos->g + pos->h;
                posiciones.push(pos);
            }
        }
        if(vis[id_objetivo])break;
    }

    if(vis[id_objetivo]==0)return {};

    while(true){
        sol.push_back(fin.estado);

        if(fin.padre == nullptr)break;
        fin = *fin.padre;
    }

    reverse(sol.begin(),sol.end());

    return sol;

}

void simulacion(){

    int averSiJala[3][3];
    int averSiJala2[3][3];


    cout << "Ingresar el estado inicial (que sea valido) y 0 para el espacio\n";

    for(int i=0;i<3;i++)for(int j=0;j<3;j++)cin>>averSiJala[i][j];

    cout << "Ingresar el estado final (que sea valido) y 0 para el espacio \n";

    for(int i=0;i<3;i++)for(int j=0;j<3;j++)cin>>averSiJala2[i][j];

    tablero inicio(averSiJala);
    tablero final(averSiJala2);

    auto respuesta = solucion(inicio, final);

    for(auto camino:respuesta)camino.print();

    if(respuesta.size()==0){
        cout << (inicio.get_id() == final.get_id() ? "Son iguales": "No hay solucion");
    }
}