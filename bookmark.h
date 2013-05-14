#ifndef BOOKMARK_H
#define BOOKMARK_H

#include <QString>
#include <set>

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
 * The bookmark class represents a region of 3D model selected by user.
 */
class Bookmark
{
private:

    QString _name;                               /**< The name of the bookmark. */
    QString _comments;                           /**< The comments of the bookmark. */
    std::set<unsigned int> _faces;              /**< The face list of the bookmark. */

public:

    /**
     * @brief Constructor.
     */
    Bookmark(QString name, QString comments, std::set<unsigned int> faces);
    QString getName();
    void setName(QString name);
    QString getComments();
    void setComments(QString comments);
    std::set<unsigned int>* getFaces();
    void setFaces(std::set<unsigned int> faces);

};

#endif // BOOKMARK_H
