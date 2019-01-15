#include "mailbox.h"

MailBox::MailBox()
{

    messageHolder[0].first = 958038;
    messageHolder[1].first = 204895;
    messageHolder[2].first = 586832;
    messageHolder[3].first = 139583;
    messageHolder[4].first = 585920;


}
MailBox::~MailBox(){}
void MailBox::Write(int address, int lock, int data)
{
    if(lock == messageHolder[address].first)
    {
        messageHolder[address].second = data;
    }
}
int MailBox::Read(int address, int lock)
{

    if(lock == messageHolder[address].first)
    {
        return messageHolder[address].second;
    }

    return -1;
}

