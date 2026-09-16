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
        Result.x = x - other.x;
        Result.y = y - other.y;
        Result.z = z - other.z;
        return Result;
    }
    float x, y,z;
};
struct Mat4
{
    int32 mat4[4][4] {};
    Mat4 operator*(const Mat4& other)
    {
        Mat4 tmp;
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                long long sum = 0;
                for (int k = 0; k < 4; ++k)
                {
                    sum += (long long)mat4[i][k] * other.mat4[k][j];
                }
                tmp.mat4[i][j] = (int32)(sum / 4096);
            }
        }
        return tmp;
    }
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
        float rx = (mat.mat4[0][0]*localPos.x + mat.mat4[0][1]*localPos.y + mat.mat4[0][2]*localPos.z) / ONE;
        float ry = (mat.mat4[1][0]*localPos.x + mat.mat4[1][1]*localPos.y + mat.mat4[1][2]*localPos.z) / ONE;
        float rz = (mat.mat4[2][0]*localPos.x + mat.mat4[2][1]*localPos.y + mat.mat4[2][2]*localPos.z) / ONE;

        float zOffset = rz + 400.0f;
        Vec3 screenPos;
        if (zOffset <= 1.0f)
        {
            screenPos.x = -99999.0f;
            screenPos.y = -99999.0f;
            screenPos.z = -1.0f;
            return screenPos;
        }
        float distanceScaling = 300.0f;
        screenPos.x = centerX + (rx * scale * distanceScaling) / zOffset;
        screenPos.y = centerY + (ry * scale * distanceScaling) / zOffset;
        screenPos.z = zOffset;

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
    inline void Mat4x4RotX(Mat4 &mat, float angle)
    {
        Mat4x4IdentEqu(mat);
        float Cos = cosf(angle);
        float Sin = sinf(angle);
        mat.mat4[1][1] = (int32)(Cos * ONE);
        mat.mat4[1][2] = (int32)(-Sin* ONE);
        mat.mat4[2][1] = (int32)(Sin * ONE);
        mat.mat4[2][2] = (int32)(Cos * ONE);
    }
}
