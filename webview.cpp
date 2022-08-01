/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/

#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include "webpage.h"
#include "webpopupwindow.h"
#include "webview.h"
#include <QContextMenuEvent>
#include <QDebug>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QAction>
#include <QFileDialog>

/**
 * 标签页面
 * @brief WebView::WebView
 * @param parent
 */
WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
    , m_loadProgress(100)
{
    connect(this, &QWebEngineView::loadStarted, [this]() {
        m_loadProgress = 0;
        emit favIconChanged(favIcon());
    });
    connect(this, &QWebEngineView::loadProgress, [this](int progress) {
        m_loadProgress = progress;
    });
    connect(this, &QWebEngineView::loadFinished, [this](bool success) {
        m_loadProgress = success ? 100 : -1;
        emit favIconChanged(favIcon());
    });
    connect(this, &QWebEngineView::iconChanged, [this](const QIcon &) {
        emit favIconChanged(favIcon());
    });

    connect(this, &QWebEngineView::renderProcessTerminated,
            [this](QWebEnginePage::RenderProcessTerminationStatus termStatus, int statusCode) {
        QString status;
        switch (termStatus) {
        case QWebEnginePage::NormalTerminationStatus:
            status = tr("Render process normal exit");
            break;
        case QWebEnginePage::AbnormalTerminationStatus:
            status = tr("Render process abnormal exit");
            break;
        case QWebEnginePage::CrashedTerminationStatus:
            status = tr("Render process crashed");
            break;
        case QWebEnginePage::KilledTerminationStatus:
            status = tr("Render process killed");
            break;
        }
        QMessageBox box(QMessageBox::Warning,status,tr("Render process exited with code: %1 ,Do you want to reload the page ?").arg(statusCode));
            box.setStandardButtons(QMessageBox::Cancel|QMessageBox::Ok);
            box.setButtonText(QMessageBox::Ok,QString(tr("Yes")));
            box.setButtonText(QMessageBox::Cancel,QString(tr("Cancel")));
        int res = box.exec ();
        if (res == QMessageBox::Ok) {
            QTimer::singleShot(0, [this] { reload(); });
        }

    });

}
/**
 * 页面设置
 * @brief WebView::setPage
 * @param page
 */
void WebView::setPage(WebPage *page)
{
    //设置页面右键菜单
    createWebActionTrigger(page,QWebEnginePage::Forward,tr("Forward"));//前进
    createWebActionTrigger(page,QWebEnginePage::Back,tr("Back"));//后退
    createWebActionTrigger(page,QWebEnginePage::Reload,tr("Reload"));//刷新
    createWebActionTrigger(page,QWebEnginePage::Stop,tr("Stop"));//停止
    createWebActionTrigger(page,QWebEnginePage::SavePage,tr("SavePage"));//保存页面
    createWebActionTrigger(page,QWebEnginePage::Copy,tr("Copy"));//复制
    createWebActionTrigger(page,QWebEnginePage::SelectAll,tr("SelectAll"));//全选
    createWebActionTrigger(page,QWebEnginePage::OpenLinkInNewTab,tr("OpenLinkInNewTab"));//在新标签打开链接
    createWebActionTrigger(page,QWebEnginePage::OpenLinkInNewWindow,tr("OpenLinkInNewWindow"));//在新窗口打开链接
    createWebActionTrigger(page,QWebEnginePage::CopyImageUrlToClipboard,tr("CopyImageUrlToClipboard"));//复制图片地址
    createWebActionTrigger(page,QWebEnginePage::CopyImageToClipboard,tr("CopyImageToClipboard"));//复制图片
    createWebActionTrigger(page,QWebEnginePage::DownloadImageToDisk,tr("DownloadImageToDisk"));//下载图片
    createWebActionTrigger(page,QWebEnginePage::CopyLinkToClipboard,tr("CopyLinkToClipboard"));//复制链接
    createWebActionTrigger(page,QWebEnginePage::DownloadLinkToDisk,tr("DownloadLinkToDisk"));//链接页面另存为
    createWebActionTrigger(page,QWebEnginePage::ViewSource,tr("ViewSource"));//查看源代码
    QWebEngineView::setPage(page);
}

int WebView::loadProgress() const
{
    return m_loadProgress;
}
/**
 * 页面触发
 * @brief WebView::createWebActionTrigger
 * @param page
 * @param webAction
 * @param name
 */
void WebView::createWebActionTrigger(QWebEnginePage *page, QWebEnginePage::WebAction webAction,const QString &name)
{
    QAction *action = page->action(webAction);
    action->setText(name);
    connect(action, &QAction::changed, [this, action, webAction]{
        emit webActionEnabledChanged(webAction, action->isEnabled());
    });
}

bool WebView::isWebActionEnabled(QWebEnginePage::WebAction webAction) const
{
    return page()->action(webAction)->isEnabled();
}
/**
 * 地址栏图标处理
 * @brief WebView::favIcon
 * @return
 */
QIcon WebView::favIcon() const
{
    QIcon favIcon = icon();
    if (!favIcon.isNull())
        return favIcon;

    if (m_loadProgress < 0) {
        static QIcon errorIcon(QStringLiteral(":dialog-error.png"));
        return errorIcon;
    } else if (m_loadProgress < 100) {
        static QIcon loadingIcon(QStringLiteral(":view-refresh.png"));
        return loadingIcon;
    } else {
        static QIcon defaultIcon(QStringLiteral(":text-html.png"));
        return defaultIcon;
    }
}

/**
 * 创建窗口
 * @brief WebView::createWindow
 * @param type
 * @return
 */
QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    BrowserWindow *mainWindow = qobject_cast<BrowserWindow*>(window());
    if (!mainWindow)
        return nullptr;

    switch (type) {
    case QWebEnginePage::WebBrowserTab: {
        return mainWindow->tabWidget()->createTab();
    }
    case QWebEnginePage::WebBrowserBackgroundTab: {
        return mainWindow->tabWidget()->createBackgroundTab();
    }
    case QWebEnginePage::WebBrowserWindow: {
        return mainWindow->browser()->createWindow()->currentTab();
    }
    case QWebEnginePage::WebDialog: {
        WebPopupWindow *popup = new WebPopupWindow(page()->profile());
        connect(popup->view(), &WebView::devToolsRequested, this, &WebView::devToolsRequested);
        return popup->view();
    }
    }
    return nullptr;
}
/**
 * 页面右键菜单处理，如检查元素（打开浏览器控制台）
 * @brief WebView::contextMenuEvent
 * @param event
 */
void WebView::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu = page()->createStandardContextMenu();
    const QList<QAction *> actions = menu->actions();
    auto inspectElement = std::find(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::InspectElement));
    if (inspectElement == actions.cend()) {
        auto viewSource = std::find(actions.cbegin(), actions.cend(), page()->action(QWebEnginePage::ViewSource));

        if (viewSource == actions.cend())
            menu->addSeparator();

        QAction *action = new QAction(menu);
        //在新窗口打开检查
        action->setText(tr("Open inspector in new window"));
        connect(action, &QAction::triggered, [this]() { emit devToolsRequested(page()); });

        QAction *before(inspectElement == actions.cend() ? nullptr : *inspectElement);
        menu->insertAction(before, action);
    } else {
        (*inspectElement)->setText(tr("Inspect element"));
    }
    menu->popup(event->globalPos());
}


