#include <iostream>

// only define those once!
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include "model.hpp"
#include "window.hpp"

int counter = 0;  // count frames to update model
bool isrunning = false;

int main(int argc, const char* argv[]) {
    Model model(100, 75);
    Window window(model, 10);

    model.addObserver(&window);

    while (window.isOpen()) {
        // handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            // close window button
            if (event.type == sf::Event::EventType::Closed)
                window.close();

            // keyboard press
            if (event.type == sf::Event::EventType::KeyPressed) {
                switch (event.key.code) {
                    case sf::Keyboard::Q:
                        window.close();
                        break;

                    case sf::Keyboard::Space:
                        isrunning = !isrunning;
                        if (isrunning)
                            window.setTitle("Game Of Life -- running");
                        else
                            window.setTitle("Game Of Life -- stopped");
                        break;

                    default:
                        break;
                }
            }

            // left click
            if (event.type == sf::Event::EventType::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                int x = event.mouseButton.x / window.getPixelSize();
                int y = event.mouseButton.y / window.getPixelSize();
                model.setData(x, y, !model.getData(x, y));
            }
        }

        counter++;
        if (counter >= 10 && isrunning) {
            model.update();
            counter = 0;
        }

        window.render();
    }

    return 0;
}
