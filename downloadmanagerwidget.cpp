/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#include "downloadmanagerwidget.h"

#include "browser.h"
#include "browserwindow.h"
#include "downloadwidget.h"

#include <QFileDialog>
#include <QDir>
#include <QWebEngineDownloadItem>

DownloadManagerWidget::DownloadManagerWidget(QWidget *parent)
    : QWidget(parent)
    , m_numDownloads(0)
{
    setupUi(this);
}

void DownloadManagerWidget::downloadRequested(QWebEngineDownloadItem *download)
{
    Q_ASSERT(download && download->state() == QWebEngineDownloadItem::DownloadRequested);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    QString path = QFileDialog::getSaveFileName(this, tr("Save as"), QDir(download->downloadDirectory()).filePath(download->downloadFileName()));
#else
    QString path = QFileDialog::getSaveFileName(this, tr("Save as"), download->path());
#endif
    if (path.isEmpty())
        return;

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    download->setDownloadDirectory(QFileInfo(path).path());
    download->setDownloadFileName(QFileInfo(path).fileName());
#else
    download->setPath(path);
#endif
    download->accept();
    add(new DownloadWidget(download));
    show();
}

void DownloadManagerWidget::add(DownloadWidget *downloadWidget)
{
    connect(downloadWidget, &DownloadWidget::removeClicked, this, &DownloadManagerWidget::remove);
    m_itemsLayout->insertWidget(0, downloadWidget, 0, Qt::AlignTop);
    if (m_numDownloads++ == 0)
        m_zeroItemsLabel->hide();
}

void DownloadManagerWidget::remove(DownloadWidget *downloadWidget)
{
    m_itemsLayout->removeWidget(downloadWidget);
    downloadWidget->deleteLater();
    if (--m_numDownloads == 0)
        m_zeroItemsLabel->show();
}
