#ifndef PAGETABLE_H
#define PAGETABLE_H
#include <QVector>
#include <QCoreApplication>
#include <QtCore>
#include "pagetableentry.h"
#include <QObject>

class PageTable : public QObject
{
    Q_OBJECT
public:
    PageTable();
    PageTable(const PageTable&);
    void operator=(const PageTable&);
    ~PageTable();
    std::vector<PageTableEntry> pages;
    uint pagesUsed;
    uint totalPages;
    void InitializePages(uint);

signals:

public slots:
};

#endif // PAGETABLE_H
