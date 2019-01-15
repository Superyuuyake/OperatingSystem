#include "line.h"

Line::Line()
{
}
Line::Line(QString name, int amount)
{
    this->name = name;
    this->amount = amount;
}
Line::Line(const Line& b1)
{
    amount = b1.amount;
    name = b1.name;
}
Line::~Line(){}
void Line::operator=(const Line& line)
{
    amount = line.amount;
    name = line.name;
}
void Line::setAmount(int amount)
{
    this->amount = amount;
}
int Line::getAmount()
{
    return amount;
}
void Line::setName(QString name)
{
    this->name = name;
}
QString Line::getName()
{
    return name;
}
