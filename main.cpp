/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/

#include "browser.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include <QApplication>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QScreen>
#include <QTranslator>

QUrl commandLineUrlArgument()
{
    const QStringList args = QCoreApplication::arguments();
    for (const QString &arg : args.mid(1)) {
        if (!arg.startsWith(QLatin1Char('-')))
            return QUrl::fromUserInput(arg);
    }
    //入口地址
    return QUrl(QStringLiteral("https://hlinfo.cc"));
}

int main(int argc, char **argv)
{
    QCoreApplication::setOrganizationName("HlinfoBrowser");
    QCoreApplication::setOrganizationDomain("hlinfo.net");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
    //支持老显卡设置
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
    QGuiApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QCoreApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);
    QApplication::setAttribute(Qt::AA_UseSoftwareOpenGL);

    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(QStringLiteral(":AppLogo.png")));
    //设置语言
    QLocale locale;
    QTranslator *translator = new QTranslator();
    if(locale.language() == QLocale::Chinese){
        translator->load(QString(":/zh_CN.qm"));
        app.installTranslator(translator);
    }
    //启用插件
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    //启用PDF预览
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::PdfViewerEnabled, true);
    //启用LocalStorage
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalStorageEnabled,true);
    //启用允许JavaScript程序读取和写入剪贴板
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::JavascriptCanAccessClipboard,true);
    //true允许本地加载的文档忽略跨源规则
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls,false);
    //启用全屏支持
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,true);
    //自动加载图片
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::AutoLoadImages,true);
    //启用web GL
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::WebGLEnabled,true);
#if QT_VERSION >= QT_VERSION_CHECK(5, 13, 0)
    //启用DNS预加载
    QWebEngineSettings::defaultSettings()->setAttribute(QWebEngineSettings::DnsPrefetchEnabled, true);
    QWebEngineProfile::defaultProfile()->setUseForGlobalCertificateVerification();
#endif

    QUrl url = commandLineUrlArgument();

    Browser browser;
    BrowserWindow *window = browser.createWindow();
    const QRect availableGeometry = window->screen()->availableGeometry();
    window->resize(availableGeometry.width() / 2, (availableGeometry.height() * 2) / 3);
    window->move((availableGeometry.width() - window->width()) / 2,(availableGeometry.height() - window->height()) / 2);
    //window->resize(1024, 768);
    window->showMaximized();
//    window->showFullScreen();
    //设置入口地址
    window->tabWidget()->setUrl(url);
    return app.exec();
}
