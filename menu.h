
#ifndef AED_MENU_H
#define AED_MENU_H

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <iomanip>

#define BUCKET 80
#define MAX 4
#define SEPARATION 20
using namespace sf;
using namespace std;



enum Operation {
    Insert,
    Delete,
    Search
};

template<typename TK=int>
class Menu {

    //friend class Linear_Hash;
    friend class Screen;
    int M;
    RenderWindow &window;
    RectangleShape b_main, b_dynamic, b_insert, b_find, b_delete;
    Font font_1, font_2;
    Text text_1, text_2, text_3, text_4, text_dynamic, text_p, text_lower, text_comparison,
            text_upper, text_factor, text_space, text_description;
    String string_dynamic = "";
    string string_1 = "Linear Hashing\n  Visualizer", string_2 = "Insertar",
            string_3 = "Eliminar", string_4 = "Buscar", string_p = "p",
            string_lower = "lower bound: 0.3", string_upper = "upper bound: 0.75",
            string_factor = "factor: ", string_comparison = "0.0", string_space = "", string_description="Empieza a insertar elementos!";
    vector<int> positions;
    vector<vector<Text>> extras;
    vector<vector<Sprite*>> buckets;
    Texture* node_image;
    Texture *green_node_image;
    bool indicator = false;

    Vector2f mousePosition;

public:

    Menu(RenderWindow& window_) : window(window_) {

        positions.resize(4,0);
        node_image = new Texture();
        node_image->loadFromFile("C:/utec/AED/images/node.png");
        green_node_image = new Texture();
        green_node_image->loadFromFile("C:/utec/AED/images/green_node.png");
        extras.resize(MAX);
        set_fonts();
        set_backgrounds();
        set_texts();
        set_table(MAX, false);
        set_factor();

    }

    void set_factor() {

        text_lower.setFont(font_2);
        text_lower.setString(string_lower);
        text_lower.setFillColor(Color::Red);
        text_lower.setCharacterSize(15);
        text_lower.setPosition(95,680);

        text_upper.setFont(font_2);
        text_upper.setString(string_upper);
        text_upper.setFillColor(Color::Blue);
        text_upper.setCharacterSize(15);
        text_upper.setPosition(275,680);

        text_factor.setFont(font_2);
        text_factor.setString(string_factor);
        text_factor.setCharacterSize(15);
        text_factor.setPosition(95,660);

        text_comparison.setFont(font_2);
        text_comparison.setString(string_comparison);
        text_comparison.setCharacterSize(15);
        text_comparison.setPosition(150,660);

        text_space.setFont(font_2);
        text_space.setString(string_space);
        text_space.setFillColor(Color::Red);
        text_space.setCharacterSize(15);
        text_space.setPosition(350,660);

        text_description.setFont(font_2);
        text_description.setString(string_description);
        text_description.setFillColor(Color::Black);
        text_description.setCharacterSize(16);
        text_description.setPosition(100,280);
    }

    void set_backgrounds() {
        b_main = RectangleShape(Vector2f(500,800));
        b_main.setFillColor(Color(111,139,166,255));
        b_main.setPosition(0,0);

        b_dynamic = RectangleShape(Vector2f(300,70));
        b_dynamic.setFillColor(Color::White);
        b_dynamic.setPosition(100,200);

        b_insert = RectangleShape(Vector2f(300,70));
        b_insert.setFillColor(Color(225, 239,128,255));
        b_insert.setPosition(100,350);

        b_delete = RectangleShape(Vector2f(300,70));
        b_delete.setFillColor(Color(225, 239,128,255));
        b_delete.setPosition(100,450);

        b_find = RectangleShape(Vector2f(300,70));
        b_find.setFillColor(Color(225, 239,128,255));
        b_find.setPosition(100,550);
    }
    void set_texts() {
        text_1.setFont(font_1);
        text_1.setString(string_1);
        text_1.setCharacterSize(60);
        text_1.setPosition(90,15);

        text_dynamic.setFont(font_1);
        text_dynamic.setFillColor(Color::Black);
        text_dynamic.setCharacterSize(35);
        text_dynamic.setPosition(130,210);

        text_2.setFont(font_2);
        text_2.setString(string_2);
        text_2.setCharacterSize(30);
        text_2.setPosition(190,365);

        text_3.setFont(font_2);
        text_3.setString(string_3);
        text_3.setCharacterSize(30);
        text_3.setPosition(190,465);

        text_4.setFont(font_2);
        text_4.setString(string_4);
        text_4.setCharacterSize(30);
        text_4.setPosition(200,565);

        text_p.setFont(font_2);
        text_p.setString(string_p);
        text_p.setCharacterSize(30);
        text_p.setPosition(750,225);
        text_p.setFillColor(Color::Black);

    }
    void set_fonts() {
        font_1.loadFromFile("C:/utec/AED/fonts/font_1.ttf");
        font_2.loadFromFile("C:/utec/AED/fonts/font_2.ttf");
    }
    void set_table(int _M, bool c, int p=0) {

        M = _M;

        if (c) {

            positions.clear();
            positions.resize(M, 0);

            // 4 x4

            for(int i=0; i<extras.size(); i++) {
                if (i != p) {
                    for (int j=0; j<extras[i].size(); j++)
                        update_key(i,j); // falta ver en que orden se insertan visualmente
                }
            }
            extras[p].clear();
            extras.resize(M);  // 4 x 4,
            buckets.clear(); //
        }

        int Y = start_height_table();

        for (int i=0, j=0; i<M; i++, j=j+BUCKET+SEPARATION) {

            vector<Sprite*> fila;

            for (int k=0, l=0; k<MAX; k++, l=l+BUCKET) {

                Sprite* node = new Sprite();
                node->setTexture(*node_image);
                node->setPosition(800 + l, Y + j);
                fila.push_back(node);
            }

            buckets.push_back(fila);

        }
        cout << "buckets size: " << buckets.size()<< endl;

    }

