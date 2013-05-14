#ifndef BOOKMARKLIST_H
#define BOOKMARKLIST_H

#include <QFile>
#include <QDomDocument>
#include <QStringList>
#include <QTextStream>
#include <vector>
#include "bookmark.h"

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
 * The BookmarkList class represents the user's bookmarks list. The
 * bookmark list always contains almost one bookmark ('None' bookmark).
 * 'None' bookmark represents the no selection.
 */
class BookmarkList
{
private:

    std::vector<Bookmark*> _list;       /**< List of bookmarks. */
    bool _updated;                      /**< Indicates if the list was updated. */

public:

    /**
     * @brief Default constructor.
     */
    BookmarkList();

    /**
     * @brief Return bookmark list.
     * @return Bookmark list.
     */
    std::vector<Bookmark*>* getList();

    /**
     * @brief Return if list was updated.
     * @return True if list was updated, false otherwise.
     */
    bool getUpdated();

    /**
     * @brief Add new bookmark to the list.
     * @param bookmark Bookmark reference to add.
     */
    void add(Bookmark *bookmark);

    /**
     * @brief Return a Bookmark in index postion.
     * @param index Index position.
     * @return Bookmark at index position.
     */
    Bookmark* getAt(unsigned int index);

    /**
     * @brief Return current list size.
     * @return Current list size.
     */
    int size();

    /**
     * @brief Delete bookmark at index.
     * @param index Index of bookmark to delete.
     */
    void deleteAt(int index);

    /**
     * @brief Clear the bookmark list and add the default bookmark 'None'.
     */
    void clear();

    /**
     * @brief Read a bookmark list from file.
     * @param path File to read.
     * @param maxIndex Maximum index allowed.
     * @param errors Number of face index that exceed the maxIndex value.
     * @return True if loaded, false otherwise.
     */
    bool open(QString path, int maxIndex, int *errors);

    /**
     * @brief Save the bookmark list to file.
     * @param path File path to save.
     * @return True if saved, false otherwise.
     */
    bool save(QString path);

};

#endif // BOOKMARKLIST_H
