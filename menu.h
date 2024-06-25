
#ifndef AED_MENU_H
#define AED_MENU_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#define BUCKET 80
#define M 4;
using namespace sf;
using namespace std;

class Menu {
friend class Screen;
    RenderWindow &window;
    RectangleShape b_main, b_dynamic, b_insert, b_find, b_delete;
    vector<RectangleShape>  buckets;
    vector<RectangleShape> lines;
    Font font_1, font_2;
    Text text_1, text_2, text_3, text_4, text_dynamic, text_p;
    String string_dynamic = "";
    string string_1 = "Linear Hashing\n  Visualizer", string_2 = "Insertar", string_3 = "Eliminar"
            , string_4 = "Buscar", string_p = "p";

public:

    Menu(RenderWindow& window_) : window(window_) {

        set_fonts();
        set_backgrounds();
        set_texts();
        set_primary();

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
        font_1.loadFromFile("C:/utec/AED/fonts/font_1.ttf");
        font_2.loadFromFile("C:/utec/AED/fonts/font_2.ttf");
    }
    void set_primary() {

        for (int i=0, j=0; i<4; i++, j=j+100) {

            for (int k=0, l=0; k<3; k++, l=l+BUCKET) {

                RectangleShape r;
                r = RectangleShape(Vector2f(2,BUCKET));
                r.setFillColor(Color::Black);
                r.setPosition(800 + BUCKET + l, 200 + j);
                lines.push_back(r);
            }

            RectangleShape r;
            r = RectangleShape(Vector2f(BUCKET*4,BUCKET));
            r.setOutlineColor(Color::Black);
            r.setOutlineThickness(2);
            r.setPosition(800,200 + j);
            buckets.push_back(r);

        }

    }
    void draw() {
        window.draw(b_main);
        window.draw(b_dynamic);
        window.draw(b_find);
        window.draw(b_delete);
        window.draw(b_insert);

        for_each(buckets.begin(), buckets.end(), [=](RectangleShape x){window.draw(x);});
        for_each(lines.begin(), lines.end(), [=](RectangleShape x){window.draw(x);});


        window.draw(text_1);
        window.draw(text_2);
        window.draw(text_3);
        window.draw(text_4);
        window.draw(text_p);
        window.draw(text_dynamic);
        //window.display();
    }

    void add_key(int key) {
        int index = key % M;
        
    }

};

#endif //AED_MENU_H
