#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <getopt.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>
#include <time.h>
#include <vector>
#include <cstdint>
#include <optional>
#include <iostream>


#define N_max 256
#define r_2_max 5

#define OX_OX 800
#define OY_OY 500

#define ARM_int 4

// g++ 4fl_Mal_bro_.cpp -o app_vect_noO3
// g++ 4fl_Mal_bro_.cpp -o app_vect_withO3  -O3


// -O3 -std=c++17 -march=armv8.5-a -mcpu=apple-m2
// clang++ -O3 -std=c++17 -arch arm64 -mcpu=apple-m2 -march=armv8.5-a 4fl_Mal_bro_.cpp -o app_vect_withO3
// clang++ -std=c++17 -arch arm64 -mcpu=apple-m2 -march=armv8.5-a 4fl_Mal_bro_.cpp -o app_vect_noO3


// hyperfine --warmup 20 --runs 100 './app_vect_noO3' './app_vect_withO3'


// clang++ -O3 -std=c++17 -march=armv8.5-a -mcpu=apple-m2 -stdlib=libc++ -o app Mal_bro_4fl.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -rpath /opt/homebrew/lib

int main()
{
    float offset_OX = -1.7f;
    float offset_OY = -0.9f;

    float zoom = 1.f;

    volatile int no_skip_O3 = 0;

        for(int y_0 = 0; y_0 < OY_OY; y_0 = y_0 + 1)                // up fps if to much delta(y)
        {
            for(int x_0 = 0; x_0 < OX_OX; x_0 = x_0 + ARM_int)
            {
                /////. x
                float x_vec[ARM_int] = {0};
                
                for(int i = 0; i < ARM_int; ++i)
                    x_vec[i] = (float)(x_0 + i) / OX_OX * 2.2f * zoom + offset_OX;

                float x_now_vec[ARM_int] = {0};
                memcpy(x_now_vec, x_vec, sizeof(x_vec));
                /////
                
                /////. y
                float y_vec[ARM_int] = {0};

                for(int i = 0; i < ARM_int; ++i)
                    y_vec[i] = (float)(y_0) / OY_OY * 1.74f * zoom + offset_OY;

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

                no_skip_O3 = no_skip_O3 + Clr_vec[0];
            }
        }
        
    return 0;
}
