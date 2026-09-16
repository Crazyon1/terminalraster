#pragma once
#include "framebuffer.hpp"
#include "math.hpp"
#include <cstddef>
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
        if (point1.z < 0 || point2.z < 0) return;
        if (point1.x > point2.x) Math::swap(point1, point2);

        const int SHIFT = 16;
        const int HALF = (1 << (SHIFT - 1));
        Vec3 dir = point2 - point1;

        //truncate
        int dx = (int)dir.x;
        int dy = (int)dir.y;
        Vec3 point = point1;
        int x1 = ((int)point1.x << SHIFT) + HALF;
        int y1 = ((int)point1.y << SHIFT) + HALF;

        int steps = std::abs(dx) > abs(dy) ? abs(dx) : abs(dy);
        int pixelX{};
        int pixelY{};
        int pixelIndex{};
        if (steps == 0)
        {
            pixelX = (int)point1.x;
            pixelY = (int)point1.y;
            pixelIndex = pixelY * WIDTH + pixelX;
            if (pixelX >= 0 && pixelX < WIDTH && pixelY >= 0 && pixelY < HEIGHT)
            {
                //depth test
                if(point1.z < depthbuffer[pixelIndex])
                {
                    depthbuffer[pixelIndex] = point1.z;
                    PutPixel((int)point1.x, (int)point1.y, '#', WIDTH, backbuffer);
                }
            }
            return;
        }
        int Xincrement = (int)(((long long)dx << SHIFT) / steps);
        int Yincrement = (int)(((long long)dy << SHIFT) / steps);

        float zStep = (point2.z - point1.z) / (float)steps;
        float currentZ = point1.z;
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
        if (vertex1.y > vertex2.y) Math::swap(vertex1, vertex2);
        if (vertex1.y > vertex3.y) Math::swap(vertex1, vertex3);
        if (vertex2.y > vertex3.y) Math::swap(vertex2, vertex3);

        int totalHeight = vertex3.y- vertex1.y;
        if (totalHeight == 0)return; //prevent divsion by 0
        Vec3 point1, point2;
        int segmentHeight{};
        if (vertex1.y != vertex2.y)
        {
            segmentHeight = vertex2.y - vertex1.y;
            int step = (segmentHeight > 0) ? 1 : -1;
            for(int y = vertex1.y; y<vertex2.y; ++y)
            {
                point1.x = vertex1.x + ((vertex3.x - vertex1.x) * (y - vertex1.y)) / totalHeight;
                point2.x = vertex1.x + ((vertex2.x - vertex1.x) * (y - vertex1.y)) / segmentHeight;
                point1.y = y;
                point2.y = y;
                point1.z = vertex1.z + ((vertex3.z - vertex1.z) * (y - vertex1.y)) / totalHeight;
                point2.z = vertex1.z + ((vertex2.z - vertex1.z) * (y - vertex1.y)) / segmentHeight;
                Drawline(point1, point2);
            }
        }
        if (vertex2.y != vertex3.y)
        {
            segmentHeight = vertex3.y - vertex2.y;
            for(int y = vertex2.y; y<(int)vertex3.y; ++y)
            {
                int x1 = vertex1.x + ((vertex3.x - vertex1.x) * (y - vertex1.y)) / totalHeight;
                int x2 = vertex2.x + ((vertex3.x - vertex2.x) * (y - vertex2.y)) / segmentHeight;
                point1.x = x1;
                point2.x = x2;
                point1.y = y;
                point2.y = y;
                point1.z = vertex1.z + ((vertex3.z - vertex1.z) * (y - vertex1.y)) / totalHeight;
                point2.z = vertex2.z + ((vertex3.z - vertex2.z) * (y - vertex2.y)) / segmentHeight;
                Drawline(point1, point2);
            }
        }
    }


    inline void DrawTrianglesRect(Vec3* a, size_t size, Mat4& mat, float scale, const uint32& x, const uint32& y)
    {
        for (size_t i {}; i < size; ++i)
        {
            Vec3 V1 = Math::TransformProject(a[i], mat, scale, x, y);
            ++i;
            Vec3 V2 = Math::TransformProject(a[i], mat, scale, x, y);
            ++i;
            Vec3 V3 = Math::TransformProject(a[i], mat, scale, x, y);
            ++i;
            Vec3 V4 = Math::TransformProject(a[i], mat, scale, x, y);
            if (V1.z < 0 || V2.z < 0 || V3.z < 0) return;
                DrawTriangle(V1, V2, V3);
                DrawTriangle(V4, V2, V3);
        }
    }
}
