#ifndef AED_SCREEN_H
#define AED_SCREEN_H

#define HEIGHT 800
#define WIDTH 1400
#include "menu.h"
#include "Linear_Hashing.h"

#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Screen {

    RenderWindow window;
    Menu<> *menu;
    Linear_Hashing<>* table;

    // singleton
    static Screen* instancia;
    Screen() {
        window.create(VideoMode(WIDTH, HEIGHT), "Linear Hashing");
        menu = new Menu (window);
        table = new Linear_Hashing<>(*menu);
    }

public:

    static Screen* getInstance() {
        if (instancia == nullptr)
            instancia = new Screen;
        return instancia;
    }

    void execute() {

        while (window.isOpen()) {

            window.clear(Color::White);

            Event event;

            while (window.pollEvent(event)) {
                if (event.type == Event::Closed)
                    window.close();

                if (event.type == Event::TextEntered) {

                    if (event.text.unicode>=48 && event.text.unicode <= 57 || event.text.unicode==8) {
                        if (event.text.unicode == 8 && menu->string_dynamic.getSize()>0)
                            menu->string_dynamic.erase(menu->string_dynamic.getSize()-1, 1);
                        else if (event.text.unicode != 8)
                            menu->string_dynamic += static_cast<char>(event.text.unicode);

                        menu->text_dynamic.setString(menu->string_dynamic);
                    }

                }

                if (event.type == Event::MouseButtonPressed && event.mouseButton.button== Mouse::Left) {
                    if (menu->b_insert.getGlobalBounds().contains(menu->mousePosition)
                            && menu->string_dynamic.getSize() > 0 ) {

                        string key_str = menu->text_dynamic.getString();
                        int key = stoi(key_str);
                        table->insert(key);
                        menu->text_dynamic.setString("");
                        menu->string_dynamic = "";

                    }

                    else if (menu->b_delete.getGlobalBounds().contains(menu->mousePosition)
                            && menu->string_dynamic.getSize()>0 ) {

                        string key_str = menu->text_dynamic.getString();
                        int key = stoi(key_str);
                        table->borrar(key);
                        menu->text_dynamic.setString("");
                        menu->string_dynamic = "";

                    }

                    else if (menu->b_find.getGlobalBounds().contains(menu->mousePosition)
                            && menu->string_dynamic.getSize() > 0 ) {
                        string key_str = menu->text_dynamic.getString();
                        int key = stoi(key_str);
                        table->find(key);
                        menu->text_dynamic.setString("");
                        menu->string_dynamic = "";

                    }
                }

            }

            menu->update_mouse_position();
            menu->draw();

 //           window.draw(*square);
            //window.display();

        }

    }

};

#endif //AED_SCREEN_H
