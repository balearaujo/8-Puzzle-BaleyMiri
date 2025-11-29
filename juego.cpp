#include <iostream>
#include <string>
using namespace std;

class MatrizJugable{
    private:
    int matriz[3][3];
    int posCeroX=-1;
    int posCeroY=-1;

    public:
    MatrizJugable(){
        for(int i=0;i<3;i++){
            for(int j=0;j<3;j++){
                matriz[i][j]=0;
            }
        }
    }

    void matrizRandom(){
        bool usado[9]={false,false,false,false,false,false,false,false,false};
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int numRand;
                do {
                    numRand = rand() % 9; // Genera un número aleatorio entre 0 y 8
                } while (usado[numRand]);
                usado[numRand] = true;
                matriz[i][j] = numRand;
                if (numRand == 0) {
                    posCeroX = i;
                    posCeroY = j;
                }
            }
        }
    }
    
    void imprimirMatriz() {
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

                    for (int p = 0; p < espacioIzq; p++){
                        cout << " ";
                    }

                    if (num == 0){
                        cout<<" ";
                    } else {
                        cout << num;
                    }
                    for (int p = 0; p < espacioDer; p++){
                        cout << " ";
                    }
                } else {
                    for (int p = 0; p < ancho; p++){
                        cout << " ";
                    }
                }

                cout << "\033[0m";
            }

            cout << "|\n";
        }
    }

    for (int j = 0; j < 3; j++) {
        cout << "+";
        for (int k = 0; k < ancho; k++) cout << "-";
    }
    cout << "+\n";
    }

    void MoverFicha(){
        cout<<"Que ficha mover: ";
        int ficha;
        cin>>ficha;
        if (ficha<0 || ficha>8){
            cout<<"Ficha invalida\n";
            return;
        } 
        for (int i=0;i<3;i++){
            for (int j=0;j<3;j++){
                if (matriz[i][j]==ficha){
                    if ((abs(posCeroX-i)==1 && posCeroY==j) || (abs(posCeroY-j)==1 && posCeroX==i)){
                        matriz[posCeroX][posCeroY]=ficha;
                        matriz[i][j]=0;
                        posCeroX=i;
                        posCeroY=j;
                        return;
                    } else {
                        cout<<"Movimiento invalido\n";
                        return;
                    }
                    return;
                }
            }
        }
    }
};

int main(){
    MatrizJugable juego;
    juego.matrizRandom();
    juego.imprimirMatriz();
    while (true) {
        juego.MoverFicha();
        juego.imprimirMatriz();
        cout << "Mover otra ficha? (1/0): ";
        int moverOtra;
        cin >> moverOtra;
        if (moverOtra == 0) {
            break;
        }
    }

    cout<<"Otra matriz? (1/0): ";
    int ans;
    cin>>ans;
    while(ans!=0){
        juego.matrizRandom();
        juego.imprimirMatriz();
        cout<<"Otra matriz? (1/0): ";
        cin>>ans;
    }
}