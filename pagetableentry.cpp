#include "pagetableentry.h"

PageTableEntry::PageTableEntry(unsigned int page)
{
    pageNumber = page;
    valid = false;
    beingShared = false;
}
PageTableEntry::PageTableEntry(const PageTableEntry& pte)
{
    pageNumber = pte.pageNumber;
    logicalAddress = pte.logicalAddress;
    lastUsed = pte.lastUsed;
    valid = pte.valid;
    beingShared = pte.beingShared;
    locationOfVA =  pte.locationOfVA;
}
void PageTableEntry::operator=(const PageTableEntry& pte)
{
    pageNumber = pte.pageNumber;
    logicalAddress = pte.logicalAddress;
    lastUsed = pte.lastUsed;
    valid = pte.valid;
    beingShared = pte.beingShared;
    locationOfVA = pte.locationOfVA;
}
PageTableEntry::~PageTableEntry(){}
unsigned int PageTableEntry::GetPageNumber()
{
    return pageNumber;
}
QString PageTableEntry::GetLogicalAddress()
{
    return logicalAddress;
}
void PageTableEntry::SetLogicalAddress(QString address)
{
    logicalAddress = address;
}
bool PageTableEntry::CheckValidation()
{
    if(valid == true)
    {
        return true;
    }
    else{
        return false;
    }
}
void PageTableEntry::SetValidation(bool validBit)
{
    valid  = validBit;
}
