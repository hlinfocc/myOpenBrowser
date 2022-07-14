/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#ifndef DOWNLOADMANAGERWIDGET_H
#define DOWNLOADMANAGERWIDGET_H

#include "ui_downloadmanagerwidget.h"

#include <QWidget>

QT_BEGIN_NAMESPACE
class QWebEngineDownloadItem;
QT_END_NAMESPACE

class DownloadWidget;

// Displays a list of downloads.
class DownloadManagerWidget final : public QWidget, public Ui::DownloadManagerWidget
{
    Q_OBJECT
public:
    explicit DownloadManagerWidget(QWidget *parent = nullptr);

    // Prompts user with a "Save As" dialog. If the user doesn't cancel it, then
    // the QWebEngineDownloadItem will be accepted and the DownloadManagerWidget
    // will be shown on the screen.
    void downloadRequested(QWebEngineDownloadItem *webItem);

private:
    void add(DownloadWidget *downloadWidget);
    void remove(DownloadWidget *downloadWidget);

    int m_numDownloads;
};

#endif // DOWNLOADMANAGERWIDGET_H
