#include "A_Main.h"
#include <arm_neon.h>

#define N_max 256
#define r_2_max 5

#define OX_OX 800
#define OY_OY 500


// clang++ -std=c++17 -stdlib=libc++ -O3 -o app defolt_Mald.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -rpath /opt/homebrew/lib


int main()
{
    sf::RenderWindow window(sf::VideoMode({OX_OX, OY_OY}), "picturesque"); //создаем окно размером


    sf::Font font;
    (void)font.openFromFile("Garamond Classico SC Regular.ttf");
    
    char str[12] = {0};
    auto fps_txt = text_create({0, 0}, str , &font);


    sf::Texture maldebrota(sf::Vector2u(OX_OX, OY_OY));

    sf::Sprite malde(maldebrota);

    std::vector<std::uint8_t> pixels(OX_OX * OY_OY * 4);


    double offset_OX = -1.7;
    double offset_OY = -0.9;

    double zoom = 1;

    while(window.isOpen()) 
    {
        clock_t start = clock();

        while(auto event = window.pollEvent()) //достаем событие
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        auto mousePose = sf::Mouse::getPosition(window);  //Vector2i


        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))        // <|y
            offset_OY = offset_OY - 0.1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))        // y|>
            offset_OY = offset_OY + 0.1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))        // x->
            offset_OX = offset_OX + 0.1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))        // <-x
            offset_OX = offset_OX - 0.1;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))        // go smaller
        {
            double old_OX_mouse_in_mald = (double)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            double old_OY_mouse_in_mald = (double)mousePose.y / OY_OY * 2 * zoom + offset_OY; 

            zoom = zoom * 1.03;

            double new_OX_mouse_in_mald = (double)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            double new_OY_mouse_in_mald = (double)mousePose.y / OY_OY * 2 * zoom + offset_OY;

            offset_OX = offset_OX + old_OX_mouse_in_mald - new_OX_mouse_in_mald;
            offset_OY = offset_OY + old_OY_mouse_in_mald - new_OY_mouse_in_mald;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))        // go bigger
        {
            double old_OX_mouse_in_mald = (double)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            double old_OY_mouse_in_mald = (double)mousePose.y / OY_OY * 2 * zoom + offset_OY; 

            zoom = zoom * 0.97;

            double new_OX_mouse_in_mald = (double)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            double new_OY_mouse_in_mald = (double)mousePose.y / OY_OY * 2 * zoom + offset_OY;

            offset_OX = offset_OX + old_OX_mouse_in_mald - new_OX_mouse_in_mald;
            offset_OY = offset_OY + old_OY_mouse_in_mald - new_OY_mouse_in_mald;
        }


        for(int y_0 = 0; y_0 < OY_OY; y_0++)
        {
                
            for(int x_0 = 0; x_0 < OX_OX; x_0++)
            {
                int N_of = 0;

                double x = (double)x_0 / OX_OX * 2.2 * zoom + offset_OX; 
                double y = (double)y_0 / OY_OY * 1.74 * zoom + offset_OY;

                double x_now = x;
                double y_now = y;

                for( ; N_of < N_max; N_of++)
                {
                    double x_2 = x_now * x_now;
                    double y_2 = y_now * y_now;
                    double x_y = x_now * y_now;

                    double r_2 = x_2 + y_2;

                    if(r_2 > r_2_max)
                        break;

                    x_now = x_2 - y_2 + x;
                    y_now = x_y + x_y + y;
                }
                    
                int now_i = (y_0 * OX_OX + x_0) * 4;

                pixels[now_i + 0] = N_of * 8;          // r
                pixels[now_i + 1] = N_of * 2;          // g
                pixels[now_i + 2] = sqrt(N_of);        // b
                pixels[now_i + 3] = 255;               // a
            }
        }
        
        clock_t end = clock();
        
        maldebrota.update((const uint8_t*)(pixels.data()));

        int fps = CLOCKS_PER_SEC / (end - start);
        snprintf(str, sizeof(str), "fps = %d", fps);

        fps_txt.setString(str);
        

        window.clear();
        window.draw(malde);
        window.draw(fps_txt);
        window.display();
    }

    return 0;
}


sf::Text text_create(sf::Vector2f pose, const char* yes_noo, sf::Font *font)
{
    sf::Text text(*font, yes_noo, 22);
    text.setFillColor(sf::Color::White);
    text.setPosition(pose);

    return text;
}
