#define _USE_MATH_DEFINES

#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include <cmath>

#include "glut37/glut.h"

struct Vec3 {

    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }

    Vec3 operator*(const Vec3& v) const { return Vec3(x * v.x, y * v.y, z * v.z); }
    Vec3 operator/(const Vec3& v) const { return Vec3(x / v.x, y / v.y, z / v.z); }

    Vec3 operator*(float d) const { return Vec3(x * d, y * d, z * d); }
    Vec3 operator/(float d) const { return Vec3(x / d, y / d, z / d); }

    Vec3 operator*(float matrix[4][4]) const {

        Vec3 result;
        result.x = x * matrix[0][0] + y * matrix[0][1] + z * matrix[0][2] + matrix[0][3];
        result.y = x * matrix[1][0] + y * matrix[1][1] + z * matrix[1][2] + matrix[1][3];
        result.z = x * matrix[2][0] + y * matrix[2][1] + z * matrix[2][2] + matrix[2][3];
        return result;
    }

    float dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

    Vec3 normalize() const {
        float mag = sqrt(x * x + y * y + z * z);
        return Vec3(x / mag, y / mag, z / mag);
    }

    float norm() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 cross(const Vec3& v) const {
        return Vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    Vec3 pow(float p) const {

        return Vec3(std::pow(x, p), std::pow(y, p), std::pow(z, p));
    }
};

Vec3 operator*(float d, const Vec3& v) {
    return Vec3(d * v.x, d * v.y, d * v.z);
}

Vec3 operator/(float d, const Vec3& v) {
    return Vec3(d / v.x, d / v.y, d / v.z);
}


struct HitInfo {

    bool hit;
    float distance;
    Vec3 point;

    HitInfo(bool hit = false, float distance = -1, Vec3 point = NULL) : hit(hit), distance(distance), point(point) {}
};

int gWidth = 512;
int gHeight = 512;

float gL = -0.1;
float gR = 0.1;
float gB = -0.1;
float gT = 0.1;
float gN = -0.1;
float gF = -1000;

int gNumVertices = 0;    // Number of 3D vertices.
int gNumTriangles = 0;    // Number of triangles.
int* gIndexBuffer = NULL; // Vertex indices for the triangles.
Vec3* gVertexBuffer = NULL; // Array for vertices

Vec3* gPixels = new Vec3[gWidth * gHeight];

float gMatrix[4][4] = {
    {2, 0, 0, 0},
    {0, 2, 0, 0},
    {0, 0, 2, -7},
    {0, 0, 0, 1}
}; // Translate(0,0,-7) * Scale(2) Matrix

void createScene()
{
    int width = 32;
    int height = 16;

    float theta, phi;
    int t;

    gNumVertices = (height - 2) * width + 2;
    gNumTriangles = (height - 2) * (width - 1) * 2;

    // TODO: Allocate an array for gNumVertices vertices.
    gVertexBuffer = new Vec3[gNumVertices];
    gIndexBuffer = new int[3 * gNumTriangles];


    t = 0;
    for (int j = 1; j < height - 1; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            theta = (float)j / (height - 1) * M_PI;
            phi = (float)i / (width - 1) * M_PI * 2;

            float   x = sinf(theta) * cosf(phi);
            float   y = cosf(theta);
            float   z = -sinf(theta) * sinf(phi);

            // TODO: Set vertex t in the vertex array to {x, y, z}.
            // Applying the actual position of vertices using matrix multiplication
            gVertexBuffer[t] = Vec3(x, y, z) * gMatrix;

            t++;
        }
    }

    // TODO: Set vertex t in the vertex array to {0, 1, 0}.
    // Applying the actual position of vertices using matrix multiplication
    gVertexBuffer[t] = Vec3(0, 1, 0) * gMatrix;

    t++;

    // TODO: Set vertex t in the vertex array to {0, -1, 0}.
    // Applying the actual position of vertices using matrix multiplication
    gVertexBuffer[t] = Vec3(0, -1, 0) * gMatrix;

    t++;

    t = 0;
    for (int j = 0; j < height - 3; ++j)
    {
        for (int i = 0; i < width - 1; ++i)
        {
            gIndexBuffer[t++] = j * width + i;
            gIndexBuffer[t++] = (j + 1) * width + (i + 1);
            gIndexBuffer[t++] = j * width + (i + 1);
            gIndexBuffer[t++] = j * width + i;
            gIndexBuffer[t++] = (j + 1) * width + i;
            gIndexBuffer[t++] = (j + 1) * width + (i + 1);
        }
    }
    for (int i = 0; i < width - 1; ++i)
    {
        gIndexBuffer[t++] = (height - 2) * width;
        gIndexBuffer[t++] = i;
        gIndexBuffer[t++] = i + 1;
        gIndexBuffer[t++] = (height - 2) * width + 1;
        gIndexBuffer[t++] = (height - 3) * width + (i + 1);
        gIndexBuffer[t++] = (height - 3) * width + i;
    }

    // The index buffer has now been generated. Here's how to use to determine
    // the vertices of a triangle. Suppose you want to determine the vertices
    // of triangle i, with 0 <= i < gNumTriangles. Define:
    //
    // k0 = gIndexBuffer[3*i + 0]
    // k1 = gIndexBuffer[3*i + 1]
    // k2 = gIndexBuffer[3*i + 2]
    //
    // Now, the vertices of triangle i are at positions k0, k1, and k2 (in that
    // order) in the vertex array (which you should allocate yourself at line
    // 27).
    //
    // Note that this assumes 0-based indexing of arrays (as used in C/C++,
    // Java, etc.) If your language uses 1-based indexing, you will have to
    // add 1 to k0, k1, and k2.
}


