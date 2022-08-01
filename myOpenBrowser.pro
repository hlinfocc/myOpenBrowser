TEMPLATE = app
TARGET = myOpenBrowser
QT += core gui axcontainer webenginewidgets network webchannel printsupport

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
    res/myOpenBrowser.qrc \
    i18n/language.qrc

TRANSLATIONS += \
    i18n/en.ts \
    i18n/zh_CN.ts

RC_ICONS = res/AppLogo.ico

# install
target.path = $$[QT_INSTALL_EXAMPLES]/myOpenBrowser
INSTALLS += target

DESTDIR = ./dist

#windows属性信息资源配置文件(文件编码需为ANSI)
RC_FILE = ./res/win_res.rc

DISTFILES +=
