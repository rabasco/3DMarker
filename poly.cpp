#include "poly.h"

int Poly::size()
{
    return _vertexIndexList.size();
}

std::vector<unsigned int>* Poly::getList()
{
    return &_vertexIndexList;
}

unsigned int Poly::getAt(unsigned int index)
{
    return _vertexIndexList[index];
}

void Poly::add(unsigned int value)
{
    _vertexIndexList.push_back(value);
}
