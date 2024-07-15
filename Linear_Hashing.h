
#ifndef AED_LINEAR_HASH_H
#define AED_LINEAR_HASH_H

#include <iostream>
#include <cmath>
#include "menu.h"

using namespace std;

template<typename TK>
struct Nodo {
    Nodo* next;
    TK key;
    Nodo(TK _key) : key(_key), next(nullptr) {}
    void display() {
        Nodo<TK>* temp = this;
        while (temp != nullptr) {
            cout << temp->key << " ";
            temp = temp->next;
        }
    }
};

template<typename TK=int>
class Linear_Hashing {
private:
    Menu<>& menu; // agregar visibilidad a las funcionalidades
    int capacity;
    Nodo<TK>** buckets;
    int p, // puntero que indica que fila bucket hará split
        m_0, // cantidad inicial de filas buckets
        level, // cuantas veces se ha recorrido toda la tabla
        M, // cantidad de filas buckets aperturados hasta el momento
        n // cantidad de elementos añadidos
        ;
    float upper_bound, lower_bound, factor;
public:

    Linear_Hashing(Menu<>& _menu) : menu(_menu), p(0), m_0(M_0), level(0),
                                    M(M_0), n(0), upper_bound(0.75), lower_bound(0.3),
                                    factor(0) {
        capacity = 200;
        buckets = new Nodo<TK>* [capacity];

        for (int i=0; i<capacity; i++)
            buckets[i] = nullptr; // inicializa cada bucket fila en nullptr
    }

    void insert(TK key) {

        if (find(key, false)) return;

        int index = find_index(key); // encuentra el índice con la función hash correcta

        push_front(buckets[index], key); // insertar en el índice que le corresponde

        menu.update_description(key, p, level); // funcion para visibilidad en pantalla
        menu.set_key(index, key); // funcion para visibilidad en pantalla

        n++;  // cuando se inserta un elemento aumenta
        update_factor(); // actualizar el factor de llenado

        menu.update_factor(factor, Insert); // funcion para visibilidad en pantalla


        if (factor >= upper_bound) {
            split(); // split cuando el factor excede el threshold
            //display();
        }
    }

    bool find(TK key, bool animate = true) {

        int index = find_index(key); // encontrar indice correcto
        Nodo<TK>* temp = buckets[index]; // puntero al bucket fila donde está la key a buscar

        while (temp != nullptr && temp->key != key) // iterar hasta encontralo o no
            temp = temp->next;

        if (animate)
            menu.search_animation(index, key); // funcion para visibilidad en pantalla

        return temp; // null si no existe

    }

    void borrar(TK key) {

        int index = find_index(key); // encontrar índice correcto
        Nodo<TK>* temp = buckets[index]; // puntero al bucket fila donde debería estar key

        if (temp == nullptr) return; // key no existe

        menu.update_description(key, p, level); // funcion para visibilidad en pantalla
        if (temp->key == key) {
            pop_front(buckets[index]); // en caso la key se encuentre como cabecera
        }
        else {

            while (temp->next != nullptr && temp->next->key != key) // identificar una key anterior de la que se quiere borrar
                temp = temp->next;
            if (temp->next == nullptr || temp->next->key != key) // key no existe
                return;

            Nodo<TK>* waste = temp->next; // conexiones se actualizan
            temp->next = temp->next->next;
            delete waste;
            waste = nullptr;
        }

        n--; // se borró un elemento
        update_factor(); // se actualiza el factor

        menu.update_factor(factor, Delete); // funcion para visibilidad en pantalla
        menu.delete_key(index, key); // funcion para visibilidad en pantalla

        if (M > m_0 && factor < lower_bound) { // si el factor es menor al limite menor se hace group
            group();
        }
    }

    void display() {
        for (int i=0; i<M; i++) {
            if (buckets[i]) {
                buckets[i]->display();
                cout << endl;
            }
        }
    }

private:

    void split() {

        create_bucket(); // se crea un nuevo bucket fila

        Nodo<TK>* temp = buckets[p]; // puntero al bucket fila al que apunta p para redistribuirlo entre ese mismo y el nuevo creado
        Nodo<TK>* lista = nullptr; // bucket fila que reemplazará al bucket fila que apuntaba p

        while (temp != nullptr) { // se itera en el bucket fila al que apunta p
            int new_index = hash_function(temp->key,1); // cada key se vuelve a hashear
            temp = temp->next;
            TK waste = pop_front(buckets[p]); // se borra la key que se volvió a hashear
            if (new_index == p) {
                push_front(lista,waste);
                menu.set_key(p, waste, true); // funcion para visibilidad en pantalla
            }
            else {
                push_front(buckets[new_index], waste); // se inserta en el nuevo bucket fila
                menu.set_key(new_index, waste, true); // funcion para visibilidad en pantalla
            }
        }
        buckets[p] = lista; // actualización del bucket fila que hizo split
        increase_p(); // p actualiza su valor

        // update factor con "DELETE" porque no hará ningún comentario
        update_factor(); // nuevo factor debido a que se cambió M
        menu.update_factor(factor, Delete); // funcion para visibilidad en pantalla

    }
    void group() {
        decrease_p(); // actualizar el valor de p
        Nodo<TK>* temp = buckets[M-1]; // se hará group en el último bucket fila

        menu.set_table(M-1, true, p); // funcion para visibilidad en pantalla

        while (temp != nullptr) { // se itera en el bucket fila que hará group
            int new_index = hash_function(temp->key, 0); // hallar nuevos índices
            temp = temp->next;
            TK waste = pop_front(buckets[M-1]); //se va eliminando de uno en uno los keys
            push_front(buckets[new_index], waste); // se reubican segun su nuevo indice
        }

        M--; // la tabla decrece en un bucket fila

        auto aux = buckets[p];
        while (aux != nullptr) {
            menu.set_key(p, aux->key);
            aux = aux->next;
        }

        update_factor();
        menu.update_factor(factor, Delete);
    }
    void decrease_p() {
        p = p - 1;
        if (p < 0) {
            level = level - 1;
            p = m_0 * pow(2, level)-1;
        }
    }
    void update_factor() {
        factor = (float) n / ((float)(MAX_H*M));
    }
    void increase_p() {
        int m = pow(2,level) * m_0;
        p = (p+1) % m;
        if (p == 0)
            level++;
        menu.update_p(p);
    }

    void create_bucket() {
        M++;
        menu.set_table(M, true, p);
    }

    TK pop_front(Nodo<TK>* &current) {
        if (current == nullptr)
            return -1;
        TK temp = current->key;
        Nodo<TK>* waste = current;
        current = current->next;
        delete waste;
        waste = nullptr;
        return temp;
    }

    int find_index(TK key) {
        int index;
        index = hash_function(key, 0); // 0: función hash principal
        if (index < p)
            index = hash_function(key, 1); // 1: función hash secundaria
        return index;
    }
    int hash_function(TK key, int i) {
        int k = static_cast<int> (key),
            m = m_0 * pow(2,level + i) ;
        return k % m;
    }
    void push_front(Nodo<TK>* &current, TK key) {
        Nodo<TK>* nuevo = new Nodo<TK>(key);
        nuevo->next = current;
        current = nuevo;
    }

};


#endif
