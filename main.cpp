#include "framebuffer.hpp"
#include "math.hpp"
#include "rasterizer.hpp"
#include <chrono>
#include <iostream>
#include <thread>
/*I've been working on a seperate project that uses a framebuffer 
but through working on it I realized that I use a framebuffer to make a renderer using some ascii
so this is me just testing that idea for funzies
*/
const uint32 boxwidth = 10;
const uint32 boxheight = 5;
const float scale = 0.25f;
int main()
{
    float angle{};
    Mat4 mat {};
    Vec3 Vertex1, Vertex2, Vertex3;
    Vertex1.vec3[0] = 0.0f;    Vertex1.vec3[1] = -100.0f; Vertex1.vec3[2] = 0.0f;  
    Vertex2.vec3[0] = 100.0f;  Vertex2.vec3[1] = 100.0f;  Vertex2.vec3[2] = 0.0f;  
    Vertex3.vec3[0] = -100.0f; Vertex3.vec3[1] = 100.0f;  Vertex3.vec3[2] = 0.0f;
    
    Clearbuffers();
    while (true)
    {
        angle += 0.04f;
        Math::Mat4x4RotY(mat, angle);
        Vec3 screenV1 = Math::TransformProject(Vertex1, mat, scale, CENTERX, CENTERY);
        Vec3 screenV2 = Math::TransformProject(Vertex2, mat, scale, CENTERX, CENTERY);
        Vec3 screenV3 = Math::TransformProject(Vertex3, mat, scale, CENTERX, CENTERY);
        if (screenV1.vec3[2] < 0 || screenV2.vec3[2] < 0 || screenV3.vec3[2] < 0) continue;
        Rasterizer::DrawTriangle(screenV1, screenV2, screenV3);
        Rasterizer::swapbuffers();
        std::this_thread::sleep_for(std::chrono::microseconds(20000));
        Printframebuffer();
        Clearbuffers();

    }
    //clearbuffers();
    //Rasterizer::swapbuffers();
    //printframebuffer();
    return 0;
}