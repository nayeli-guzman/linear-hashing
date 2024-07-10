
#ifndef AED_MENU_H
#define AED_MENU_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#define BUCKET 80
#define MAX 4
#define SEPARATION 20
using namespace sf;
using namespace std;

template<typename TK=int>
class Menu {

    friend class Screen;

    int M;
    RenderWindow &window;
    RectangleShape b_main, b_dynamic, b_insert, b_find, b_delete;
    Font font_1, font_2;
    Text text_1, text_2, text_3, text_4, text_dynamic, text_p;
    String string_dynamic = "";
    string string_1 = "Linear Hashing\n  Visualizer", string_2 = "Insertar", string_3 = "Eliminar"
            , string_4 = "Buscar", string_p = "p";
    vector<int> positions;
    vector<vector<Text>> extras;
    vector<vector<Sprite*>> buckets;
    Texture* node_image;

    Vector2f mousePosition;

public:

    Menu(RenderWindow& window_) : window(window_) {

        positions.resize(4,0);
        node_image = new Texture();
        node_image->loadFromFile("/home/andreacoa/Documents/cplusplus/linear-hashing/images/node.png");
        extras.resize(MAX);
        set_fonts();
        set_backgrounds();
        set_texts();
        set_table(MAX, false);

    }

    void set_backgrounds() {
        b_main = RectangleShape(Vector2f(500,800));
        b_main.setFillColor(Color(150,160,210,255));
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
        font_1.loadFromFile("/home/andreacoa/Documents/cplusplus/linear-hashing/fonts/font_1.ttf");
        font_2.loadFromFile("/home/andreacoa/Documents/cplusplus/linear-hashing/fonts/font_2.ttf");
    }
    void set_table(int _M, bool c, int p=0) {

        M = _M;

        if (c) {
            //extras.clear();

            positions.clear();
            positions.resize(M, 0);

            for(int i=0; i<extras.size(); i++) {
                if (i != p) {
                    for (int j=0; j<extras[i].size(); j++) update_key(i,j); // falta ver en que orden se insertan visualmente
                }
            }
            extras[p].clear();
            extras.resize(M);
            buckets.clear();
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
    void set_key(int x, TK key) {

        int Y = start_height_table() + x * BUCKET + x * SEPARATION + 10;

        Text _key;
        _key.setFont(font_2);
        _key.setString(to_string(key));
        _key.setPosition(820 + positions[x]++ * BUCKET, Y);
        _key.setCharacterSize(30);
        _key.setColor(Color::Black);
        extras[x].push_back(_key);

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

        for_each(buckets.begin(), buckets.end(), [=](vector<Sprite*> x){
            for_each(x.begin(), x.end(), [=](Sprite* y){window.draw(*y);});
        });

        for_each(extras.begin(), extras.end(), [=](vector<Text> x){
            for_each(x.begin(), x.end(), [=](Text y){window.draw(y);});
        });

        //window.display();
    }


};

#endif //AED_MENU_H
