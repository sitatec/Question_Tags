#include "include/window.hpp"
#include "include/update.hpp"

static const char* CRITICAL_ERROR_MESSAGE = "A critical error has occurred. Please try again, if the "
                                            "problem persists restart the software.<br> Error message:<br>"
                                            "<span style=\"color: red;\">%1</span>";

static const char* MULTIPLE_GENERATION_STEPS = "This feature allows you to generate multiple question tags "
                                                "at a time.<br> To do this you must: <ol><li>put all your sentences "
                                                "in a text file, one sentence per line (otherwise the software "
                                                "will only consider the first sentence on the same line).</li><br>"
                                                "<li>Click on the button select a file below then select the text "
                                                "file that contains the sentences.</li><br><li>Then a dialog box will "
                                                "appear. Select the location where the file containing the question"
                                                " tags will be saved.</li><br><li>Then Click on the generate button"
                                                " which will appear below.</li></ol>";


Window::Window(QWidget *parent) : QWidget(parent)
{
    setFixedSize(800, 600);

    setWindowTitle("Question Tags(Beta Version)");
    setWindowIcon(QIcon(":/ressources/images/logo.icns"));

    loadPalettes();
    setPalette(_initialPalette);

    QTimer *timer = new QTimer(this );
    timer->setSingleShot(true);
    connect( timer, SIGNAL( timeout() ), this, SLOT( loadWidgets() ) );
    timer->start(3200);

    connect(this, SIGNAL( enterKeyPressed() ), this, SLOT( generate() ));
}



void Window::loadPalettes()
{
    _backgroundImg   = QPixmap(":/ressources/images/bg_img.png");
    _backgroundImg   = _backgroundImg.scaled(this->size(), Qt::IgnoreAspectRatio);

    _backgroundImg2  = QPixmap(":/ressources/images/bg_img2.png");
    _backgroundImg2  = _backgroundImg2.scaled(this->size(), Qt::IgnoreAspectRatio);

    _initialPalette.setBrush(QPalette::Background, _backgroundImg );
    _palette.setBrush(QPalette::Background, _backgroundImg2 );
}



void Window::loadWidgets()
{
    _mainLayout =  new  QHBoxLayout;
    _tab        =  new  QTabWidget;

    loadLeftMenu();
    loadGeneratorWidgets();
    loadMultipleGeneratorWidgets();

    _tab->addTab( _generatorWidget,tr("Generator") );
    _tab->addTab( _listGeneratorWidget,tr("List Generator") );
    /*_tab->addTab(&_quizWidget,     tr("Quiz"));
    _tab->addTab(&_courseWidget,   tr("Course"));
    _tab->addTab(&_faqWidget,      tr("Faq"));*/

    _mainLayout->addWidget(_leftMenu);
    _mainLayout->addWidget(_tab);

    setPalette(_palette);
    setLayout(_mainLayout);

    autoUpdateCheck();
}



void Window::loadLeftMenu()
{
    int const GENERATOR_BUTTON = 0;
    int const LIST_GENERATOR_BUTTON = 1;
    int const UPDATE_BUTTON = 2;
    int const QUIT_BUTTON = 3;

    _leftLayout =  new  QVBoxLayout;
    _leftMenu   =  new  QWidget;

    _menus.push_back( new QPushButton( tr(  "&Generator"   ) ));
    _menus.push_back( new QPushButton( tr("&List Generator") ));
    _menus.push_back( new QPushButton( tr(    "&Update"    ) ));
    _menus.push_back( new QPushButton( tr(     "&Quit"     ) ));

 // _menus.push_back( new QPushButton( tr("Qui&z1"   ) ));
 // _menus.push_back( new QPushButton( tr("&Learn1"  ) ));

    for (unsigned long i = 0; i < _menus.size() ; i++) {
        _leftLayout->addWidget( _menus[ i ] );
        _menus[ i ]->setStyleSheet(" font-size: 14px; text-align: left;");
        _menus[ i ]->setCursor(Qt::PointingHandCursor);
    }

    _leftLayout->addSpacing(450);
    _leftMenu->setLayout(_leftLayout );
    _leftMenu->setStyleSheet("background-color:#f3f3f4;  border-radius: 5px;");

    connect( _menus[ LIST_GENERATOR_BUTTON ], SIGNAL( clicked() ), this, SLOT( switchToListGenerator() ));
    connect( _menus[    GENERATOR_BUTTON   ], SIGNAL( clicked() ), this, SLOT(   switchToGenerator()   ));
    connect( _menus[     UPDATE_BUTTON     ], SIGNAL( clicked() ), this, SLOT(     checkForUpdate()    ));
    connect( _menus[      QUIT_BUTTON      ], SIGNAL( clicked() ), this, SLOT(         quitApp()       ));
}



