#include "bookmark.h"

Bookmark::Bookmark(QString name, QString comments, std::set<unsigned int> faces)
{
    _name = name;
    _comments = comments;
    _faces.insert(faces.begin(), faces.end());
}

QString Bookmark::getName()
{
    return _name;
}

void Bookmark::setName(QString name)
{
    _name = name;
}

QString Bookmark::getComments()
{
    return _comments;
}

void Bookmark::setComments(QString comments)
{
    _comments = comments;
}

std::set<unsigned int>* Bookmark::getFaces()
{
    return &_faces;
}

void Bookmark::setFaces(std::set<unsigned int> faces)
{
    _faces.clear();
    _faces.insert(faces.begin(), faces.end() );
}
