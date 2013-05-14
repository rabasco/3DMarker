#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <glu.h>
#include <set>
#include <QGLWidget>
#include <QMouseEvent>
#include <QDateTime>
#include "plyimporter.h"
#include "bookmarklist.h"

/**
 * @brief The Mode enum represents how mouse click affects to the view:
 * ROTATION: Produces a rotation.
 * ZOOM: Produces a zoom in the view.
 * PICK: Produces a selection of faces.
 */
enum Mode {
    ROTATION,
    ZOOM,
    PICK
};

/**
 * @brief The SelectionMode enum represents how mouse clicks affects to
 * current selection:
 * ADD: Selected faces are ADDED to selected face list.
 * DEL: Selected faces are REMOVED from selected face list.
 */
enum SelectionMode {
    ADD,
    DEL
};

/**
 * @brief The RenderMode enum represents a diferents mode to render the scene:
 * SOLID: Solid mode. (default)
 * WIRE: Wire mode.
 * SOLID_WIRE: Solid + wire mode.
 */
enum RenderMode {
    POINTS,
    WIRED,
    SOLID,
    SOLID_WIRE
};

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
 * The GLWidget class represents a 3D model viewer.
 */
class GLWidget : public QGLWidget
{
    Q_OBJECT

private:

    Mode _mode;                      /**< Current viewer mode. */
    SelectionMode _selectionMode;    /**< Current selection mode. */
    RenderMode _renderMode;          /**< Current render mode. */
    Model* _model;                   /**< 3D Model. */
    QPoint _lastPos;                 /**< Last mouse position. */
    bool _isPicking;                 /**< True when picking (selecting polygons). */
    bool _hitMode;                   /**< True when hit mode is enabled. */

    GLuint _modelDisplayListIndex;   /**< Index of displayList */
    GLubyte _displayLists[2];        /**< References to displayList */

    float _cameraDistance;           /**< Camera distance from origin. */
    float _cameraHAngle;             /**< Camera horizontal angle. */
    float _cameraVAngle;             /**< camera vertical angle. */
    float _cameraIncrement;          /**< Camera steps (depends of model size). */

    unsigned int _pickSize;           /**< Pick window size. */

    std::set<unsigned int> _currentSelection;  /**< Faces selected by user. */

    /**
     * @brief Draw a polygon solid or wired.
     * @param poly Polygon to draw.
     * @param wired True for wired mode, false for solid mode.
     */
    void drawPoly(Poly *poly, bool wired);

    /**
     * @brief Special render to select polygons.
     * @param x Horizontal mouse position.
     * @param y Vertical mouse position.
     */
    void picking(int x, int y);

    /**
     * @brief Configure camera on scene.
     */
    void setCamera();

    /**
     * @brief Initialize the display lists.
     */
    void createDisplayLists();

protected:
    /**
     * @brief Represents a user interaction: Mouse click.
     * @param pressEvent Mouse event.
     */
    void mousePressEvent(QMouseEvent *pressEvent);

    /**
     * @brief Represents a user interaction: Mouse move when click.
     * @param moveEvent Mouse event.
     */
    void mouseMoveEvent(QMouseEvent *moveEvent);

    /**
     * @brief Represents a user interaction: Mouse wheel move.
     * @param event Mouse event.
     */
    void wheelEvent(QWheelEvent *event);

public:

    /**
     * @brief Default constructor.
     * @param parent Parent of widget.
     */
    GLWidget(QWidget *parent = 0);

    /**
     * @brief Configure OpenGL environment.
     */
    void initializeGL();

    /**
     * @brief Main OpenGL draw function.
     */
    void paintGL();

    /**
     * @brief Main resize OpenGL function.
     * @param width New viewer width.
     * @param height New viewer height.
     */
    void resizeGL(int width, int height);

    /**
     * @brief Set a new model to draw.
     * @param model Model to draw.
     */
    void setModel(Model* model);

    /**
     * @brief Change viewer mode.
     * @param mode New viewer mode.
     */
    void setViewerMode(Mode mode);

    /**
     * @brief Change viewer selection mode.
     * @param mode New viewer selection mode.
     */
    void setSelectionMode(SelectionMode mode);

    /**
     * @brief Clear viewer.
     */
    void clear();

    /**
     * @brief Clear only current selection.
     */
    void clearSelection();

    /**
     * @brief Set the current selection.
     * @param selection Current selection to display.
     */
    void setCurrentSelection(std::set<unsigned int> *selection);

    /**
     * @brief Set the current render mode.
     * @param mode New render mode.
     */
    void setRenderMode(RenderMode mode);

    /**
     * @brief Set the current pick size.
     * @param size New pick size.
     */
    void setPickSize(unsigned int size);

    /**
     * @brief Enable or disable hit mode.
     * @param Enable hit mode.
     */
    void enableHitMode(bool enabled);

    /**
     * @brief Return current selection.
     * @return Current selection.
     */
    std::set<unsigned int> getCurrentSelection();

signals:
    void pickResult(std::set<unsigned int> hit);

};

#endif // GLWIDGET_H
