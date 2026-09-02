#pragma once
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#define WIDTH 100 // 1280 old res
#define HEIGHT 50 // 720 old res

#define CENTERX 50 //width /2
#define CENTERY 25 //height /2
#define uint32 uint32_t

//color
#define BLANK '.'
#define FILLED '#'
const uint32 framebuffersize = WIDTH * HEIGHT;
inline char framebuffer[framebuffersize];
inline char backbuffer[framebuffersize];
inline float depthbuffer[framebuffersize];

inline void Clearbuffers()
{
    for (int i{}; i < framebuffersize; ++i)
    {
        backbuffer[i] = '.';
        depthbuffer[i] = 1000000.0f;
    }
}
inline void PutPixel(uint32 x, uint32 y,char type, uint32 bufferWidth, char(&buffer)[framebuffersize])
{
    buffer[y * bufferWidth + x] = type;
}

inline void Printframebuffer()
{
    system("clear");
    for (int y{}; y < HEIGHT; ++y)
    {
        for (int x{}; x < WIDTH; ++x)
        {
            printf("%c", framebuffer[y * WIDTH + x]);
        }

        printf("\n");
    }
}
inline void FillRect(uint32 x, uint32 y, uint32 width, uint32 height, char type, uint32 bufferWidth, char(&buffer)[framebuffersize])
{
    char* where = buffer + y * bufferWidth + x;
    for (uint32 y {}; y < height; ++y)
    {
        for (uint32 x{}; x < width; ++x)
        {
            where[x] = type;
        }
        where += bufferWidth;
    }
}