#include "vertex.h"

Vertex::Vertex( )
{
    _x = _y = _z = 0.0;
    _nx = 0.0;
    _ny = 0.0;
    _nz = 0.0;
}

Vertex::Vertex(float x, float y, float z)
{
    _x = x;
    _y = y;
    _z = z;
    _nx = 0.0;
    _ny = 0.0;
    _nz = 0.0;
}

float Vertex::getX()
{
    return _x;
}

float Vertex::getY()
{
    return _y;
}

float Vertex::getZ()
{
    return _z;
}

float Vertex::getNormalX()
{
    return _nx;
}

float Vertex::getNormalY()
{
    return _ny;
}

float Vertex::getNormalZ()
{
    return _nz;
}

void Vertex::addNormal(float normalX, float normalY, float normalZ)
{
    _nx += normalX;
    _ny += normalY;
    _nz += normalZ;
}

float Vertex::module(float x, float y, float z)
{
    float len;
    len = sqrt( x*x + y*y + z*z );

    return len;
}

void Vertex::normalize()
{
    float len;

    len = this->module(_nx, _ny, _nz);
    len = 1.0/len;
    _nx *= len;
    _ny *= len;
    _nz *= len;
}

void Vertex::setValues(float x, float y, float z)
{
    _x = x;
    _y = y;
    _z = z;
}
