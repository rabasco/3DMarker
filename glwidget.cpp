#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    _model = new Model();
    clear();
}

void GLWidget::setModel(Model* model)
{
    clear();
    _model = model;
    createDisplayLists();

    // Set distance's camera and increment step.
    float size = _model->getSize();
    _cameraDistance = size;
    _cameraIncrement = size / 10.0;
    setCamera();

    updateGL();
}

void GLWidget::clear()
{
    _currentSelection.clear();

    _cameraDistance = 1.0;
    _cameraHAngle   = 0.0;
    _cameraVAngle   = 0.0;
    _cameraIncrement = 1.0;

    _pickSize = 10;

    _mode = ROTATION;
    _selectionMode = ADD;
    _renderMode = SOLID;
    _isPicking = false;
    _hitMode = false;

    glDeleteLists(_modelDisplayListIndex, 1);
    _modelDisplayListIndex = 0;

    updateGL();
}

void GLWidget::drawPoly(Poly *poly, bool wired = false)
{
    int mode = GL_LINE_LOOP;           // Use to select draw mode (TRIANGLES, QUADS & POLYGONS)

    if ( !wired )
    {
        switch (poly->size()) {
            case 3:
                mode = GL_TRIANGLES;
                break;
            case 4:
                mode = GL_QUADS;
                break;
            default:
                mode = GL_POLYGON;
                break;
        }
    }

    std::vector<unsigned int>* list = poly->getList();
    std::vector<unsigned int>::const_iterator it = list->begin(), end = list->end();
    glBegin(mode);
    for ( ; it != end; ++it )
    {
        Vertex *vertex = _model->getVertexAt(*it);
        glNormal3f(vertex->getNormalX(), vertex->getNormalY(), vertex->getNormalZ());
        glVertex3f(vertex->getX(), vertex->getY(), vertex->getZ());
    }
    glEnd();
}

void GLWidget::initializeGL()
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);    // Define clear color
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    // Enable lights
    glEnable(GL_LIGHTING);


    // Create light components
    float ambientLight[] = { 0.08f, 0.08f, 0.08f, 1.0f };
    float diffuseLight[] = { 0.4f, 0.4f, 0.4, 1.0f };
    float specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float position[] = { 0.0f, 0.0f, -100.0f,  1.0f };
    float position2[] = { 0.0f, -100.0f, 0.0f, 1.0f };
    float position3[] = { -100.0f, 0.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT1, GL_POSITION, position);

    glLightfv(GL_LIGHT2, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT2, GL_POSITION, position2);

    glLightfv(GL_LIGHT3, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT3, GL_SPECULAR, specularLight);
    glLightfv(GL_LIGHT3, GL_POSITION, position3);

    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHT2);
    glEnable(GL_LIGHT3);

    // Material
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

void GLWidget::paintGL()
{
    // Clear buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if ( _model->isLoaded() )
    {
        // Draw current selection.
        if ( !_hitMode )
        {
            glColor3f(0.5, 1.0, 0.5);

            std::set<unsigned int>::const_iterator it = _currentSelection.begin();
            for (;it != _currentSelection.end(); ++it)
                drawPoly( _model->getPolyAt(*it), false );
        }

        // Draw complete model.
        if (_renderMode == SOLID_WIRE)
            glCallList(_modelDisplayListIndex+1);
        else
            glCallList(_modelDisplayListIndex);
    }
}

void GLWidget::resizeGL(int width, int height)
{
    float max, position = 0.0;
    if (width > height)
    {
        max = width;
        position = (max - height)/2;
    }
    else
        max = height;

    glViewport(0, -position, max, max);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 1.0, 0.01, 10000.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *pressEvent)
{
    _lastPos = pressEvent->pos();    // Set initial mouse position

    if ( _isPicking )
    {
        if ( pressEvent->button() == Qt::LeftButton )
            _mode = PICK;                               // Left button -> Pick.
        else
            _mode = ROTATION;                           // Other button -> Rotate.
    }
    else
        _mode = ROTATION;                               // If no picking, all buttons -> Rotate.

    if ( _hitMode )
    {
        unsigned int backupPickSize = _pickSize;
        _pickSize = 1;
        _currentSelection.clear();
        picking(pressEvent->pos().x(), pressEvent->pos().y());
        _pickSize = backupPickSize;
        emit pickResult(_currentSelection);
    }


}

