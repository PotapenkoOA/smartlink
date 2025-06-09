#ifndef __COMMAND_INTERFACE
#define __COMMAND_INTERFACE

#include <memory>

using namespace std;

class ICommand{
    public:
    virtual void Execute() = 0;
};

using ICommandPtr = std::shared_ptr<ICommand>;

#endif