#pragma once
#include "framebuffer.hpp"
#include "math.hpp"
#include <cstdlib>
namespace Rasterizer 
{
    inline void swapbuffers()
    {
        for (int i {}; i < framebuffersize; ++i)
        {
            framebuffer[i] = backbuffer[i];
        }
    }

    inline void Drawline(Vec3 point1,Vec3 point2)
    {
        if (point1.vec3[2] < 0 || point2.vec3[2] < 0) return;
        if (point1.vec3[0] > point2.vec3[0]) Math::swap(point1, point2);

        const int SHIFT = 16;
        const int HALF = (1 << (SHIFT - 1));
        Vec3 dir = point2 - point1;

        //truncate
        int dx = (int)dir.vec3[0];
        int dy = (int)dir.vec3[1];
        Vec3 point = point1;
        int x1 = ((int)point1.vec3[0] << SHIFT) + HALF;
        int y1 = ((int)point1.vec3[1] << SHIFT) + HALF;

        int steps = std::abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        int pixelX{};
        int pixelY{};
        int pixelIndex{};
        if (steps == 0)
        {
            pixelX = (int)point1.vec3[0];
            pixelY = (int)point1.vec3[1];
            pixelIndex = pixelY * WIDTH + pixelX;
            if (pixelX >= 0 && pixelX < WIDTH && pixelY >= 0 && pixelY < HEIGHT)
            {
                //depth test
                if(point1.vec3[2] < depthbuffer[pixelIndex])
                {
                    depthbuffer[pixelIndex] = point1.vec3[2];
                    PutPixel((int)point1.vec3[0], (int)point1.vec3[1], '#', WIDTH, backbuffer);
                }
            }
            return;
        }
        int Xincrement = (int)(((long long)dx << SHIFT) / steps);
        int Yincrement = (int)(((long long)dy << SHIFT) / steps);

        float zStep = (point2.vec3[2] - point1.vec3[2]) / (float)steps;
        float currentZ = point1.vec3[2];
        for (int i{};i <= steps; ++i)
        {
            pixelX = x1 >> SHIFT;
            pixelY = y1 >> SHIFT;
            pixelIndex = pixelY * WIDTH + pixelX;

            if (pixelX >= 0 && pixelX < WIDTH && pixelY >= 0 && pixelY < HEIGHT)
            {
                pixelIndex = pixelY * WIDTH + pixelX;

                //write depth
                if (currentZ < depthbuffer[pixelIndex])
                {
                    depthbuffer[pixelIndex] = currentZ;
                    PutPixel(pixelX, pixelY, '#', WIDTH, backbuffer);
                }
            }
            x1 += Xincrement;
            y1 += Yincrement;
            currentZ += zStep;
        }
    }
    inline void DrawTriangle(Vec3 vertex1, Vec3 vertex2, Vec3 vertex3)
    {
        if (vertex1.vec3[1] > vertex2.vec3[1]) Math::swap(vertex1, vertex2);
        if (vertex1.vec3[1] > vertex3.vec3[1]) Math::swap(vertex1, vertex3);
        if (vertex2.vec3[1] > vertex3.vec3[1]) Math::swap(vertex2, vertex3);

        int totalHeight = vertex3.vec3[1] - vertex1.vec3[1];
        if (totalHeight == 0)return; //prevent divsion by 0
        Vec3 point1, point2;
        int segmentHeight{};
        if (vertex1.vec3[1] != vertex2.vec3[1])
        {
            segmentHeight = vertex2.vec3[1] - vertex1.vec3[1];
            for(int y = vertex1.vec3[1]; y<=vertex2.vec3[1]; ++y)
            {
                point1.vec3[0] = vertex1.vec3[0] + ((vertex3.vec3[0] - vertex1.vec3[0]) * (y - vertex1.vec3[1])) / totalHeight;
                point2.vec3[0] = vertex1.vec3[0] + ((vertex2.vec3[0] - vertex1.vec3[0]) * (y - vertex1.vec3[1])) / segmentHeight;
                point1.vec3[1] = y;
                point2.vec3[1] = y;
                point1.vec3[2] = vertex1.vec3[2] + ((vertex3.vec3[2] - vertex1.vec3[2]) * (y - vertex1.vec3[1])) / totalHeight;
                point2.vec3[2] = vertex1.vec3[2] + ((vertex2.vec3[2] - vertex1.vec3[2]) * (y - vertex1.vec3[1])) / segmentHeight;
                Drawline(point1, point2);
            }
        }
        if (vertex2.vec3[1] != vertex3.vec3[1])
        {
            segmentHeight = vertex3.vec3[1] - vertex2.vec3[1];
            for(int y = vertex2.vec3[1]; y<=(int)vertex3.vec3[1]; ++y)
            {
                int x1 = vertex1.vec3[0] + ((vertex3.vec3[0] - vertex1.vec3[0]) * (y - vertex1.vec3[1])) / totalHeight;
                float s = ((float)y + 0.5f - vertex2.vec3[1]) / segmentHeight;
                if (s < 0) s = 0; else if (s > 1) s = 1;
                int x2 = (int)(vertex2.vec3[0] + (vertex3.vec3[0] - vertex2.vec3[0]) * s);
                //int x2 = vertex2.vec3[0] + ((vertex3.vec3[0] - vertex2.vec3[0]) * (y - vertex2.vec3[1])) / segmentHeight;
                point1.vec3[0] = x1;
                point2.vec3[0] = x2;
                point1.vec3[1] = y;
                point2.vec3[1] = y;
                point1.vec3[2] = vertex1.vec3[2] + ((vertex3.vec3[2] - vertex1.vec3[2]) * (y - vertex1.vec3[1])) / totalHeight;
                point2.vec3[2] = vertex2.vec3[2] + ((vertex3.vec3[2] - vertex2.vec3[2]) * (y - vertex2.vec3[1])) / segmentHeight;
                Drawline(point1, point2);
            }
        }
    }
}