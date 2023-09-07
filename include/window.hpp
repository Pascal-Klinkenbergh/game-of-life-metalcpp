//
//  window.hpp
//  GameOfLifeMetal
//
//  Created by Pascal Klinkenbergh on 06.09.23.
//

#pragma once

#include <SFML/Graphics.hpp>

#include "model.hpp"
#include "observer.hpp"

class Window : public Observer {
public:
    Window(Model &model, int pixelSize)
        : model(model),
          pixelSize(pixelSize),
          win(sf::VideoMode(model.getWidth() * pixelSize, model.getHeight() * pixelSize),
              "Game Of Life -- stopped",
              sf::Style::Titlebar | sf::Style::Close) {
        // set fps limit
        win.setFramerateLimit(60);

        // drawing setup
        image.create(model.getWidth(), model.getHeight());
        texture.create(model.getWidth(), model.getHeight());
        sprite.setTexture(texture);
        sprite.setScale(pixelSize, pixelSize);

        // update image first
        update();
    }

    bool isOpen() {
        return win.isOpen();
    }

    bool pollEvent(sf::Event &event) {
        return win.pollEvent(event);
    }

    void close() {
        win.close();
    }

    void update() {
        const int width = model.getWidth();
        const int height = model.getHeight();

        // update image data
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                image.setPixel(x, y, model.getData(x, y) ? sf::Color::Black : sf::Color::White);
            }
        }

        texture.update(image);
    }

    void setTitle(std::string str) {
        win.setTitle(str);
    }

    void render() {
        win.draw(sprite);
        win.display();
    }

    int getPixelSize() {
        return pixelSize;
    }

private:
    Model &model;
    const int pixelSize;

    sf::RenderWindow win;
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
};
