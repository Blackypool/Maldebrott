#ifndef MALDEBROTA_M_H
#define MALDEBROTA_M_H


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

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdint>
#include <optional>
#include <iostream>


sf::Text text_create(sf::Vector2f pose, const char* yes_noo, sf::Font *font);


#define AsserT(what_need, retern)  \
            if(what_need){  \
                fprintf(stderr, "\nerror in line, %s:%d\n", __FILE__, __LINE__);  \
                return retern;  \
            }


#endif