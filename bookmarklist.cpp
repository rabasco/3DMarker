#include "bookmarklist.h"

BookmarkList::BookmarkList()
{
    clear();
}

std::vector<Bookmark*>* BookmarkList::getList()
{
    return &_list;
}

bool BookmarkList::getUpdated()
{
    return _updated;
}

Bookmark* BookmarkList::getAt(unsigned int index)
{
    return _list[index];
}

int BookmarkList::size()
{
    return _list.size();
}

void BookmarkList::add(Bookmark *bookmark)
{
    _list.push_back(bookmark);
    _updated = true;
}

void BookmarkList::deleteAt(int index)
{
    _list.erase(_list.begin() + index);
    _updated = true;
}

void BookmarkList::clear()
{
    _list.clear();
    _updated = false;
    std::set<unsigned int> faces;
    Bookmark *bookmark = new Bookmark("None", "None", faces);
    _list.push_back(bookmark);
}

bool BookmarkList::save(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QDomDocument* xml = new QDomDocument;
    QDomElement root = xml->createElement("bookmarks");
    xml->appendChild(root);

    // Create node for each bookmark.
    for (unsigned int i = 1; i < _list.size(); i++)
    {
        Bookmark* bookmark = _list[i];
        QDomElement bookmarkNode = xml->createElement("bookmark");
        root.appendChild(bookmarkNode);

        // Set name.
        QDomElement nameNode = xml->createElement("name");
        nameNode.appendChild(xml->createTextNode(bookmark->getName()));
        bookmarkNode.appendChild(nameNode);

        // Set comments.
        QDomElement commentsNode = xml->createElement("comments");
        commentsNode.appendChild(xml->createTextNode(bookmark->getComments()));
        bookmarkNode.appendChild(commentsNode);

        // Set faces.
        QDomElement facesNode = xml->createElement("faces");
        QString faces = "";

        std::set<unsigned int>::const_iterator it = bookmark->getFaces()->begin();
        for ( ; it != bookmark->getFaces()->end(); ++it )
            faces.append( QString().setNum( *it ) ).append(",");

        // Remove last comma.
        faces = faces.remove(faces.length()-1,1);

        facesNode.appendChild(xml->createTextNode(faces));
        bookmarkNode.appendChild(facesNode);

    }

    QTextStream textStream(&file);
    textStream << xml->toString();

    file.close();

    return true;
}

bool BookmarkList::open(QString path, int maxIndex, int* errors)
{
    errors = 0; // Reset errors.

    clear();

    QFile* file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QDomDocument doc("mydocument");
    if (!doc.setContent(file)) {
        return false;
    }

    // Get the root element.
    QDomElement docElem = doc.documentElement();

    // Get <bookmark> nodes
    QDomNodeList nodeList = docElem.elementsByTagName("bookmark");

    for (int i = 0; i < nodeList.count(); i++)
    {
        QDomElement bookmarkNode = nodeList.at(i).toElement();
        QDomElement nameNode = bookmarkNode.elementsByTagName("name").at(0).toElement();
        QDomElement commentsNode = bookmarkNode.elementsByTagName("comments").at(0).toElement();
        QDomElement facesNode = bookmarkNode.elementsByTagName("faces").at(0).toElement();

        QString faces = facesNode.text();
        QStringList faceList = faces.split(",");
        std::set<unsigned int> faceSet;

        for (int j = 0; j < faceList.count(); j++)
        {
            int index = faceList[j].toInt();
            if ( index <= maxIndex )
                faceSet.insert(index);
            else
                errors++;
        }

        Bookmark* bookmark = new Bookmark(nameNode.text(), commentsNode.text(), faceSet);
        _list.push_back(bookmark);
    }

    file->close();

    return true;
}
