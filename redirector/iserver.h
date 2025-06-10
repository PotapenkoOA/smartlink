#ifndef __SERVER__
#define __SERVER__

class IServer 
{
    public:
    virtual void Loop() = 0;
};

using IServerPtr = std::shared_ptr<IServer>;

#endif