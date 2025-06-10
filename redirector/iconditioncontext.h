#ifndef __ICONDITIONALCONTEXT__
#define __ICONDITIONALCONTEXT__

#include "icontext.h"

class IConditionContext
{
    public:
        virtual std::string getConditionValue() = 0;
        virtual IContextPtr getContext() = 0;
        
};

using IConditionContextPtr = std::shared_ptr<IConditionContext>;

class ConditionContext: public IConditionContext
{
    IContextPtr m_context;
    std::string m_conditionalValue;
    public:
        ConditionContext (IContextPtr context, std::string conditionalValue)
        {
            m_context = context;
            m_conditionalValue = conditionalValue;
        }

        std::string getConditionValue() 
        {
            return m_conditionalValue;
        }

        IContextPtr getContext() { return m_context; }
        
};


#endif