void Window::loadGeneratorWidgets()
{
    _generatorLayout  =  new  QVBoxLayout;
    _generatorWidget  =  new  QWidget;
    _generateButton   =  new  QPushButton;
    _textInputLayout  =  new  QHBoxLayout;
    _buttonLayout     =  new  QHBoxLayout;
    _textInput        =  new  QLineEdit;
    _arrowImg         =  new  QLabel;
    _infoArea         =  new  QLabel;
    _questionTagLabel =  new  QLabel;


    _generateButton->setText(tr("Générer"));
    _generateButton->setCursor(Qt::PointingHandCursor);
    _generateButton->setToolTip(tr("Click to generate the question tag of the entered sentence"));
    _generateButton->setStyleSheet("border-radius: 0px; \
                                    padding: 3px; \
                                    background-color: #66e6eb; \
                                    font-size: 15px;");

    _buttonLayout->addSpacing(230);
    _buttonLayout->addWidget(_generateButton);
    _buttonLayout->addSpacing(230);

    _questionTagLabel->setStyleSheet(" font-size: 15px; background-color: white; padding: 2px;");
    _questionTagLabel->setFixedHeight(25);
    _questionTagLabel->setText("                   ? ");
    _questionTagLabel->setCursor(Qt::IBeamCursor);
    _questionTagLabel->setToolTip(tr("The question tag corresponding to the entered sentence will be displayed here"));

    _textInput->setPlaceholderText(tr("Saisiser une phrase ici"));
    _textInput->setStyleSheet("border-radius: 0px; \
                               padding: 2px; \
                               font-size: 14px; ");

    _arrowImg->setPixmap(QPixmap(":/ressources/images/arrow1.png"));

    _textInputLayout->addSpacing( 65 );
    _textInputLayout->addWidget( _textInput );
    _textInputLayout->addWidget( _arrowImg );
    _textInputLayout->addWidget( _questionTagLabel );
    _textInputLayout->addSpacing( 65 );

    _infoArea->setWordWrap(true);
    _infoArea->setAlignment(Qt::AlignTop);
    _infoArea->setStyleSheet( "border-radius: 10px; \
                               background-color: white; \
                               margin-top: 30px; " );

    _generatorLayout->addSpacing( 13 );
    _generatorLayout->addLayout( _textInputLayout );
    _generatorLayout->addLayout( _buttonLayout );
    _generatorLayout->addWidget( _infoArea );
    _generatorLayout->addSpacing( 130 );

    _generatorWidget->setLayout( _generatorLayout );

    connect( _generateButton, SIGNAL( clicked() ),this, SLOT( generate() ) );
}



void Window::loadMultipleGeneratorWidgets()
{
    _listGeneratorWidget =  new  QWidget;
    _listGeneratorLayout =  new  QVBoxLayout;
    _listGeneratorLable  =  new  QLabel;
    _selectFileButton    =  new  QPushButton(tr("&Select a File"));

    _selectFileButton->setFixedWidth( 120 );

    _listGeneratorLable->setWordWrap( true );
    _listGeneratorLable->setText(tr( MULTIPLE_GENERATION_STEPS ));

    _listGeneratorLayout->addWidget( _listGeneratorLable, 0, Qt::AlignTop  );
    _listGeneratorLayout->addWidget(  _selectFileButton, 0, Qt::AlignHCenter );
    _listGeneratorLayout->addSpacing( 120 );

    _listGeneratorWidget->setLayout( _listGeneratorLayout );

    connect( _selectFileButton, SIGNAL( clicked() ), this, SLOT( selectFile() ));
    connect( this, SIGNAL( fileHasBeenSelected() ), this, SLOT( displayGenerateButton() ) );
}



