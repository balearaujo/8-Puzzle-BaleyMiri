#include <iostream>
using namespace std;
#include "algoritmo.hpp"
#include "juego.hpp"

int main(){
    cout << "Bienvenido al juego del 8!\n";
    cout << "Elija una opcion:\n";
    cout << "1. Jugar manualmente\n";
    cout << "2. Ver solucion de un tablero\n";
    cout << "3. Mostrar reporte de jugadores\n";
    cout << "4. Salir\n";
    int opcion;
    cin >> opcion;

    while(opcion != 3){
        if(opcion == 1){
            jugarManualmente();
        } else if(opcion == 2){
            simulacion();
        } else if(opcion == 3){
            //implementar reporte de jugadores
        } else if (opcion == 4){
            break;
        } else {
            cout << "Opcion invalida, intente de nuevo.\n";
        }

        cout << "Elija una opcion:\n";
        cout << "1. Jugar manualmente\n";
        cout << "2. Ver solucion de un tablero\n";
        cout << "3. Salir\n";
        cin >> opcion;
    }

    cout << "Gracias por jugar!\n";
    return 0;
}