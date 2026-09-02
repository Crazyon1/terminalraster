#pragma once
#include <cmath>
#include <cstdint>
#include <cstdio>
#define uint32 uint32_t
#define int32 int32_t
#define ONE 4096
struct Vec3
{
    Vec3 operator-(const Vec3& other)
    {
        Vec3 Result;
        Result.vec3[0] = vec3[0] - other.vec3[0];
        Result.vec3[1] = vec3[1] - other.vec3[1];
        Result.vec3[2] = vec3[2] - other.vec3[2];
        return Result;
    }
    float vec3[3];
};
struct Mat4
{
    int32 mat4[4][4] {};
};
namespace Math
{
    inline void swap(Vec3& v1, Vec3& v2)
    {
        Vec3 tmp = v1;
        v1 = v2;
        v2 = tmp;
    }
    inline Vec3 TransformProject(Vec3& localPos, Mat4 mat, float scale, uint32 centerX, uint32 centerY)
    {
        float rx = (mat.mat4[0][0]*localPos.vec3[0] + mat.mat4[0][1]*localPos.vec3[1] + mat.mat4[0][2]*localPos.vec3[2]) / ONE;
        float ry = (mat.mat4[1][0]*localPos.vec3[0] + mat.mat4[1][1]*localPos.vec3[1] + mat.mat4[1][2]*localPos.vec3[2]) / ONE;
        float rz = (mat.mat4[2][0]*localPos.vec3[0] + mat.mat4[2][1]*localPos.vec3[1] + mat.mat4[2][2]*localPos.vec3[2]) / ONE;

        float zOffset = rz + 400.0f;
        Vec3 screenPos;
        if (zOffset <= 1.0f)
        {
            screenPos.vec3[0] = -99999.0f;
            screenPos.vec3[1] = -99999.0f;
            screenPos.vec3[2] = -1.0f;
            return screenPos;
        }
        float distanceScaling = 300.0f;
        screenPos.vec3[0] = centerX + (rx * scale * distanceScaling) / zOffset;
        screenPos.vec3[1] = centerY + (ry * scale * distanceScaling) / zOffset;
        screenPos.vec3[2] = zOffset;

        return screenPos;
    }
    inline void Mat4x4IdentEqu(Mat4& mat) 
    { 
        for(int i=0; i<4; i++) for(int j=0; j<4; j++) mat.mat4[i][j] = 0;
        mat.mat4[0][0] = ONE;
        mat.mat4[1][1] = ONE;
        mat.mat4[2][2] = ONE;
        mat.mat4[3][3] = ONE;
    }
    inline void Mat4x4RotY(Mat4& mat, float angle)
    {
        Mat4x4IdentEqu(mat);
        float Cos = std::cosf(angle);
        float Sin = std::sinf(angle);
        mat.mat4[0][0] = (int32)(Cos * ONE);
        mat.mat4[0][2] = (int32)(Sin * ONE);
        mat.mat4[2][0] = (int32)(-Sin * ONE);
        mat.mat4[2][2] = (int32)(Cos * ONE);
    }
}