SOURCES += \
    src/main.cpp \
    src/sentence.cpp \
    src/error.cpp \
    src/tool_kit.cpp \
    src/update.cpp \
    src/window.cpp

HEADERS += \
    include/sentence.hpp \
    include/error.hpp \
    include/tool_kit.hpp \
    include/update.hpp \
    include/window.hpp \
    include/word.hpp

QT += widgets network

CONFIG += c++17

RESOURCES += \
    dictionaries.qrc \
    images.qrc

VERSION_MAJOR = 0
VERSION_MINOR = 1

DEFINES += "VERSION_MAJOR=$$VERSION_MAJOR"\
           "VERSION_MINOR=$$VERSION_MINOR"\

VERSION = $${VERSION_MAJOR}.$${VERSION_MINOR}

ICON = /Users/sitaberete/Projets/Question_Tags/ressources/images/logo.icns
