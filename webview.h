/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QIcon>
#include <QWebEngineView>

class WebPage;
class WebView : public QWebEngineView
{
    Q_OBJECT

public:
    WebView(QWidget *parent = nullptr);
    void setPage(WebPage *page);

    int loadProgress() const;
    bool isWebActionEnabled(QWebEnginePage::WebAction webAction) const;
    QIcon favIcon() const;

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

signals:
    void webActionEnabledChanged(QWebEnginePage::WebAction webAction, bool enabled);
    void favIconChanged(const QIcon &icon);
    void devToolsRequested(QWebEnginePage *source);

private:
    void createWebActionTrigger(QWebEnginePage *page, QWebEnginePage::WebAction,const QString &name);

private:
    int m_loadProgress;
};

#endif
