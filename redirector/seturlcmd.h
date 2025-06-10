#ifndef __SETRULES_CMD__
#define __SETRULES_CMD__

#include <string>

#include "icommand.h"
#include "context.h"

class SetUrlCmd: public ICommand
{
    ContextPtr m_context;
    std::string m_url;

    public:
    SetUrlCmd( std::string url , ContextPtr context )
    {
        m_context = context; 
        m_url = url;
    }

    void Execute()
    {
        m_context->next = true;
        m_context->res.result(http::status::temporary_redirect);
        m_context->res.set(http::field::content_type, "application/json");
        m_context->res.set(http::field::location, m_url);
        m_context->res.body() = "";
        m_context->res.prepare_payload();
    }
};

#endif
