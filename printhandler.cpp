/****************************************************************************
**
** Copyright (C) 2022 hlinfo.net
** Contact: https://raw.githubusercontent.com/hlinfocc/myOpenBrowser/master/LICENSE
**
****************************************************************************/


#include "printhandler.h"
#include <QEventLoop>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include <QPrintPreviewDialog>
#include <QWebEnginePage>

PrintHandler::PrintHandler(QObject *parent)
    : QObject(parent)
{

}

void PrintHandler::setPage(QWebEnginePage *page)
{
    Q_ASSERT(!m_page);
    m_page = page;
    connect(m_page, &QWebEnginePage::printRequested, this, &PrintHandler::printPreview);
}

void PrintHandler::print()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, m_page->view());
    if (dialog.exec() != QDialog::Accepted)
        return;
    printDocument(&printer);
}

void PrintHandler::printDocument(QPrinter *printer)
{
    QEventLoop loop;
    bool result;
    auto printPreview = [&](bool success) { result = success; loop.quit(); };
    m_page->print(printer, std::move(printPreview));
    loop.exec();
    if (!result) {
        QPainter painter;
        if (painter.begin(printer)) {
            QFont font = painter.font();
            font.setPixelSize(20);
            painter.setFont(font);
            painter.drawText(QPointF(10,25),
                             QStringLiteral("Could not generate print preview."));

            painter.end();
        }
    }
}

void PrintHandler::printPreview()
{
    if (!m_page)
        return;
    if (m_inPrintPreview)
        return;
    m_inPrintPreview = true;
    QPrinter printer;
    QPrintPreviewDialog preview(&printer, m_page->view());
    connect(&preview, &QPrintPreviewDialog::paintRequested,
            this, &PrintHandler::printDocument);
    preview.setWindowTitle(tr("Print Preview"));
    preview.resize(1024,780);
//    preview.showMaximized();
    preview.exec();
    m_inPrintPreview = false;
}
