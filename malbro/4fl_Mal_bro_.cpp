#include "A_Main.h"
#include <arm_neon.h>

#define N_max 256
#define r_2_max 5

#define OX_OX 800
#define OY_OY 500

#define ARM_int 4


// clang++ -std=c++17 -march=armv8.5-a -mcpu=apple-m2 -stdlib=libc++ -O3 -o app 4fl_Mal_bro_.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -rpath /opt/homebrew/lib


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


    float offset_OX = -1.7;
    float offset_OY = -0.9;

    float zoom = 1;

    while(window.isOpen()) 
    {
        clock_t start = clock();

        while(auto event = window.pollEvent()) //достаем событие
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        ////// keywords
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
            float old_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            float old_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2 * zoom + offset_OY; 

            zoom = zoom * 1.03;

            float new_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            float new_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2 * zoom + offset_OY;

            offset_OX = offset_OX + old_OX_mouse_in_mald - new_OX_mouse_in_mald;
            offset_OY = offset_OY + old_OY_mouse_in_mald - new_OY_mouse_in_mald;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))        // go bigger
        {
            float old_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            float old_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2 * zoom + offset_OY; 

            zoom = zoom * 0.97;

            float new_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2 * zoom + offset_OX; 
            float new_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2 * zoom + offset_OY;

            offset_OX = offset_OX + old_OX_mouse_in_mald - new_OX_mouse_in_mald;
            offset_OY = offset_OY + old_OY_mouse_in_mald - new_OY_mouse_in_mald;
        }
        //////

        for(int y_0 = 0; y_0 < OY_OY; y_0 = y_0 + 1)                // up fps if to much delta(y)
        {
            for(int x_0 = 0; x_0 < OX_OX; x_0 = x_0 + ARM_int)
            {
                /////. x
                float x_vec[ARM_int] = {0};
                
                for(int i = 0; i < ARM_int; ++i)
                    x_vec[i] = (float)(x_0 + i) / OX_OX * 2.20 * zoom + offset_OX;

                float x_now_vec[ARM_int] = {0};
                memcpy(x_now_vec, x_vec, sizeof(x_vec));
                /////
                
                /////. y
                float y_vec[ARM_int] = {0};

                for(int i = 0; i < ARM_int; ++i)
                    y_vec[i] = (float)(y_0) / OY_OY * 1.74 * zoom + offset_OY;

                float y_now_vec[ARM_int] = {0};
                memcpy(y_now_vec, y_vec, sizeof(y_vec));
                /////

                float x_2_vec[ARM_int] = {0};   // x2
                float y_2_vec[ARM_int] = {0};   // y2

                float x_y_vec[ARM_int] = {0};   // xy
                float r_2_vec[ARM_int] = {0};   // r2

                int Clr_vec[ARM_int] = {0};               // color == N_of
                int inc_vec[ARM_int] = {1, 1, 1, 1};      // for ++

                int count_N_of = 0;
                int how_much_iter = 0;

                while(count_N_of < ARM_int && how_much_iter < N_max)
                {
                    for(int i = 0; i < ARM_int; i++)                // x2
                        x_2_vec[i] = x_now_vec[i] * x_now_vec[i];

                    for(int i = 0; i < ARM_int; i++)                // y2
                        y_2_vec[i] = y_now_vec[i] * y_now_vec[i];

                    for(int i = 0; i < ARM_int; i++)                // xy
                        x_y_vec[i] = x_now_vec[i] * y_now_vec[i];

                    for(int i = 0; i < ARM_int; i++)                // r2
                        r_2_vec[i] = x_2_vec[i] + y_2_vec[i];

                    for(int i = 0; i < ARM_int; i++)                // check r2 > r_MAX?
                        if(r_2_vec[i] > r_2_max)
                            inc_vec[i] = 0;                         // > => stop up N_of

                    /////
                    for(int i = 0; i < ARM_int; i++)                // new X
                        x_now_vec[i] = x_2_vec[i] - y_2_vec[i] + x_vec[i];

                    for(int i = 0; i < ARM_int; i++)                // new Y
                        y_now_vec[i] = x_y_vec[i] + x_y_vec[i] + y_vec[i];
                    /////


                    for(int i = 0; i < ARM_int; i++)                // N_of++
                        Clr_vec[i] = Clr_vec[i] + inc_vec[i];
                    

                    count_N_of = 0;
                    for(int i = 0; i < ARM_int; i++)                // count for while
                        if(inc_vec[i] == 0)
                            ++count_N_of;

                    how_much_iter++;
                }

                // do color_es
                int now_i = (y_0 * OX_OX + x_0) * 4;
                for(int i = 0; i < ARM_int; i++)
                {
                    pixels[now_i + 0 + i*4] = Clr_vec[i] * 8;          // r
                    pixels[now_i + 1 + i*4] = Clr_vec[i] * 2;          // g
                    pixels[now_i + 2 + i*4] = sqrt(Clr_vec[i]);        // b
                    pixels[now_i + 3 + i*4] = 255;                     // a
                }
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