void Window::autoUpdateCheck()
{
    std::string appPath = qApp->applicationDirPath().toStdString();
    auto dotPos = appPath.find_last_of(".");// find the dot position in "applicationName.app"
    QString filePath = appPath.erase( dotPos ).c_str();

    QFile file( filePath + ".app/Contents/Resources/auto_update_date.txt" );
    if( file.open( QIODevice::ReadWrite | QIODevice::Text ) ){
        auto autoUpdateDateStr = file.readAll();
        QDate nextAutoUpdateCheck = QDate::currentDate().addDays( 14 );
        if( autoUpdateDateStr.isEmpty() ){
            file.write( nextAutoUpdateCheck.toString().toLocal8Bit() );
            checkForUpdate( false );// False means it is not a manual check
        }
        else {
            QDate autoUpdateDate = QDate::fromString( autoUpdateDateStr );
            if( autoUpdateDate <= QDate::currentDate() ){
                checkForUpdate( false );// False means it is not a manual check
                file.resize( 0 );
                file.write( nextAutoUpdateCheck.toString().toLocal8Bit() );
            }
        }
    }
}



void Window::checkForUpdate( bool isManualUpdateCheck )
{
    Update *update = new Update( this );
    update->checkForUpdate();
    if( isManualUpdateCheck )
        connect( update, SIGNAL( softwareUpToDate() ), update, SLOT( softwareUpToDateSlot() ) );
}




/*****************************************************************************
                                 SLOTS
 *****************************************************************************/



void Window::generate()
{
    if( _textInput->text().isEmpty() ){
        showErrorMsg("You must enter a sentence to generate a tag question");
    }
    else if( tool::trim(_textInput->text().toStdString() )  == _previousSentence ){
        // If the previous sentence is identical to the current sentence we don't change the result
    }
    else {
        preventSpanning();// To prevent the user from spamming the generate button

        std::string entredSentence = tool::trim( _textInput->text().toStdString() );
        std::string questionTag{""};
        QString explanation{""}, answer{""}, warnings{""};
        bool exceptionsThrown{ false };

        tool::removeEndDot( entredSentence );

        try {
            Sentence sentence( entredSentence );
            questionTag = sentence.getQuestionTag();
            explanation = sentence.getExplanation().c_str();
            answer  = entredSentence.c_str();// The answer must first contain the sentence entered
                                            // by the user to get something like "sentence, question tag"
            warnings = sentence.getWarnings().c_str();
        }
        catch (Error const& e) {
            exceptionsThrown = true;
            handleError( e );
        }
        catch( std::exception const& e ){
            exceptionsThrown = true;
            _infoArea->setText(tr( CRITICAL_ERROR_MESSAGE ).arg( e.what() ));
        }

        if( !exceptionsThrown ) {
            showResults( questionTag, answer, explanation, warnings );
        }
        _previousSentence = entredSentence;
    }
}



void Window::selectFile()
{
    _sentencesFilePath = QFileDialog::getOpenFileName(this, tr("Select File"), QStandardPaths::displayName(
                                                      QStandardPaths::DocumentsLocation ),
                                                      tr("Text File (*.txt *.text)"));
    if( !_sentencesFilePath.isEmpty() ) emit fileHasBeenSelected();
}



void Window::displayGenerateButton()
{
    _generateMultipleButton = new QPushButton(tr("&Generate"));
    _generateMultipleButton->setFixedWidth( 120 );

    _listGeneratorLayout->replaceWidget( _selectFileButton, _generateMultipleButton );

    connect( _generateMultipleButton, SIGNAL( clicked() ), SLOT( generateMultiple() ));
}



void Window::generateMultiple()
{
    _listGeneratorLayout->replaceWidget( _generateMultipleButton, _selectFileButton );

    QFile sentencesFile( _sentencesFilePath );
    if ( !sentencesFile.open( QIODevice::ReadOnly | QIODevice::Text )) {
        QMessageBox::critical( this, tr("Error"),
                               tr("An error occurred while opening the file, please try again") );
    }
    else {
        int errors{0};
        QVector <std::string> questionTags;
        Sentence sentence;
        while ( !sentencesFile.atEnd() ) {
            std::string line = sentencesFile.readLine().toStdString();
            if( line.empty() ) continue;
            tool::removeEndDot( line );
            try {
                questionTags.push_back( line + ", " + sentence.createQuestionTags( line ) );
            } catch ( Error const& e ) {
                auto exceptionType = e.getType();
                if( exceptionType == Error::DICTIONARY_CAN_BE_OPEN ){
                    QMessageBox::critical(this,tr("Error"),tr( CRITICAL_ERROR_MESSAGE ).arg( e.what() ));
                    break;
                }
                else{
                    ++errors;
                    continue;
                }
            }
            catch ( std::exception const& e ) {
                QMessageBox::critical(this,tr("Error"), tr( CRITICAL_ERROR_MESSAGE ).arg( e.what() ));
                break;
            }
        }
        auto lastSlash = _sentencesFilePath.lastIndexOf('/');
        auto sentensesFileDir = _sentencesFilePath.remove( lastSlash, _sentencesFilePath.size() );

        _questionTagsFilePath = QFileDialog::getSaveFileName( this, tr("Save File"), sentensesFileDir,
                                                              tr("Text File (*.txt *.text)"));
        QFile questionTagsFile( _questionTagsFilePath );
        if( !questionTagsFile.open( QIODevice::WriteOnly | QIODevice::Text ) ){
            QMessageBox::critical( this, tr("Error"),
                                   tr("An error occurred while saving the file, please try again"
                                      ", if the problem persists change the file directory.") );
        }
        else {
            for (int i = 0, j = questionTags.size(); i < j ; i++ ) {
                questionTagsFile.write( questionTags[ i ].append("\n").c_str() );
            }
            QMessageBox::information(this, tr("Success"),
                                     tr("list of question tags successfully generated and saved."
                                        " %n error(s) has(are) occurred while generating. You can"
                                        " find your question tags list at :<br> <i>%1</i>", "", errors)
                                     .arg( _questionTagsFilePath ) );
        }
    }
}



