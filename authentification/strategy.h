#ifndef __STRATEGY_CMD__
#define __STRATEGY_CMD__

#include <list>

#include "icommand.h"
#include "context.h"
#include "IoC/IoCcontainer.h"
#include "middleware.h"
#include "cmd_from_so.h"

class StrategyCmd: public ICommand{

    ContextPtr m_context;
    IMiddlewarePtr m_handler;
    
    public:
    StrategyCmd( ContextPtr context )
    {
        m_context = context;
        m_handler = IoC::Resolve<IMiddlewarePtr>( "Http.Middleware", context );        
    }

    void Execute()
    {
        m_handler->Handle(m_context);
    }
};

#endif