#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Menu
    QMenu* modelMenu = menuBar()->addMenu("&Model");
    modelMenu->addAction("&Open model...",  this, SLOT(openModel()) );
    modelMenu->addSeparator();
    modelMenu->addAction("&Close model",    this, SLOT(closeModel()) );

    QMenu* markMenu = menuBar()->addMenu("&Bookmarks");
    markMenu->addAction("&New file...",     this, SLOT(closeBookmarksFile()) );
    markMenu->addAction("&Open file...",    this, SLOT(openBookmarksFile()) );
    markMenu->addSeparator();
    markMenu->addAction("&Close file",      this, SLOT(closeBookmarksFile()) );
    markMenu->addSeparator();
    markMenu->addAction("&Save file", this, SLOT(saveBookmarksFile()));
    markMenu->addAction("&Save file as...", this, SLOT(saveAsBookmarksFile()));

    QMenu* viewMenu = menuBar()->addMenu("&View");
    viewMenu->addAction("&View as points",    this,       SLOT(viewAsPoints()) );
    viewMenu->addAction("&View as wired",    this,       SLOT(viewAsWired()) );
    viewMenu->addAction("&View as solid",  this,        SLOT(viewAsSolid()) );
    viewMenu->addAction("&View as solid + wired",this,   SLOT(viewAsSolidWire()) );

    QMenu* modeMenu = menuBar()->addMenu("&Mode");
    modeMenu->addAction("&Viewer",    this,             SLOT(viewerMode()) );
    modeMenu->addAction("&Editor",    this,             SLOT(editorMode()) );
    modeMenu->addAction("&Ask Me!",      this,             SLOT(testMode()) );


    // Section list panel buttons.
    QObject::connect(ui->addButton, SIGNAL(clicked()), this, SLOT(showAddSectionPanel()));
    QObject::connect(ui->editButton, SIGNAL(clicked()), this, SLOT(showEditSectionPanel()));
    QObject::connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(deleteCurrentSection()));
    QObject::connect(ui->infoButton, SIGNAL(clicked()), this, SLOT(showInfoSectionPanel()));
    QObject::connect(ui->listWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),
                     this, SLOT(listWidgetItemClicked(QListWidgetItem*)));

    // Add bookmark panel buttons.
    QObject::connect(ui->addModeButton, SIGNAL(clicked()), this, SLOT(setAddSelectionMode()));
    QObject::connect(ui->removeModeButton, SIGNAL(clicked()), this, SLOT(setRemoveSelectionMode()));
    QObject::connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(saveBookmark()));
    QObject::connect(ui->discardButton, SIGNAL(clicked()), this, SLOT(discardBookmark()));
    QObject::connect(ui->brushSizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setBrushSize(int)));

    // Information panel buttons
    QObject::connect(ui->backButton, SIGNAL(clicked()), this, SLOT(showListPanel()));

    // Test panel buttons
    QObject::connect(ui->nextQuestionButton, SIGNAL(clicked()), this, SLOT(nextQuestion()));
    QObject::connect(ui->glwidget, SIGNAL(pickResult(std::set<unsigned int>)), this, SLOT(checkResponse(std::set<unsigned int>)));


    // Set window title.
    setWindowTitle("untitled.txt");

    // Initialize variables.
    _bookmarkList = new BookmarkList();
    clearBookmarkList();
    _model = new Model();
    _indexTest = 0;

    // Set viewer mode.
    viewerMode();

    // Show welcome message
    statusBar()->showMessage("Welcome to 3D Marker.");
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 *  Model management.
 */

void MainWindow::openModel()
{
    QString path = QFileDialog::getOpenFileName( this, tr("Open"), QDir::homePath(), tr(".PLY files (*.ply *.PLY)"), 0 );

    if( !path.isNull() )
    {
        ModelImporter *importer = new PlyImporter();

        if ( importer->import(_model, path) )
        {
            ui->glwidget->setModel( _model );
            statusBar()->showMessage("Model loaded.");     // Show information message.
        }
        else
            statusBar()->showMessage("Error reading file. Please see console for more details");     // Show information message.
    }

}

