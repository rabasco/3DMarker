#ifndef Model_H
#define Model_H

#include <vector>
#include "Vertex.h"
#include "Poly.h"

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
 * The Model class represents a 3D model.
 */
class Model {

private:

    std::vector<Vertex> _vertexList;    /**< Vertices list. */
    std::vector<Poly> _polyList;        /**< Polygon list. */
    float _size;                    /**< Biggest dimension of the model. */

    /**
     * @brief Vertex normal calculation.
     */
    void computeNormals();

    /**
     * @brief Vector product calculation between vector P(v2-v1) and vector Q(v3-v1).
     * @param v1 Origin of vectors.
     * @param v2 Vector P destination.
     * @param v3 Vector Q destination.
     * @param NormalX Result, x-coordinate.
     * @param NormalY Result, y-coordinate.
     * @param NormalZ Result, z-coordinate.
     */
    void vectorProduct(const Vertex v1, const Vertex v2, const Vertex v3, float *NormalX, float *NormalY, float *NormalZ);

public:

    /**
     * @brief Default constructor.
     */
    Model();

    /**
     * @brief Reset model. Clear all variables.
     */
    void clear();

    /**
     * @brief Set a new model.
     * @param vertexList Model vertex list.
     * @param polyList Model polygon list.
     * @param size Biggest dimension of the model.
     */
    void setModel(std::vector<Vertex> _vertexList, std::vector<Poly> _polyList, float size);

    /**
     * @brief Get the current model size.
     * @return Return model current size.
     */
    float getSize();

    /**
     * @brief Get if model loaded.
     * @return Return true if model was loaded, false otherwise.
     */
    bool isLoaded();

    /**
     * @brief Returns polygons counter.
     * @return Polygons counter
     */
    unsigned int numPoly();

    /**
     * @brief Returns vertex counter.
     * @return Vertex counter.
     */
    unsigned int numVertex();

    /**
     * @brief Returns vertex at index position.
     * @param index Index position of vertex.
     * @return Vertex at index position.
     */
    Vertex* getVertexAt(unsigned int index);

    /**
     * @brief Returns polygon at index position.
     * @param index Index position of polygon.
     * @return Polygon at index position.
     */
    Poly* getPolyAt(unsigned int index);



};

#endif // Model_H
