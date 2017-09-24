#include <assert.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>
#include <iomanip>
#include <cmath>

using namespace std;
typedef std::complex<double> comp;
comp mandel(comp z, comp c);
unsigned int isBlowingUp(comp z, comp c);
double scale(double number, double from_min, double from_max, double to_min, double to_max);
double max_x = 2;
int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML works!");

    unsigned int size_x = 1000, size_y = 800;
    sf::Image img;
    img.create(size_x, size_y, sf::Color::Blue);
    sf::Texture texture;
    sf::Sprite sprite;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::KeyReleased:
                if(event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                break;
            // close the window with controller
            /*case sf::Event::JoystickButtonReleased:
                if(event.joystickButton.button == 1){
                    window.close();
                }using namespace std::literals::complex_literals
                break;*/
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        //comp zero(0, 0);
        //comp position(mousePos.x / 1000, 0);
        //unsigned int iteration_count = isBlowingUp(zero, position);
        window.clear();
        for(unsigned int x = 0; x < size_x; x++) {
            for(unsigned int y = 0; y < size_y; y++) {
                //img.setPixel(x, y, sf::Color(std::abs(std::sin(double(x + y)/double(size_x + size_y) * 3.14)) * 255, 0, 0));
                comp zero(0, 0);
                comp pos(scale(x, 0, size_x, -2, max_x), scale(y, 0, size_y, -2, 2));
                img.setPixel(x, y, sf::Color(isBlowingUp(zero, pos)*100, 0, 0));
            }
        }
        texture.loadFromImage(img);
        sprite.setTexture(texture);
        window.draw(sprite);
        window.display();
        //max_x -= 0.1;
    }

    return 0;
}


comp mandel(comp z, comp c)
{
    return pow(z, 2) + c;
}

unsigned int isBlowingUp(comp z, comp c)
{
    //cout << "starting with z = " <<  z << endl << "and c = " << c << endl;
    unsigned int max_interation = 40, curr_iteration = 0;
    while(abs(z) < 2.0 && curr_iteration <= max_interation) {
        z = mandel(z, c);
        //cout << z << endl;
        curr_iteration++;
    }
    return curr_iteration;
}

double scale(double number, double from_min, double from_max, double to_min, double to_max){
    assert(from_min < from_max);
    assert(to_min < to_max);
    assert(number >= from_min);
    assert(number <= from_max);
    return ((number - from_min) * (to_max - to_min)) /
                (from_max - from_min)                           + to_min;
}
