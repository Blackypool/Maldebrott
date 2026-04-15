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

// app_no_opt_noO3              g++ defolt_Mald.cpp -o app_no_opt_noO3
// app_no_opt_with_O3 -O3       g++ defolt_Mald.cpp -o app_no_opt_with_O3 -O3

// hyperfine --runs 100 './app_no_opt_with_O3' './app_no_opt_noO3'

// hyperfine --warmup 20 --runs 100 './app_no_opt_noO3' './app_no_opt_with_O3'

//clang++ -O3 -std=c++17 -arch arm64 -mcpu=apple-m2 -march=armv8.5-a defolt_Mald.cpp -o app_no_opt_with_O3
//clang++ -std=c++17 -arch arm64 -mcpu=apple-m2 -march=armv8.5-a defolt_Mald.cpp -o app_no_opt_noO3



// hyperfine './app_no_opt_noO3' './app_no_opt_with_O3'

// clang++ -std=c++17 -stdlib=libc++ -o app_no_opt_noO3 defolt_Mald.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -rpath /opt/homebrew/lib


int main()
{
    double offset_OX = -1.7;
    double offset_OY = -0.9;

    double zoom = 1;

    int N_of = 0;
    volatile int no_skip_O3 = 0;

        for(int y_0 = 0; y_0 < OY_OY; y_0++)
        {
                
            for(int x_0 = 0; x_0 < OX_OX; x_0++)
            {
                N_of = 0;

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

                no_skip_O3 = no_skip_O3 + N_of;
            }
        }

    return 0;
}
