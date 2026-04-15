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

#include <arm_neon.h>

#define N_max 256
#define r_2_max 5

#define OX_OX 800.0f
#define OY_OY 500.0f

#define ARM_int 4

// g++ Intrin_ALBRO.cpp -o app_intrin_noO3
// g++ Intrin_ALBRO.cpp -o app_intrin_withO3 -O3

// clang++ -std=c++17 -stdlib=libc++ -O3 -o app Intrin_ALBRO.cpp -I/opt/homebrew/include -L/opt/homebrew/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -rpath /opt/homebrew/lib

// hyperfine --warmup 20 --runs 100 './app_intrin_noO3' './app_intrin_withO3'

// clang++ -std=c++17 -arch arm64 -mcpu=apple-m2 -march=armv8.5-a Intrin_ALBRO.cpp -o app_intrin_noO3
// clang++ -O3 -std=c++17 -arch arm64 -mcpu=apple-m2 -march=armv8.5-a Intrin_ALBRO.cpp -o app_intrin_withO3

int main()
{
    float d_x = 2.20f / OX_OX;
    float d_y = 1.74f / OY_OY;

    float offset_OX = -1.7f;
    float offset_OY = -0.9f;

    float zoom = 1;

    volatile unsigned int no_skip_O3 = 0;

    float32x4_t r_2_max_for_cmp = vdupq_n_f32(r_2_max);

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
                unsigned int colores[ARM_int] = {0};
                vst1q_u32(colores, Clr_vec);    // v store = save

                no_skip_O3 = no_skip_O3 + colores[0];
            }
        }

    return 0;
}
