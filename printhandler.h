/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#ifndef PRINTHANDLER_H
#define PRINTHANDLER_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QPainter;
class QPrinter;
class QWebEnginePage;
QT_END_NAMESPACE

class PrintHandler : public QObject
{
    Q_OBJECT
public:
    PrintHandler(QObject *parent = nullptr);
    void setPage(QWebEnginePage *page);

public slots:
    void print();
    void printPreview();
    void printDocument(QPrinter *printer);

private:
    QWebEnginePage *m_page = nullptr;
    bool m_inPrintPreview = false;
};

#endif // PRINTHANDLER_H
