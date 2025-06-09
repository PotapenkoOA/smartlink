#ifndef __RULES_CMD__
#define __RULES_CMD__

#include <string>
#include <list>
#include <iostream>

#include "icommand.h"
#include "context.h"
#include "conditions.h"

class RuleCmd: public ICommand
{
    ContextPtr m_context;
    shared_ptr<std::list<ICommandPtr>> m_cmds;

    public:
    RuleCmd( shared_ptr<std::list<ICommandPtr>> pcmds , ContextPtr context )
    {
        m_context = context; 
        m_cmds = pcmds;
    }

    void Execute()
    {
        for( auto cmd: *m_cmds )
        {
            std::cout<<"RuleCmd\n";
            cmd->Execute();
            if( ! m_context->next )
                return;
        }
    }
};

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
        //std::cout<<"url:"<<m_url<<std::endl;
        m_context->next = true;
        m_context->res.result(http::status::temporary_redirect);
        m_context->res.set(http::field::content_type, "application/json");
        m_context->res.set(http::field::location, m_url);
    }
};

class StopCheckUpCmd: public ICommand
{
    ContextPtr m_context;
    shared_ptr<std::list<ICommandPtr>> m_cmds;

    public:
    StopCheckUpCmd( shared_ptr<std::list<ICommandPtr>> pcmds , ContextPtr context )
    {
        m_context = context; 
        m_cmds = pcmds;
    }

    void Execute()
    {
        for( auto cmd: *m_cmds )
        {
           // std::cout<<"NextCmd\n";
            cmd->Execute();
            if( m_context->next )
            {
                m_context->next = !m_context->next;
                return;
            }
        }
    }
};



#endif