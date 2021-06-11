include($$QTWEBENGINE_OUT_ROOT/src/core/qtwebenginecore-config.pri) # workaround for QTBUG-68093
QT_FOR_CONFIG += webenginecore webenginecore-private

TEMPLATE = app
TARGET = myOpenBrowser
QT += webenginewidgets printsupport

HEADERS += \
    browser.h \
    browserwindow.h \
    downloadmanagerwidget.h \
    downloadwidget.h \
    tabwidget.h \
    webpage.h \
    webpopupwindow.h \
    webview.h \
    printhandler.h

SOURCES += \
    browser.cpp \
    browserwindow.cpp \
    downloadmanagerwidget.cpp \
    downloadwidget.cpp \
    main.cpp \
    tabwidget.cpp \
    webpage.cpp \
    webpopupwindow.cpp \
    webview.cpp \
    printhandler.cpp

FORMS += \
    certificateerrordialog.ui \
    passworddialog.ui \
    downloadmanagerwidget.ui \
    downloadwidget.ui

RESOURCES += \
    data/myOpenBrowser.qrc \
    i18n/language.qrc

TRANSLATIONS += \
    i18n/en.ts \
    i18n/zh_CN.ts

RC_ICONS = data/AppLogoColor.ico

# install
target.path = $$[QT_INSTALL_EXAMPLES]/myOpenBrowser
INSTALLS += target
