QT += widgets
CONFIG += c++17 console
CONFIG -= app_bundle
TARGET = ShoppingList
TEMPLATE = app

SOURCES += main.cpp mainwindow.cpp purchase_delegate.cpp
HEADERS += mainwindow.h purchase_delegate.h

msvc:QMAKE_CXXFLAGS += /utf-8
