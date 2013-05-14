#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <algorithm>

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QStatusBar>
#include <QListWidgetItem>
#include <QTime>
#include "glwidget.h"
#include "bookmarklist.h"

namespace Ui {
class MainWindow;
}

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
 * The MainWindow class represents the program main window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;             /**< User interface. */
    BookmarkList* _bookmarkList;     /**< Bookmark list. */
    Model* _model;                   /**< 3D model. */
    unsigned int _indexTest;        /**< Index of current question. */

    // Bookmarks management

    /**
     * @brief Add a new bookmark to the list.
     * @param name Name of bookmark.
     * @param comments Comments of bookmark.
     * @param selection Face list of bookmark.
     */
    void addBookmark(QString name, QString comments, std::set<unsigned int> selection);

    /**
     * @brief Update bookmark.
     * @param name Name of bookmark.
     * @param comments Comments of bookmark.
     * @param selection Face list of bookmark.
     */
    void updateBookmark(QString name, QString comments, std::set<unsigned int> selection);

    /**
     * @brief Delete selected bookmark.
     */
    void deleteCurrentBookmark();

    /**
     * @brief Clear bookmark list.
     */
    void clearBookmarkList();

public:

    /**
     * @brief Default constructor.
     * @param parent Parent widget.
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Default desctructor.
     */
    ~MainWindow();

public slots:

    void openModel();   /**< Menu action: Open model. */
    void closeModel();  /**< Menu action: Close model. */

    void openBookmarksFile();   /**< Menu action: Open bookmarks file. */
    void closeBookmarksFile();  /**< Menu action: Close bookmarks file. */
    void saveBookmarksFile();   /**< Menu action: Save current bookmarks file. */
    void saveAsBookmarksFile(); /**< Menu action: Save under different name the current bookmarks file. */

    void viewAsPoints();        /**< Menu action: View as points. */
    void viewAsWired();         /**< Menu action: View as wired. */
    void viewAsSolid();         /**< Menu action: View as solid. */
    void viewAsSolidWire();     /**< Menu action: View as solid + wired. */

    void viewerMode();           /**< Menu action: Set viewer mode. */
    void editorMode();           /**< Menu action: Set editor mode. */
    void testMode();             /**< Menu action: Set test mode. */

    void setAddSelectionMode();     /**< Button action: Set selection mode: ADD. */
    void setRemoveSelectionMode();  /**< Button action: Set selection mode: DEL. */
    void saveBookmark();            /**< Button action: Save current bookmark. */
    void discardBookmark();         /**< Button action: Discard current bookmark. */
    void setBrushSize(int size);    /**< Slider action: Set brush size. */

    void showListPanel();           /**< Button action: Show bookmark list panel. */
    void showAddSectionPanel();     /**< Button action: Show new bookmark panel. */
    void showEditSectionPanel();    /**< Button action: Show edit bookmark panel. */
    void showInfoSectionPanel();    /**< Button action: Show bookmark information panel. */

    void deleteCurrentSection();    /**< Button action: Delete selected bookmark. */

    void nextQuestion();            /**< Button action: Generate a new question. */
    void checkResponse(std::set<unsigned int> hits);   /**< Test: User response. */

    void listWidgetItemClicked(QListWidgetItem *);  /**< Bookmark click action. */

};

#endif // MAINWINDOW_H
