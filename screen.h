#ifndef AED_SCREEN_H
#define AED_SCREEN_H

#define HEIGHT 800
#define WIDTH 1400
#include "menu.h"
#include <iostream>
#include "Linear_Hash.h"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Screen {

    RenderWindow window;
    Texture *image;
    Sprite *square;
    Menu<> *menu;
    Linear_Hashing<>* table;


public:
    Screen() {
        window.create(VideoMode(WIDTH, HEIGHT), "Linear Hashing");

        menu = new Menu (window);
        table = new Linear_Hashing<>(*menu);

    }
    void execute() {

        while (window.isOpen())
        {

            window.clear(Color::White);

            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();
                if (event.type == Event::TextEntered) {
                    if (event.text.unicode>=48 && event.text.unicode <= 57 || event.text.unicode==8) {
                        if (event.text.unicode == 8 && menu->string_dynamic.getSize()>0)
                            menu->string_dynamic.erase(menu->string_dynamic.getSize()-1, 1);
                        else
                            menu->string_dynamic += static_cast<char>(event.text.unicode);

                        menu->text_dynamic.setString(menu->string_dynamic);
                    }

                    if (event.text.unicode == 13 && menu->string_dynamic.getSize()>0) {
                        string key_str = menu->text_dynamic.getString();
                        int key = stoi(key_str);
                        table->insert(key);
                        menu->text_dynamic.setString("");
                        menu->string_dynamic = "";
                    }
                }
            }



/*

*/
            menu->draw();

 //           window.draw(*square);
            window.display();

        }

    }

};

#endif //AED_SCREEN_H
