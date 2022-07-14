/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/

#include "browser.h"
#include "browserwindow.h"

Browser::Browser()
{
    // Quit application if the download manager window is the only remaining window
    m_downloadManagerWidget.setAttribute(Qt::WA_QuitOnClose, false);

    QObject::connect(
        QWebEngineProfile::defaultProfile(), &QWebEngineProfile::downloadRequested,
        &m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);
}

BrowserWindow *Browser::createWindow(bool offTheRecord)
{
    if (offTheRecord && !m_otrProfile) {
        m_otrProfile.reset(new QWebEngineProfile);
        QObject::connect(
            m_otrProfile.get(), &QWebEngineProfile::downloadRequested,
            &m_downloadManagerWidget, &DownloadManagerWidget::downloadRequested);
    }
    auto profile = offTheRecord ? m_otrProfile.get() : QWebEngineProfile::defaultProfile();
    auto mainWindow = new BrowserWindow(this, profile, false);
    m_windows.append(mainWindow);
    QObject::connect(mainWindow, &QObject::destroyed, [this, mainWindow]() {
        m_windows.removeOne(mainWindow);
    });
    mainWindow->show();
    return mainWindow;
}

BrowserWindow *Browser::createDevToolsWindow()
{
    auto profile = QWebEngineProfile::defaultProfile();
    auto mainWindow = new BrowserWindow(this, profile, true);
    m_windows.append(mainWindow);
    QObject::connect(mainWindow, &QObject::destroyed, [this, mainWindow]() {
        m_windows.removeOne(mainWindow);
    });
    mainWindow->show();
    return mainWindow;
}