    float start_height_table() {
        int height_table = BUCKET*M + SEPARATION*(M-1);
        return HEIGHT/2 - height_table/2;
    }
    void set_key(int x, TK key, bool split = false) {

        int Y = start_height_table() + x * BUCKET + x * SEPARATION + 10;

        Text _key;
        _key.setFont(font_2);
        _key.setString(to_string(key));
        _key.setPosition(820 + positions[x]++ * BUCKET, Y);
        _key.setCharacterSize(30);
        _key.setFillColor(Color::Blue);
        if (split) {
            _key.setFillColor(Color::Black);
            extras[x].push_back(_key);

        }
        else {
            extras[x].push_back(_key);
            hold_on();
            extras[x][extras[x].size() - 1].setFillColor(Color::Black);
        }
    }

    void hold_on(int milisecs = 500) {
        draw();
        sleep(milliseconds(milisecs));
    }

    void delete_key(int x, TK key) {
        // hallar el key y borrarlo
        auto it = std::find_if(extras[x].begin(), extras[x].end(), [key](const sf::Text& text) {
            return text.getString() == std::to_string(key);
        });

        if (it != extras[x].end()) {
            extras[x].erase(it);
        }

        // decrementar positions
        positions[x] =  0;

        // actualizar los positions de cada Text en extras
        for (int j = 0; j < extras[x].size(); j++) {
            update_key(x, j);
        }
    }

    void update_key(int x, int y) {
        int Y = start_height_table() + x * BUCKET + x * SEPARATION + 10;
        extras[x][y].setPosition(820 + positions[x]++ * BUCKET, Y);
    }

    void update_mouse_position() {
        mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        // std::cout<<"Mouse at: " << mousePosition.x << " - " << mousePosition.y << endl;
    }

    void update_p(int y) {
        int Y = start_height_table();
        text_p.setPosition(750,Y+y*BUCKET+y*SEPARATION+BUCKET/4);
    }

    void update_description(Operation operation, TK key, int p, int m0, int level) {
        string new_description;
        int first_b = m0*pow(2,level);
        int first_index = key % first_b;

        new_description = "Index: " + to_string(key) + " % " + to_string(first_b) +  " = "  + to_string(first_index);
        if (first_index < p) {
            int second_index = key % (first_b*2);
            new_description+=(", es menor a p!\nIndex recalculado: \n" + to_string(key) + "%" + to_string(first_b*2) + " = " + to_string(second_index) );
        }
        text_description.setString(new_description);
        hold_on(500);
    }

    void search_animation(int x, int key) {
        for (int i = 0; i < buckets.size(); i++) {
            buckets[x][i]->setTexture(*green_node_image);
            window.clear(Color::White);
            hold_on(800);
            buckets[x][i]->setTexture(*node_image);
            if (extras[x][i].getString() == to_string(key))
                break;
        }
    }

    void update_factor(float factor, Operation operation) {
        string factor_str = to_string(factor);
        string_comparison = string(1,factor_str[0]) + string(1,factor_str[1]) + string(1,factor_str[2]) + string(1,factor_str[3]);
        string temp = string_comparison;

        switch (operation) {
            case Insert:
                if (factor < 0.75) {
                    temp = temp + "\t<\t0.75";
                    string_space = "Hay espacio";
                } else {
                    temp = temp + "\t>=\t0.75";
                    string_space = "Split!";
                }
                break;
            case Delete:
                if (factor >= 0.3) {
                    temp = temp + "\t>=\t0.3";
                    string_space = "Sobra espacio";
                } else {
                    temp = temp + "\t<t0.3";
                    string_space = "Group!";
                }
                break;
            case Search:
                break;
        }

        text_space.setString(string_space);
        text_comparison.setString(temp);
        text_comparison.setFillColor(Color::Red);

        hold_on();
        indicator = true;

        hold_on();
        indicator = false;

        text_comparison.setFillColor(Color::Black);
        text_comparison.setString(string_comparison);

    }


    void draw() {

        window.draw(b_main);
        window.draw(b_dynamic);
        window.draw(b_find);
        window.draw(b_delete);
        window.draw(b_insert);

        window.draw(text_1);
        window.draw(text_2);
        window.draw(text_3);
        window.draw(text_4);
        window.draw(text_p);
        window.draw(text_dynamic);

        window.draw(text_upper);
        window.draw(text_factor);
        window.draw(text_lower);
        window.draw(text_comparison);
        window.draw(text_description);

        for_each(buckets.begin(), buckets.end(), [=](vector<Sprite*> x){
            for_each(x.begin(), x.end(), [=](Sprite* y){window.draw(*y);});
        });

        for_each(extras.begin(), extras.end(), [=](vector<Text> x){
            for_each(x.begin(), x.end(), [=](Text y){window.draw(y);});
        });

        if (indicator) window.draw(text_space);

        window.display();
    }


};



#endif //AED_MENU_H
