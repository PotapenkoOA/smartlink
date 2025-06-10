#ifndef __RULES_CMD__
#define __RULES_CMD__

#include <string>
#include <list>

#include "icommand.h"
#include "context.h"

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
            cmd->Execute();
            if( ! m_context->next )
                return;
        }
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