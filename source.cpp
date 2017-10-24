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
unsigned int max_iteration = 500;

typedef struct {
    double r = 0;       // a fraction between 0 and 1
    double g = 0;       // a fraction between 0 and 1
    double b = 0;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h = 0;       // angle in degrees
    double s = 1;       // a fraction between 0 and 1
    double v = 0.5;       // a fraction between 0 and 1
} hsv;

rgb hsv2rgb(hsv in);

int main()
{
    unsigned int size_x = 800, size_y = 800;
    sf::RenderWindow window(sf::VideoMode(size_x, size_y), "SFML works!");

    sf::Image img;
    img.create(size_x, size_y, sf::Color::Blue);
    sf::Texture texture;
    sf::Sprite sprite;

    window.clear();
    for(unsigned int x = 0; x < size_x; x++) {
        for(unsigned int y = 0; y < size_y; y++) {
            //img.setPixel(x, y, sf::Color(std::abs(std::sin(double(x + y)/double(size_x + size_y) * 3.14)) * 255, 0, 0));
            comp zero(0, 0);
            comp pos(scale(x, 0, size_x, -2, max_x), scale(y, 0, size_y, -2, 2));
            unsigned int result = isBlowingUp(zero, pos);
            rgb colorRGB;
            if(result != max_iteration) {
                hsv colorHSV;
                // set hue equal to the number of iterations / total number of iterations
                colorHSV.h =  int(double(result) / double(max_iteration) * 359 - 120 + 360) % 360;
                colorRGB = hsv2rgb(colorHSV);
            }
            img.setPixel(x, y, sf::Color(colorRGB.r * 255, colorRGB.g * 255, colorRGB.b * 255));
        }
    }
    texture.loadFromImage(img);
    sprite.setTexture(texture);
    window.draw(sprite);
    window.display();

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

        //sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
        //comp zero(0, 0);
        //comp position(mousePos.x / 1000, 0);
        //unsigned int iteration_count = isBlowingUp(zero, position);

        //max_x -= 0.1;
        //max_iteration++;
        //cout << max_iteration << endl;
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
    unsigned int curr_iteration = 0;
    while(abs(z) < 2.0 && curr_iteration < max_iteration) {
        z = mandel(z, c);
        //cout << z << endl;
        curr_iteration++;
    }
    return curr_iteration;
}

double scale(double number, double from_min, double from_max, double to_min, double to_max)
{
    assert(from_min < from_max);
    assert(to_min < to_max);
    assert(number >= from_min);
    assert(number <= from_max);
    return ((number - from_min) * (to_max - to_min)) /
           (from_max - from_min)                           + to_min;
}



rgb hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;
}
