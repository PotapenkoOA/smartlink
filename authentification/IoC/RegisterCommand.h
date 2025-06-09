#ifndef __REGISTERCMD__
#define __REGISTERCMD__

#include "../icommand.h"
#include "IoCcontainer.h"
#include "IResolverContainer.h"

class RegisterCommand: public ICommand
{
    string _dependency;
    IResolverContainerPtr _pcontainer;

    IObjectPtr _currentScope;

public:
    RegisterCommand( IObjectPtr currentScope, string dependency, IResolverContainerPtr pcontainer )
    {
        _currentScope = currentScope;
        _dependency = dependency;
        _pcontainer = pcontainer;       
    }

    void Execute()
    {
        if(_currentScope == nullptr)
            throw new invalid_argument("RegisterCommand: scope is nullptr");
       (*_currentScope)[_dependency] = _pcontainer;
    }
};

#endif