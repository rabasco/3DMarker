#include "model.h"

Model::Model( )
{
    clear();
}

void Model::clear()
{
    _vertexList.clear();
    _polyList.clear();
    _size = 0.0;
}

void Model::setModel(std::vector<Vertex> vertexList, std::vector<Poly> polyList, float size)
{
    this->_vertexList.clear();
    this->_vertexList.insert(this->_vertexList.begin(), vertexList.begin(), vertexList.end());

    this->_polyList.clear();
    this->_polyList.insert(this->_polyList.begin(), polyList.begin(), polyList.end());

    _size = size;

    computeNormals();
}

float Model::getSize()
{
    return _size;
}

void Model::vectorProduct(Vertex v1, Vertex v2, Vertex v3, float *NormalX,
                            float *NormalY, float *NormalZ)
{
    float Qx, Qy, Qz, Px, Py, Pz;

    Px = v2.getX()-v1.getX();
    Py = v2.getY()-v1.getY();
    Pz = v2.getZ()-v1.getZ();
    Qx = v3.getX()-v1.getX();
    Qy = v3.getY()-v1.getY();
    Qz = v3.getZ()-v1.getZ();
    *NormalX = Py*Qz - Pz*Qy;
    *NormalY = -(Pz*Qx - Px*Qz);
    *NormalZ = Px*Qy - Py*Qx;
}

void Model::computeNormals()
{
    float normalX, normalY, normalZ;

    std::vector<Poly>::iterator it, end;
    Poly *poly;
    for ( it = _polyList.begin(), end = _polyList.end(); it != end; ++it )
    {
        poly = &(*it);

        // Vector product
        vectorProduct(_vertexList[poly->getAt(0)],
                _vertexList[poly->getAt(1)],
                _vertexList[poly->getAt(2)],
                &normalX, &normalY, &normalZ);

        // Save normal
        Vertex *vertex = &_vertexList[poly->getAt(0)];
        vertex->addNormal(normalX, normalY, normalZ);

        vertex = &_vertexList[poly->getAt(1)];
        vertex->addNormal(normalX, normalY, normalZ);

        vertex = &_vertexList[poly->getAt(2)];
        vertex->addNormal(normalX, normalY, normalZ);
    }

    std::vector<Vertex>::iterator itV, endV;
    for ( itV = _vertexList.begin(), endV = _vertexList.end(); itV != endV; ++itV )
        (*itV).normalize();

}

bool Model::isLoaded()
{
    if ( _vertexList.size() > 0 )
        return true;
    else
        return false;
}

unsigned int Model::numPoly()
{
    return _polyList.size();
}

unsigned int Model::numVertex()
{
    return _vertexList.size();
}

Vertex* Model::getVertexAt(unsigned int index)
{
    return &_vertexList[index];
}

Poly* Model::getPolyAt(unsigned int index)
{
    return &_polyList[index];
}