void MainWindow::closeModel()
{
    closeBookmarksFile();
    _model->clear();
    ui->glwidget->clear();
}

/*
 *  Sections file management.
 */

void MainWindow::openBookmarksFile()
{
    if ( _model->isLoaded() )
    {
        QString filename = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(), tr(".XML files (*.xml *.XML)"), 0);

        if ( !filename.isNull() )
        {
            int errors = 0;
            int maxIndex = _model->numPoly() - 1;

            _bookmarkList->open(filename, maxIndex, &errors);

            // Alert if errors.
            if ( errors > 0 )
            {
                QMessageBox msgBox;
                msgBox.setText("This bookmark file was generated with another model. Invalid references will be omitted.");
                msgBox.exec();
            }

            // Reload list widget.
            ui->listWidget->clear();

            std::vector<Bookmark*>::const_iterator it = _bookmarkList->getList()->begin();
            for ( ; it != _bookmarkList->getList()->end(); ++it )
                ui->listWidget->addItem(new QListWidgetItem((*it)->getName()));

            ui->listWidget->setCurrentRow(0);
            setWindowTitle(filename);
        }
    }
    else
    {
        // Alert: No model loaded.
        QMessageBox msgBox;
        msgBox.setText("No model loaded.");
        msgBox.exec();
    }
}

