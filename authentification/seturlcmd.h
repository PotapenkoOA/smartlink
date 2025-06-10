#ifndef __SETRULES_CMD__
#define __SETRULES_CMD__

#include <string>

#include "icommand.h"
#include "context.h"

class SetUrlCmd: public ICommand
{
    IContextPtr m_context;
    std::string m_url;

    public:
    SetUrlCmd( std::string url , IContextPtr context )
    {
        m_context = context; 
        m_url = url;
    }

    void Execute()
    {
        m_context->setNext(true) ;
        http::response<http::string_body> res;
        res.result(http::status::temporary_redirect);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::location, m_url);
        res.body() = "";
        res.prepare_payload();
        m_context->setResponse(res);
    }
};

#endif
