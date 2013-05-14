#ifndef PLYIMPORTER_H
#define PLYIMPORTER_H

#include <iostream>

#include <QDateTime>
#include <QFile>
#include <QStringList>
#include <QTextStream>

#include "modelimporter.h"

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
 * The PlyImporter class represents a ply importer class. We use this class
 * to open ply models from file.
 */
class PlyImporter : public ModelImporter
{
public:

    /**
     * @brief Import a model from ply file.
     * @param model Model to be loaded.
     * @param path File to load.
     * @return True if model was loaded, false otherwise.
     */
    bool import(Model *model, QString path) const;

};

#endif // PLYIMPORTER_H
