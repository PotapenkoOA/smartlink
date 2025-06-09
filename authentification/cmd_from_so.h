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
    std::string m_condition_value;
    // обработчик выполнения? string m_value2;
    void* m_handle;

    ContextPtr m_context;

    public:
    ConditionCmd(std::string lib_path,  ContextPtr context )
    {
        m_context = context;
        //? загрузка условия в json
        // Загружаем библиотеку
        m_handle = dlopen(lib_path.c_str(), RTLD_NOW);
        if (!m_handle) {
            std::cerr << "Ошибка загрузки библиотеки: " << dlerror() << std::endl;
            return ;
        }    
    }

    ~ConditionCmd()
    {
        // Закрываем библиотеку
        if (m_handle)
            dlclose(m_handle);
    }

    void Execute()
    {
        func_t _function = (func_t)dlsym(m_handle, "Handle");

        const char* error = dlerror();
        if (error) {
            std::cerr << "Функция не найдена: " << error << std::endl;
            return;// 1;
        }

        // Вызываем функцию
        _function(m_context);
        return;
    }
};

#endif