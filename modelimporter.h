#ifndef MODELIMPORTER_H
#define MODELIMPORTER_H

#include "model.h"

class ModelImporter
{
public:
    virtual bool import(Model *model, QString path) const = 0;
};

#endif // MODELIMPORTER_H
