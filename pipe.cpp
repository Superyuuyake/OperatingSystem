#include "pipe.h"

Pipe::Pipe()
{

}
Pipe::Pipe(int lock, int communicationVariable)
{
    this->lock = lock;
    this-> sharedVariable = communicationVariable;
}
Pipe::~Pipe(){}
int Pipe::Read(int key)
{
    if(key == lock)
    {
        return sharedVariable;
    }
    return -1;
}
void Pipe::Write(int key, int variable)
{
    if(key == lock)
        sharedVariable = variable;
}
