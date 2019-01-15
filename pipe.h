#ifndef PIPE_H
#define PIPE_H
#include <QList>
#include <QObject>

class Pipe
{
public:
    Pipe();
    Pipe(int, int);
    ~Pipe();

    void Write(int, int);
    int Read(int);
private:
    int sharedVariable;
    int lock;

    void SetTheLock(int);

};

#endif // PIPE_H
