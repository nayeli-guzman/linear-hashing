
#include "screen.h"
#include <iostream>


using namespace std;


Screen* Screen::instancia = nullptr;

int main() {

    cout << "Cargando Visualizador ...";

    Screen* screen = Screen::getInstance();
    screen->execute();

    return 0;
}
