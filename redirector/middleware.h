#ifndef __MIDDLEWARE__
#define __MIDDLEWARE__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <string>

using std::cout, std::endl;

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>

#include "imiddleware.h"
#include "icommand.h"
#include "context.h"

class MidlewareOne: public IMiddleware
{    
    IMiddlewarePtr m_next; 
    ICommandPtr m_cmd;
    public:
    MidlewareOne( IMiddlewarePtr next, ICommandPtr cmd )
    {
        m_next = next;
        m_cmd = cmd;
    }    

    void Handle( ContextPtr context )
    {
        m_cmd->Execute();
         
        if(m_next != nullptr && context->next)
            m_next->Handle(context);
    }
};

#endif