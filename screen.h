#ifndef AED_SCREEN_H
#define AED_SCREEN_H

#include "menu.h"
#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

class Screen {

    RenderWindow window;
    Texture *image;
    Sprite *square;
    Menu *menu;

public:
    Screen() {
        window.create(sf::VideoMode(1400, 800), "My window");
        square = new Sprite();
        image = new Texture();
        menu = new Menu (window);

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
                        menu->add_key(key);
                    }
                }
            }



/*
            square->setPosition(500,500);
            image->loadFromFile("C:/utec/AED/first.png");
            square->setTexture(*image);
*/
            menu->draw();

 //           window.draw(*square);
            window.display();

        }

    }

};

#endif //AED_SCREEN_H
