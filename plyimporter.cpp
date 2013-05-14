#include "plyimporter.h"

bool PlyImporter::import(Model *model, QString path) const
{
    enum Area { HEADER, VERTICES, FACES, END };
    Area area = HEADER;
    float xmin, xmax, ymin, ymax, zmin, zmax = 0.0;     // Used to center model in view.
    unsigned int numVertices, numFaces = 0;
    QString currentLine;                                // Current line readed.
    QStringList stringList;
    std::vector<Vertex> vertexList;
    std::vector<Poly> polyList;
    bool error = false;

    vertexList.clear();
    polyList.clear();

    QFile file(path);
    if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) )
    {
        std::cerr << "Error: Error openning file." << std::endl;
        return false;
    }

    QTextStream in(&file);
    while ( !in.atEnd() && !error )
    {
        currentLine = in.readLine();

        // Proccess header
        if ( area == HEADER )
        {
            if ( currentLine.contains("element vertex") )
            {
                stringList = currentLine.split(" ");
                numVertices = stringList[2].toInt();
            }
            else if ( currentLine.contains("element face") )
            {
                stringList = currentLine.split(" ");
                numFaces = stringList[2].toInt();
            }
            else if (currentLine.contains("format binary_little_endian 1.0") ||
                      currentLine.contains("format binary_big_endian 1.0"))
            {
                std::cerr << "Error: Only support for ascii ply files." << std::endl;
                error = true;
            }
            else if ( currentLine.contains("end_header") )
                area = VERTICES;
        }
        else if ( area == VERTICES )    // Proccess vertices
        {
            if ( vertexList.size() < numVertices )
            {
                stringList = currentLine.split(" ");
                Vertex* vertex = new Vertex(stringList[0].toFloat(), stringList[1].toFloat(), stringList[2].toFloat());
                vertexList.push_back(*vertex);

                if ( vertexList.size() == 1 )
                {
                    xmin = xmax = vertex->getX();
                    ymin = ymax = vertex->getY();
                    zmin = zmax = vertex->getZ();
                }
                else
                {
                    if (vertex->getX() < xmin)
                        xmin = vertex->getX();
                    if (vertex->getX() > xmax)
                        xmax = vertex->getX();
                    if (vertex->getY() < ymin)
                        ymin = vertex->getY();
                    if (vertex->getY() > ymax)
                        ymax = vertex->getY();
                    if (vertex->getZ() < zmin)
                        zmin = vertex->getZ();
                    if (vertex->getZ() > zmax)
                        zmax = vertex->getZ();
                }

                if ( vertexList.size() == numVertices )
                    area = FACES;
            }
        }
        else if ( area == FACES )   // Proccess faces
        {
            if ( polyList.size() < numFaces && !currentLine.isNull() )
            {
                stringList = currentLine.split(" ");
                Poly *poly = new Poly();

                for (int i = 0; i < stringList[0].toInt(); i++)
                    poly->add(stringList[i+1].toInt());

                polyList.push_back(*poly);
            }
        }
    }

    // Close file.
    file.close();

    // Return if error.
    if ( error )
        return false;

    // Get model position from origin.
    float sizeX = xmax - xmin;
    float sizeY = ymax - ymin;
    float sizeZ = zmax - zmin;
    float modelSize = 0.0;

    if ( sizeX > sizeY && sizeX > sizeZ)
        modelSize = sizeX;
    else if ( sizeY > sizeZ)
        modelSize = sizeY;
    else
        modelSize = sizeZ;

    float centerX = xmax - (sizeX / 2.0);
    float centerY = ymax - (sizeY / 2.0);
    float centerZ = zmax - (sizeZ / 2.0);

    for ( unsigned int i = 0; i < vertexList.size(); i++)
    {
        Vertex *vertex = &vertexList[i];
        vertex->setValues(vertex->getX()-centerX, vertex->getY()-centerY, vertex->getZ()-centerZ);
    }

    // Set model
    model->setModel(vertexList, polyList, modelSize);

    return true;
}