void GLWidget::mouseMoveEvent(QMouseEvent *moveEvent)
{
    switch (_mode) {
        case ROTATION:
            _cameraVAngle += moveEvent->pos().x() - _lastPos.x();
            _cameraHAngle += moveEvent->pos().y() - _lastPos.y();
            setCamera();
            break;
        case PICK:
            picking(moveEvent->pos().x(), moveEvent->pos().y());
            break;
        default:
            break;
    }

    _lastPos = moveEvent->pos();
    updateGL();
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    if ( event->orientation() == Qt::Vertical )
    {
        _cameraDistance += (double)(event->delta()) * _cameraIncrement / 100;
        setCamera();
        updateGL();
    }
}

void GLWidget::createDisplayLists()
{
    _modelDisplayListIndex = glGenLists(2);

    // Solid
    glNewList(_modelDisplayListIndex, GL_COMPILE);
        glColor3f(0.44, 0.6, 0.95);   // Set model color.
        for ( unsigned int i = 0; i < _model->numPoly(); ++i )
        {
            glLoadName(i);
            drawPoly( _model->getPolyAt(i), false );
        }
    glEndList();

    // Solid + wire
    glNewList(_modelDisplayListIndex+1, GL_COMPILE);
        glCallList(_modelDisplayListIndex);
        glColor3f(1.0, 1.0, 1.0);   // Set wire color.
        for ( unsigned int i = 0; i < _model->numPoly(); ++i )
            drawPoly( _model->getPolyAt(i), true );
    glEndList();
}

void GLWidget::setCamera()
{
    glLoadIdentity();
    gluLookAt(0, 1, _cameraDistance, 0, 0, 0, 0, 1, 0);
    glRotatef(_cameraHAngle, 1.0, 0.0, 0.0);
    glRotatef(_cameraVAngle, 0.0, 1.0, 0.0);
}

void GLWidget::picking(int x, int y)
{
    glEnable(GL_CULL_FACE);     // Enable culling to improve pick.

    GLuint nameBuffer[10000];     // Buffer names
    GLint hits;                 // Hits
    GLint viewport[4];          // Viewport
    GLfloat projMatrix[16];     // Projection matrix

    glSelectBuffer( 10000, nameBuffer );  // select a buffer for the hit results.
    glRenderMode(GL_SELECT);          // must change to select mode _before_ you touch the name stack.

    glInitNames();                      // initialise the name stack.
    glPushName( 0xffffffff );           // put a dummy on the stack so we can call glLoadName without error.

    glGetFloatv( GL_PROJECTION_MATRIX, projMatrix );

    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv( GL_VIEWPORT, viewport );

    gluPickMatrix( (GLdouble)x, (GLdouble)( this->size().height() - y  ), _pickSize, _pickSize, viewport );

    glMultMatrixf( projMatrix );
    glMatrixMode( GL_MODELVIEW );

    // Set camera
    setCamera();

    // Clear buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Draw model
    glCallList(_modelDisplayListIndex);

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();
    glMatrixMode( GL_MODELVIEW );

    hits = glRenderMode(GL_RENDER);
    if( hits > 0 )
    {
        GLuint names;
        GLuint* pPtr = nameBuffer;

        for( GLint i = 0; i < hits; ++i )
        {
            names = *pPtr;
            ++pPtr;
            ++pPtr;
            ++pPtr;

            for( GLuint j = 0; j < names; ++j, ++pPtr )
            {
                if ( _selectionMode == ADD )
                    _currentSelection.insert(*pPtr);
                if ( _selectionMode == DEL )
                    _currentSelection.erase(*pPtr);
            }
        }
    }

    glDisable(GL_CULL_FACE);
}

void GLWidget::setViewerMode(Mode mode)
{
    if ( mode == PICK )
        _isPicking = true;
    else
        _isPicking = false;

    updateGL();
}

void GLWidget::setSelectionMode(SelectionMode mode)
{
    _selectionMode = mode;
}

void GLWidget::clearSelection()
{
    _currentSelection.clear();
    updateGL();
}

void GLWidget::setCurrentSelection(std::set<unsigned int>* selection)
{
    _currentSelection.clear();
    _currentSelection.insert( selection->begin(), selection->end());

    updateGL();
}

void GLWidget::setRenderMode(RenderMode mode)
{
    _renderMode = mode;

    switch (_renderMode) {
        case POINTS:
            glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
            break;
        case WIRED:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        default:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }

    updateGL();
}

void GLWidget::setPickSize(unsigned int size)
{
    _pickSize =  size;
}

void GLWidget::enableHitMode(bool enabled)
{
    _hitMode = enabled;
    _currentSelection.clear();
    updateGL();
}

std::set<unsigned int> GLWidget::getCurrentSelection()
{
    return _currentSelection;
}
