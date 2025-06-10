#ifndef __STRATEGY_CMD__
#define __STRATEGY_CMD__

#include "icommand.h"
#include "icontext.h"
#include "imiddleware.h"

#include "IoC/IoCcontainer.h"


class StrategyCmd: public ICommand{

    IContextPtr m_context;
    IMiddlewarePtr m_handler;
    
    public:
    StrategyCmd( IContextPtr context )
    :m_context( context )
    {
        m_handler = IoC::Resolve<IMiddlewarePtr>( "Http.Middleware", context );        
    }

    void Execute()
    {
        m_handler->Handle(m_context);
    }
};

#endif