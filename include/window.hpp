#pragma once

//#define UNIX_KERNEL
#define WINDOWS

#include <QtWidgets>
#include <QKeyEvent>
#include "include/sentence.hpp"

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

signals:
     void exitConfirmed();
     void enterKeyPressed();
     void fileHasBeenSelected();

protected:
    void KeyPressEvent(QKeyEvent *event);

private slots:
     void loadWidgets();
     void generate();
     void activeGenerateButton();
     void quitApp();
     void selectFile();
     void generateMultiple();
     void displayGenerateButton();
     void checkForUpdate( bool isTheManualUpdateCheck = true );
     void switchToGenerator();
     void switchToListGenerator();

private:

     QHBoxLayout *_mainLayout,     *_buttonLayout,     *_textInputLayout;
     QVBoxLayout *_leftLayout,     *_generatorLayout,  *_listGeneratorLayout;

     QWidget     *_leftMenu,       *_generatorWidget,  *_listGeneratorWidget;

     QLineEdit   *_textInput;
     QPushButton *_generateButton, *_selectFileButton, *_generateMultipleButton;
     QTabWidget  *_tab;

     QLabel      *_infoArea,       *_arrowImg,         *_listGeneratorLable,
                 *_questionTagLabel;

     QPalette    _initialPalette,   _palette;
     QPixmap     _backgroundImg,    _backgroundImg2;

     std::vector <QPushButton*>     _menus;

     std::string _previousSentence;

     QString     _sentencesFilePath, _questionTagsFilePath;

     void loadMultipleGeneratorWidgets();
     void loadGeneratorWidgets();
     void autoUpdateCheck();
     void loadLeftMenu();
     void loadPalettes();
     void preventSpanning();
     void showResults( std::string questionTag, QString answer, QString explanation, QString warnings );
     void showErrorMsg( const char* errorMsg );
     void handleError( Error const& e );
};

