#include "A_Main.h"
#include <arm_neon.h>

#define N_max 256
#define r_2_max 5

#define OX_OX 800
#define OY_OY 500

#define ARM_int 4


// clang++ -std=c++17 -stdlib=libc++ -march=armv8.5-a -mcpu=apple-m2 -O3 -o app Intrin_ALBRO.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -rpath /opt/homebrew/lib


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

    float d_x = 2.20f / OX_OX;
    float d_y = 1.74f / OY_OY;

    float offset_OX = -1.7f;
    float offset_OY = -0.9f;

    float zoom = 1;

    float32x4_t r_2_max_for_cmp = vdupq_n_f32(r_2_max);

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
            offset_OY = offset_OY - 0.1f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))        // y|>
            offset_OY = offset_OY + 0.1f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))        // x->
            offset_OX = offset_OX + 0.1f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))        // <-x
            offset_OX = offset_OX - 0.1f;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))        // go smaller
        {
            float old_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2.f * zoom + offset_OX; 
            float old_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2.f * zoom + offset_OY; 

            zoom = zoom * 1.03;

            float new_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2.f * zoom + offset_OX; 
            float new_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2.f * zoom + offset_OY;

            offset_OX = offset_OX + old_OX_mouse_in_mald - new_OX_mouse_in_mald;
            offset_OY = offset_OY + old_OY_mouse_in_mald - new_OY_mouse_in_mald;

            d_x = d_x * zoom;
            d_y = d_y * zoom;
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))        // go bigger
        {
            float old_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2.f * zoom + offset_OX; 
            float old_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2.f * zoom + offset_OY; 

            zoom = zoom * 0.97;

            float new_OX_mouse_in_mald = (float)mousePose.x / OX_OX * 2.f * zoom + offset_OX; 
            float new_OY_mouse_in_mald = (float)mousePose.y / OY_OY * 2.f * zoom + offset_OY;

            offset_OX = offset_OX + old_OX_mouse_in_mald - new_OX_mouse_in_mald;
            offset_OY = offset_OY + old_OY_mouse_in_mald - new_OY_mouse_in_mald;

            d_x = d_x * zoom;
            d_y = d_y * zoom;
        }
        //////

        for(int y_0 = 0; y_0 < OY_OY; y_0 = y_0 + 1)                // up fps if to much delta(y)
        {
            for(int x_0 = 0; x_0 < OX_OX; x_0 = x_0 + ARM_int)
            {
                /////. x
                float param_for_x = (float)x_0*d_x + offset_OX;
                float32x4_t x_vec = {param_for_x, param_for_x + d_x, param_for_x + 2*d_x, param_for_x + 3*d_x};

                float32x4_t x_now_vec = x_vec;
            
                /////. y
                float param_for_y = (float)y_0*d_y + offset_OY;
                float32x4_t y_vec = {param_for_y, param_for_y, param_for_y, param_for_y};

                float32x4_t y_now_vec = y_vec;
                /////

                float32x4_t x_2_vec = vdupq_n_f32(0);   // x2
                float32x4_t y_2_vec = vdupq_n_f32(0);   // y2

                float32x4_t x_y_vec = vdupq_n_f32(0);   // xy
                float32x4_t r_2_vec = vdupq_n_f32(0);   // r2

                uint32x4_t Clr_vec = vdupq_n_u32(0);     // color == N_of
                uint32x4_t inc_vec = vdupq_n_u32(1);     // for ++

                int how_much_iter = 0;

                      // all in one number
                while(vaddvq_u32(inc_vec) > 0 && how_much_iter < N_max)
                {
                    x_2_vec = vmulxq_f32(x_now_vec, x_now_vec);     // x2

                    y_2_vec = vmulxq_f32(y_now_vec, y_now_vec);     // y2

                    x_y_vec = vmulxq_f32(x_now_vec, y_now_vec);     // xy

                    r_2_vec = vaddq_f32(x_2_vec, y_2_vec);          // r2

                    /////   cmp r2, r_MAX N_max_for_cmp
                    uint32x4_t mask = vcleq_f32(r_2_vec, r_2_max_for_cmp);    // cashback 1,1,0,1 => i=3 is out
                    inc_vec = vandq_u32(inc_vec, mask);                       // inc & mask
                    /////

                    x_now_vec = vsubq_f32(vaddq_f32(x_2_vec, x_vec), y_2_vec);  // new X

                    y_now_vec = vaddq_f32(y_vec , vaddq_f32(x_y_vec, x_y_vec)); // new Y

                    /////

                    Clr_vec = vaddq_u32(Clr_vec, inc_vec);          // N_of++

                    how_much_iter++;
                }

                // do color_es
                int now_i = (y_0 * OX_OX + x_0) * 4;
                
                unsigned int colores[ARM_int] = {0};
                vst1q_u32(colores, Clr_vec);    // v store = save

                for(int i = 0; i < ARM_int; i++)
                {
                    pixels[now_i + 0 + i*4] = colores[i] * 8;          // r
                    pixels[now_i + 1 + i*4] = colores[i] * 2;          // g
                    pixels[now_i + 2 + i*4] = sqrt(colores[i]);        // b
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