void MainWindow::closeBookmarksFile()
{
    if ( _bookmarkList->getUpdated() )
    {
        QMessageBox msgBox;
        msgBox.setText("File have unsaved changes");
        msgBox.setInformativeText("Save changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int option = msgBox.exec();

        switch (option) {
            case QMessageBox::Save:
                saveBookmarksFile();
                clearBookmarkList();
                break;
            case QMessageBox::Discard:
                clearBookmarkList();
                break;
           case QMessageBox::Cancel:
               break;
         }
    }
    else
    {
        clearBookmarkList();
    }
}

void MainWindow::saveBookmarksFile()
{
    if ( this->windowTitle() == "untitled.txt" )
    {
        saveAsBookmarksFile();
    }
    else
    {
        _bookmarkList->save(this->windowTitle());    // Save
        statusBar()->showMessage("File saved.");    // Show information message.
    }
}

void MainWindow::saveAsBookmarksFile()
{
    QString filename = QFileDialog::getSaveFileName( this, tr("Save File As"), QDir::homePath(), tr(".XML files (*.xml)"), 0 );

    if( !filename.isNull() )
    {
        _bookmarkList->save(filename);               // Save.
        statusBar()->showMessage("File saved.");    // Show information message.
        setWindowTitle(filename);                   // Set window title.
    }
}

void MainWindow::viewAsPoints()
{
    ui->glwidget->setRenderMode( POINTS );
}

void MainWindow::viewAsWired()
{
    ui->glwidget->setRenderMode( WIRED );
}

void MainWindow::viewAsSolid()
{
    ui->glwidget->setRenderMode( SOLID );
}

void MainWindow::viewAsSolidWire()
{
    ui->glwidget->setRenderMode( SOLID_WIRE );
}

/*
 *  Section list panel methods.
 */

void MainWindow::showListPanel()
{
    ui->glwidget->setViewerMode(ROTATION);
    ui->rightPanel->setCurrentIndex(0);
}

void MainWindow::showAddSectionPanel()
{
    if ( _model->isLoaded() )
    {
        // Unselect mark.
        ui->listWidget->setCurrentRow( 0 );

        // Configure viewer
        ui->glwidget->clearSelection();
        ui->glwidget->setViewerMode(PICK);
        ui->glwidget->setSelectionMode(ADD);
        ui->addModeButton->setChecked(true);

        // Configure panel
        ui->nameLineEdit->clear();
        ui->informationTextEdit->clear();
        ui->rightPanel->setCurrentIndex(1);

        // Configure cursor.
        setCursor(Qt::CrossCursor);
    }
    else
    {
        // Alert: No model loaded.
        QMessageBox msgBox;
        msgBox.setText("No model loaded.");
        msgBox.exec();
    }
}

void MainWindow::showEditSectionPanel()
{
    if ( _model->isLoaded() && ui->listWidget->currentRow() > 0 )
    {
        // Configure viewer
        ui->glwidget->setViewerMode(PICK);
        ui->glwidget->setSelectionMode(ADD);
        ui->addModeButton->setChecked(true);

        // Configure panel
        Bookmark *current = _bookmarkList->getAt( ui->listWidget->currentRow() );
        ui->nameLineEdit->setText(current->getName());
        ui->informationTextEdit->setText(current->getComments());
        ui->rightPanel->setCurrentIndex(1);
    }
    else
    {
        // Alert: No mark selected.
        QMessageBox msgBox;
        msgBox.setText("No mark selected.");
        msgBox.exec();
    }
}

void MainWindow::deleteCurrentSection()
{
    if (ui->listWidget->currentRow() > 0)
    {
        QMessageBox msgBox;
        msgBox.setText("Delete selected mark?");
        msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int option = msgBox.exec();

        if (option == QMessageBox::Ok)
            deleteCurrentBookmark();
    }
}

void MainWindow::showInfoSectionPanel()
{
    if (ui->listWidget->currentRow() == 0)
    {
        // Alert: No information to display.
        QMessageBox msgBox;
        msgBox.setText("No information to display.");
        msgBox.exec();
    }
    else if (ui->listWidget->currentRow() > 0)
    {
        // Configure panel
        Bookmark *bookmark = _bookmarkList->getAt( ui->listWidget->currentRow() );
        ui->nameLabel->setText(bookmark->getName());
        ui->descriptionLabel->setText(bookmark->getComments());
        ui->rightPanel->setCurrentIndex(2);
    }

}

/*
 *  Add bookmark panel buttons
 */

void MainWindow::setAddSelectionMode()
{
    ui->glwidget->setSelectionMode(ADD);
    ui->glwidget->setViewerMode(PICK);
    statusBar()->showMessage("Adding polygons to selection.");         // Show information message.
}

void MainWindow::setRemoveSelectionMode()
{
    ui->glwidget->setSelectionMode(DEL);
    ui->glwidget->setViewerMode(PICK);
    statusBar()->showMessage("Removing polygons to selection.");         // Show information message.
}

void MainWindow::saveBookmark()
{
    std::set<unsigned int> currentSelection = ui->glwidget->getCurrentSelection();
    QString name = ui->nameLineEdit->text();
    QString comments = ui->informationTextEdit->toPlainText();

    if (currentSelection.size() == 0)
    {
        // Alert: Nothing selected.
        QMessageBox msgBox;
        msgBox.setText("No faces selected.");
        msgBox.exec();
    }
    else if (ui->nameLineEdit->text().length() == 0)
    {
        // Alert: No name.
        QMessageBox msgBox;
        msgBox.setText("Please complete name field.");
        msgBox.exec();
    }
    else
    {
        if ( ui->listWidget->currentRow() == 0 ) // Add bookmark
            addBookmark(name, comments, currentSelection);
        else
            updateBookmark(name, comments, currentSelection);

        // Configure cursor.
        setCursor(Qt::ArrowCursor);

        showListPanel();
    }
}

void MainWindow::discardBookmark()
{
    QMessageBox msgBox;
    msgBox.setText("Discard this bookmark?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int option = msgBox.exec();

    if (option == QMessageBox::Ok)
    {
        ui->glwidget->clearSelection();                     // Clear selected faces.
        ui->listWidget->setCurrentRow( 0 );                 // Select first list row.
        statusBar()->showMessage("Section discarded.");     // Show information message.
        setCursor(Qt::ArrowCursor);                         // Configure cursor.
        showListPanel();                                    // Show list panel.
    }
}

void MainWindow::addBookmark(QString name, QString comments, std::set<unsigned int> selection)
{
    Bookmark *bookmark = new Bookmark(name, comments, selection);

    _bookmarkList->add(bookmark);
    ui->listWidget->addItem(new QListWidgetItem(bookmark->getName()));
    ui->listWidget->setCurrentRow(_bookmarkList->size() - 1);

    statusBar()->showMessage("Bookmark saved.");         // Show information message.
}

void MainWindow::updateBookmark(QString name, QString comments, std::set<unsigned int> selection)
{
    // Update bookmark
    Bookmark *bookmark = _bookmarkList->getAt( ui->listWidget->currentRow() );
    bookmark->setName(name);
    bookmark->setComments(comments);
    bookmark->setFaces(selection);

    ui->listWidget->currentItem()->setText(name);       // Update widget.
    statusBar()->showMessage("Bookmark updated.");      // Show information message.
}

void MainWindow::deleteCurrentBookmark()
{
    _bookmarkList->deleteAt(ui->listWidget->currentRow());
    qDeleteAll(ui->listWidget->selectedItems());
}

void MainWindow::listWidgetItemClicked(QListWidgetItem *)
{
    if ( ui->listWidget->currentRow() >= 0 )
    {
        Bookmark *bookmark = _bookmarkList->getAt( ui->listWidget->currentRow() );
        ui->glwidget->setCurrentSelection(bookmark->getFaces());
    }
}

void MainWindow::clearBookmarkList()
{
    setWindowTitle("untitled.txt");

    _bookmarkList->clear();

    ui->listWidget->clear();
    ui->listWidget->addItem(new QListWidgetItem(_bookmarkList->getAt(0)->getName()));
    ui->listWidget->setCurrentRow(0);
}

void MainWindow::setBrushSize(int size)
{
    ui->glwidget->setPickSize(size);
    ui->brushSizeLabel->setText( QString().setNum(size) );
}

void MainWindow::viewerMode()
{
    ui->glwidget->enableHitMode(false);

    ui->addButton->setVisible(false);
    ui->editButton->setVisible(false);
    ui->deleteButton->setVisible(false);
    ui->listWidget->setCurrentRow(0);
    showListPanel();
}

void MainWindow::editorMode()
{
    ui->glwidget->enableHitMode(false);

    ui->addButton->setVisible(true);
    ui->editButton->setVisible(true);
    ui->deleteButton->setVisible(true);
    ui->listWidget->setCurrentRow(0);
    showListPanel();
}

void MainWindow::testMode()
{
    if ( _bookmarkList->size() > 2 )
    {
        ui->glwidget->enableHitMode(true);
        QTime time = QTime::currentTime();
        qsrand((uint)time.msec());
        nextQuestion();
        ui->rightPanel->setCurrentIndex(3);
    }
    else
        statusBar()->showMessage("INFO: Ask Me! mode need two bookmarks or more.");      // Show information message.
}

void MainWindow::nextQuestion()
{
    // Generate a new index.
    unsigned int newIndex;
    do {
        newIndex = qrand() % ((_bookmarkList->size()) - 1) + 1;
    } while (newIndex == _indexTest);
    _indexTest = newIndex;

    // Compose message.
    QString msg = "What is ";
    msg.append( _bookmarkList->getAt(_indexTest)->getName().toLower() );
    msg.append( "?" );

    // Configure UI.
    ui->testBookmarkName->setText( msg );
    ui->successLabel->setVisible(false);
    ui->errorLabel->setVisible(false);
}

void MainWindow::checkResponse(std::set<unsigned int> hits)
{
    std::set<unsigned int>* faces = _bookmarkList->getAt(_indexTest)->getFaces();
    std::set<unsigned int>::const_iterator it;
    for (it = hits.begin(); it != hits.end(); ++it)
    {
        if (faces->find(*it) != faces->end())
        {
            ui->errorLabel->setVisible(false);
            ui->successLabel->setVisible(true);
            return;
        }
    }

    ui->successLabel->setVisible(false);
    ui->errorLabel->setVisible(true);
}