void Window::activeGenerateButton()
{
    _generateButton->setDisabled(false);
}


void Window::quitApp()
{
    connect(this, SIGNAL( exitConfirmed() ), qApp, SLOT( quit() ));

    auto response = QMessageBox::question(this, tr("Closing"),
                                          tr("Are you sure you want to quit the software?"),
                                          QMessageBox::No |  QMessageBox::Yes, QMessageBox::No );
    if( response == QMessageBox::Yes ){
        emit exitConfirmed();
    }
}



void Window::switchToGenerator()
{
    _tab->setCurrentWidget( _generatorWidget );
}



void Window::switchToListGenerator()
{
    _tab->setCurrentWidget( _listGeneratorWidget );
}



void Window::KeyPressEvent(QKeyEvent *event)
{
    qDebug()<<"key pressed\n";
    if(event->key() == Qt::Key_Enter){
        qDebug()<<"Enter key pressed\n";
        emit enterKeyPressed();
    }
}



/*****************************************************************************
                            FUNCTIONS USED IN SLOTS
 *****************************************************************************/



void Window::showErrorMsg( const char* errorMsg )
{
    _questionTagLabel->setText("                   ? ");
    _infoArea->setText( tr("<h3 style=\"text-align: center; color: red; \">Error</h3> %1 ").arg( errorMsg ));
    _infoArea->setStyleSheet(" font-size: 18;"
                             " margin-top: 30px;"
                             " background-color: white;"
                             " border-radius: 10px;");
}



void Window::handleError( Error const& e )
{
    auto exceptionType = e.getType();
    if( exceptionType == Error::EMPTY_SENTENCE ){
        showErrorMsg("You must enter a sentence to generate a tag question");
    }
    else if( exceptionType == Error::VERB_NOT_FOUND ){
        showErrorMsg( e.what() );
    }
    else if( exceptionType == Error::EMPTY_QUESTION_TAG ){
        showErrorMsg( e.what() );
    }
    else if( exceptionType == Error::DICTIONARY_CAN_BE_OPEN ){
        showErrorMsg( e.what() );
    }
}



void Window::showResults( std::string questionTag, QString answer, QString explanation, QString warnings )
{
    answer += ", <i style=\"color: green;\">";
    answer += questionTag.c_str();
    answer += "</i>";

    if( !warnings.isEmpty() )
    {
        warnings = "<br><h6 style=\"color: orange;\"> Warning! </h6>" + warnings ;
    }

    _questionTagLabel->setText( questionTag.c_str() );
    _infoArea->setText(tr("<h6 style=\"text-align: center; font-size: 15px;\" > %1 </h6>"
                          "%2 %3").arg( answer, explanation, warnings));
    _infoArea->setStyleSheet("color: black; "
                             "border-radius: 10px;"
                             "margin-top: 30px;"
                             "font-size: 13 px; "
                             "background-color: white;");
    _infoArea->setAlignment(Qt::AlignTop);
}



void Window::preventSpanning()
{
    _generateButton->setDisabled( true );

    QTimer *timer = new QTimer;
    timer->setSingleShot( true );
    connect(timer, SIGNAL( timeout() ),this , SLOT( activeGenerateButton() ));
    timer->start( 1000 );
}