HitInfo intersectTriangle(const Vec3& orig, const Vec3& dir, const Vec3& v0, const Vec3& v1, const Vec3& v2) {

    const float EPSILON = 1e-6;

    Vec3 edge1 = v1 - v0;
    Vec3 edge2 = v2 - v0;

    Vec3 h = dir.cross(edge2);
    float a = edge1.dot(h);

    if (a > -EPSILON && a < EPSILON) {
        return HitInfo(false);
    }


    float f = 1.0f / a;

    Vec3 s = orig - v0;
    float u = f * (s.dot(h));
    if (u < 0.0 || u > 1.0) {
        return HitInfo(false);
    }


    Vec3 q = s.cross(edge1);
    float v = f * dir.dot(q);
    if (v < 0.0 || u + v > 1.0) {
        return HitInfo(false);
    }

    float t = f * edge2.dot(q);

    if (t > EPSILON) {
        return HitInfo(true, t, orig + dir * t);
    }

    return HitInfo(false);
}

void render() {

    Vec3 eye(0, 0, 0);

    for (int y = 0; y < gHeight; y++)
    {
        for (int x = 0; x < gWidth; x++) {

            float u = gL + (gR - gL) * (x + 0.5) / gWidth;
            float v = gB + (gT - gB) * (y + 0.5) / gHeight;

            Vec3 rayDirection = Vec3(u, v, gN).normalize();

            HitInfo hitInfo(false);

            for (int i = 0, l = gNumTriangles; i < l; i++)
            {
                Vec3 v0 = gVertexBuffer[gIndexBuffer[i * 3 + 0]];
                Vec3 v1 = gVertexBuffer[gIndexBuffer[i * 3 + 1]];
                Vec3 v2 = gVertexBuffer[gIndexBuffer[i * 3 + 2]];

                HitInfo currentHitInfo = intersectTriangle(eye, rayDirection, v0, v1, v2);
                if (currentHitInfo.hit == true) {

                    if (hitInfo.hit == false || currentHitInfo.distance < hitInfo.distance) {
                        hitInfo = currentHitInfo;
                    }
                }
            }

            Vec3 color = Vec3(0, 0, 0);
            if (hitInfo.hit == true) {
                color = Vec3(1, 1, 1);
            }

            gPixels[y * gWidth + x] = color;
        }
    }

    glDrawPixels(gWidth, gHeight, GL_RGB, GL_FLOAT, gPixels);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    render();

    glutSwapBuffers();
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(gWidth, gHeight);
    glutInitWindowPosition(100, 100);

    glutCreateWindow("HW4 TNR");

    createScene();

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;
}