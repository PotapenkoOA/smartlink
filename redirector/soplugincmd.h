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

typedef void (*func_t)(IContextPtr);

class ConditionCmd: public ICommand
{
    void* m_handle;
    IContextPtr m_context;

    public:
    ConditionCmd(std::string lib_path,  IContextPtr context )
    :m_context( context )
    {
        m_handle = dlopen( lib_path.c_str(), RTLD_NOW );
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

#endif