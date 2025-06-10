#ifndef __IMIDDLEWARE__
#define __IMIDDLEWARE__

#include "context.h"

class IMiddleware
{    
    public:
    virtual void Handle(IContextPtr context) = 0;
};

using IMiddlewarePtr = std::shared_ptr<IMiddleware>;

#endif