#ifndef POLY_H
#define POLY_H

#include <vector>

/**
 * This source file is part of 3DMarker.
 * @author Jose Manuel Rabasco de Damas (rabasco@gmail.com)
 * @version 1.0
 *
 * @section LICENSE
 *
 * 3DMarker is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 3, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 *
 * @section DESCRIPTION
 *
 * The Poly class represents a polygon composed by a undetermined number
 * of vertices indices.
 */
class Poly
{
private:

    std::vector<unsigned int> _vertexIndexList;  /**< Vertex index list. */

public:

    /**
     * @brief Returns list size.
     * @return List size.
     */
    int size();

    /**
     * @brief Returns vertex index list.
     * @return Vertex index list.
     */
    std::vector<unsigned int>* getList();

    /**
     * @brief Returns vertex index at index position.
     * @param index Index of vertex index.
     * @return Vertex index at index position.
     */
    unsigned int getAt(unsigned int index);

    /**
     * @brief Add a new vertex index.
     * @param value Value of new vertex index.
     */
    void add(unsigned int value);

};

#endif // POLY_H
