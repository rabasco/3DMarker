#ifndef VERTEX_H
#define VERTEX_H

#include <math.h>

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
 * The Vertex class represents a vertex in the 3D space.
 */
class Vertex
{

private:

    float _x, _y, _z;      /**< Vertex cordinates. */
    float _nx, _ny, _nz;   /**< Vertex normals. */

    /**
     * @brief Get vector module.
     * @param x X vector component.
     * @param y Y vector component.
     * @param z Z vector component.
     * @return Return the vector module.
     */
    float module(float x, float y, float z);

public:

    /**
     * @brief Default constructor.
     */
    Vertex( );
    Vertex(float x, float y, float z);

    float getX();
    float getY();
    float getZ();

    float getNormalX();
    float getNormalY();
    float getNormalZ();

    void setValues(float x, float y, float z);

    /**
     * @brief Add a new normal value to current value.
     * @param normalX X value of normal to add.
     * @param normalY Y value of normal to add.
     * @param normalZ Z value of normal to add.
     */
    void addNormal(float normalX, float normalY, float normalZ);

    /**
     * @brief Normalize vertex normal value.
     */
    void normalize();

};

#endif // VERTEX_H
