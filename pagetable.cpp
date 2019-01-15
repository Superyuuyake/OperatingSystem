#include "pagetable.h"
#include "pagetableentry.h"

PageTable::PageTable()
{
    pagesUsed=0;
}
PageTable::PageTable(const PageTable& pt)
{
    pages = pt.pages;
    pagesUsed = pt.pagesUsed;
    totalPages = pt.totalPages;
}
void PageTable::operator=(const PageTable& pt)
{
    pages = pt.pages;
    pagesUsed = pt.pagesUsed;
    totalPages = pt.totalPages;
}
PageTable::~PageTable(){}
void PageTable::InitializePages(unsigned int numberOfPages)
{
    for(unsigned int i = 0; i<numberOfPages; i++)
        pages.emplace_back(PageTableEntry(i));

}

