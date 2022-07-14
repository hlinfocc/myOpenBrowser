/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QWebEnginePage>

QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

class WebView;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWebEngineProfile *profile, QWidget *parent = nullptr);

    WebView *currentWebView() const;

signals:
    // current tab/page signals
    void linkHovered(const QString &link);
    void loadProgress(int progress);
    void titleChanged(const QString &title);
    void urlChanged(const QUrl &url);
    void favIconChanged(const QIcon &icon);
    void webActionEnabledChanged(QWebEnginePage::WebAction action, bool enabled);
    void devToolsRequested(QWebEnginePage *source);
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    void findTextFinished(const QWebEngineFindTextResult &result);
#endif

public slots:
    // current tab/page slots
    void setUrl(const QUrl &url);
    void triggerWebPageAction(QWebEnginePage::WebAction action);

    WebView *createTab();
    WebView *createBackgroundTab();
    void closeTab(int index);
    void nextTab();
    void previousTab();
    void printPreview();
    void printToPDF();

private slots:
    void handleCurrentChanged(int index);
    void handleContextMenuRequested(const QPoint &pos);
    void cloneTab(int index);
    void closeOtherTabs(int index);
    void reloadAllTabs();
    void reloadTab(int index);

private:
    WebView *webView(int index) const;
    void setupView(WebView *webView);

    QWebEngineProfile *m_profile;
};

#endif // TABWIDGET_H
