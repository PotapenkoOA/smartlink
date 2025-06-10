#ifndef __RULES_CMD__
#define __RULES_CMD__

#include <string>
#include <list>

#include "icommand.h"
#include "icontext.h"

class RuleCmd: public ICommand
{
    IContextPtr m_context;
    shared_ptr<std::list<ICommandPtr>> m_cmds;

    public:
    RuleCmd( shared_ptr<std::list<ICommandPtr>> pcmds , IContextPtr context )
    {
        m_context = context; 
        m_cmds = pcmds;
    }

    void Execute()
    {
        for( auto cmd: *m_cmds )
        {
            cmd->Execute();
            if( ! m_context->getNext() )
                return;
        }
    }
};

class StopCheckUpCmd: public ICommand
{
    IContextPtr m_context;
    shared_ptr<std::list<ICommandPtr>> m_cmds;

    public:
    StopCheckUpCmd( shared_ptr<std::list<ICommandPtr>> pcmds , IContextPtr context )
    {
        m_context = context; 
        m_cmds = pcmds;
    }

    void Execute()
    {
        for( auto cmd: *m_cmds )
        {
            cmd->Execute();
            if( m_context->getNext() )
            {
                m_context->setNext(!m_context->getNext());
                return;
            }
        }
    }
};

#endif