#include <iostream>
#include "Linear_Hashing.h"
using namespace std;

int main() {

    Linear_Hashing<int> hash;

    for (int i=0; i<27; i++)
        hash.insert(i);
    
    hash.display();
    //hash.values();


    return 0;
}