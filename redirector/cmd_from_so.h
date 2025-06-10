#ifndef __CONDITIONS_CMD__
#define __CONDITIONS_CMD__

#include <string>
#include <list>
#include <iostream>
#include "icommand.h"
#include <boost/json.hpp>
#include <filesystem>
#include <dlfcn.h>

#include "context.h"

typedef void (*func_t)(ContextPtr);

class ConditionCmd: public ICommand
{
    void* m_handle;
    ContextPtr m_context;

    public:
    ConditionCmd( std::string lib_path,  ContextPtr context )
    {
        m_context = context;        
        m_handle = dlopen(lib_path.c_str(), RTLD_NOW);
        if (!m_handle) {
            std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
            return ;
        }    
    }

    ~ConditionCmd()
    {
        if (m_handle)
            dlclose(m_handle);
    }

    void Execute()
    {
        func_t _function = (func_t)dlsym(m_handle, "Handle");
        const char* error = dlerror();
        if (error) {
            std::cerr << "Функция не найдена: " << error << std::endl;
            return;
        }
       
        _function(m_context);
        return;
    }
};

typedef void (*func__t)(ConditionContextPtr);

class SoConditionCmd: public ICommand
{
    std::string m_condition_value;    
    void* m_handle;

    ConditionContextPtr m_context;

    public:
    SoConditionCmd(std::string lib_path,  ConditionContext context )
    {
        m_context = make_shared<ConditionContext>(context);
        
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
        cout<<m_context->condition<<" "<<m_context->cntxt->next<<endl;

        return;
    }
};

#endif