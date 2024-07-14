
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
    Menu<>& menu;
    int capacity;
    Nodo<TK>** buckets;
    int p, m_0, level, M, n, max;
    float upper_bound, lower_bound, factor;
public:

    Linear_Hashing(Menu<>& _menu) : menu(_menu), p(0), m_0(4), level(0), M(4), n(0), upper_bound(0.75), lower_bound(0.3),factor(0), max(4) {
        capacity = 200;
        buckets = new Nodo<TK>* [capacity];
        for (int i=0; i<capacity; i++)
            buckets[i] = nullptr;
    }

    void insert(TK key) {

        if (find(key, false)) return;

        int index = find_index(key);
        push_front(buckets[index], key);

        menu.update_description(Insert, key, p, m_0, level);
        menu.set_key(index, key);

        n++;
        update_factor();

        menu.update_factor(factor, Insert);


        if (factor >= upper_bound) {

            split();
            display();
        }
    }


    void split() {
        create_bucket();

        Nodo<TK>* temp = buckets[p];
        Nodo<TK>* lista = nullptr;

        //temp->display(); cout << endl;

        while (temp != nullptr) {
            int new_index = hash_function(temp->key,1);
            temp = temp->next;
            TK waste = pop_front(buckets[p]);
            if (new_index == p) {
                push_front(lista,waste);
                cout << "p: " << p << endl;
                menu.set_key(p, waste, true);
            }
            else {
                push_front(buckets[new_index], waste);
                menu.set_key(new_index, waste, true);
            }
        }
        buckets[p] = lista;
        increase_p();

    }

    bool find(TK key, bool animate = true) {
        int index = find_index(key);
        Nodo<TK>* temp = buckets[index];

        while (temp != nullptr && temp->key != key)
            temp = temp->next;

        if (animate)
        menu.search_animation(index, key);
        return temp;
    }

    void borrar(TK key) {
        int index = find_index(key);
        Nodo<TK>* temp = buckets[index];

        if (temp == nullptr) return; //?

        menu.update_description(Delete, key, p, m_0, level);
        if (temp->key == key) {
            pop_front(buckets[index]);
        }

        else {

            while (temp->next != nullptr && temp->next->key != key)
                temp = temp->next;
            if (temp->next == nullptr || temp->next->key != key)
                return;


            Nodo<TK>* waste = temp->next;
            temp->next = temp->next->next;
            delete waste;
            waste = nullptr;
        }

        n--;
        update_factor();
        menu.update_factor(factor, Delete);
        menu.delete_key(index, key);

        if (M > m_0 && factor < lower_bound) {
            group();
        }
        display();
    }

    void group() {
        decrease_p();
        Nodo<TK>* temp = buckets[M-1];
        menu.set_table(M-1, true, p);

        while (temp != nullptr) {
            int new_index = hash_function(temp->key, 0);
            TK waste = pop_front(buckets[M-1]);
            temp = temp->next;
            push_front(buckets[new_index], waste);
        }
        M--;

        auto aux = buckets[p];
        while (aux != nullptr) {
            menu.set_key(p, aux->key);
            aux = aux->next;
        }

    }

    void decrease_p() {
        p = p - 1;
        if (p < 0) {
            level = level - 1;
            p = m_0 * pow(2, level)-1;
        }
    }

    void values () {
        cout << "p: " << p << endl;
        cout << "level: " << level << endl;
    }

    void update_factor() {

        factor = (float) n / ((float)(max*M));
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

    void push_front(Nodo<TK>* &current, TK key) {
        Nodo<TK>* nuevo = new Nodo<TK>(key);
        nuevo->next = current;
        current = nuevo;
    }

    void display() {
        for (int i=0; i<M; i++) {
            if (buckets[i]) {
                buckets[i]->display();
                cout << endl;
            }
        }
    }

    int find_index(TK key) {
        int index;
        index = hash_function(key, 0);
        if (index < p)
            index = hash_function(key, 1);
        // menu.update_description(Search, key, p, m_0, level);
        return index;
    }

    int hash_function(TK key, int i) {
        int k = static_cast<int> (key), m = m_0*pow(2,level + i) ;
        return k % m;
    }

};


#endif
