#ifndef AED_LINEAR_HASHING_H
#define AED_LINEAR_HASHING_H

#include <iostream>
#include <cmath>
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

template<typename TK>
class Linear_Hashing {
    int capacity;
    Nodo<TK>** buckets;
    int p, m_0, level, M, n, max;
    float upper_bound, lower_bound, factor;
public:

    Linear_Hashing() : p(0), m_0(4), level(0), M(4), n(0), upper_bound(0.75), lower_bound(0.3),factor(0), max(4) {
        capacity = 200;
        buckets = new Nodo<TK>* [capacity];
        for (int i=0; i<capacity; i++)
            buckets[i] = nullptr;
    }

    void insert(TK key) {
        int index = find_index(key);
        push_front(buckets[index], key);
        n++;
        update_factor();

        if (factor >= upper_bound) {
            split();
        }
    }

    bool find(TK key) {
        int index = find_index(key);
        Nodo<TK>* temp = buckets[index];

        while (temp != nullptr && temp->key != key)
            temp = temp->next;

        return temp;
    }

    void borrar(TK key) {
        int index = find_index(key);
        Nodo<TK>* temp = buckets[index];

        if (temp->key == key) {
            pop_front(temp);
            return;
        }

        while (temp->next != nullptr && temp->next->key != key)
            temp = temp->next;

        Nodo<TK>* waste = temp->next;
        temp->next = temp->next->next;
        delete waste;
        waste = nullptr;
        n--;
        update_factor();
        if (factor < lower_bound)
            group();
    }

    void group() {
        decrease_p();
        Nodo<TK> temp = buckets[M-1];
        while (temp != nullptr) {
            int new_index = hash_function(temp.key, 0);
            TK waste = pop_front(buckets[M-1]);
            temp = temp.next;
            push_front(buckets[new_index], waste);
        }
        M--;
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

    void split() {
        create_bucket();
        Nodo<TK>* temp = buckets[p];
        Nodo<TK>* lista = nullptr;

        //temp->display(); cout << endl;

        while (temp != nullptr) {
            int new_index = hash_function(temp->key,1);
            temp = temp->next;
            TK waste = pop_front(buckets[p]);
            if (new_index == p)
                push_front(lista,waste);
            else
                push_front(buckets[new_index], waste);
        }
        buckets[p] = lista;
        increase_p();

    }

    void increase_p() {
        int m = pow(2,level) * m_0;
        p = (p+1) % m;
        if (p == 0)
            level++;
    }

    void create_bucket() {
        M++;
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
            buckets[i]->display();
            cout << endl;
        }
    }

    int find_index(TK key) {
        int index;
        index = hash_function(key, 0);
        if (index < p)
            index = hash_function(key, 1);
        return index;
    }

    int hash_function(TK key, int i) {
        int k = static_cast<int> (key), m = m_0*pow(2,level + i) ;
        return k % m;
    }

};

#endif //AED_LINEAR_HASHING_H
