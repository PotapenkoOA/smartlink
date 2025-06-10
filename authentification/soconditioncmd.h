#ifndef __SOCONDITIONS_CMD__
#define __SOCONDITIONS_CMD__

#include <string>
#include <list>
#include <iostream>
#include "icommand.h"
#include <boost/json.hpp>
#include <filesystem>
#include <dlfcn.h>

#include "iconditioncontext.h"

typedef void (*func__t)(IConditionContextPtr);

class SoConditionCmd: public ICommand
{
    std::string m_condition_value;    
    void* m_handle;

    IConditionContextPtr m_context;

    public:
    SoConditionCmd(std::string lib_path,  IConditionContextPtr context )
    {
        m_context = context;
        
        m_handle = dlopen(lib_path.c_str(), RTLD_NOW);
        if (!m_handle) {
            std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
            return ;
        }    
    }

    ~SoConditionCmd()
    {
        if (m_handle)
            dlclose(m_handle);
    }

    void Execute()
    {
        func__t _function = (func__t)dlsym(m_handle, "Handle");

        const char* error = dlerror();
        if (error) {
            std::cerr << "Функция не найдена: " << error << std::endl;
            return;// 1;
        }

        _function(m_context);
        cout<<m_context->getConditionValue()<<" "<<m_context->getContext()->getNext()<<endl;

        return;
    }
};

#endif