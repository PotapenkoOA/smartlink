#ifndef __SETTINGS__
#define __SETTINGS__

#include <boost/json.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

#include "server.h"
#include "IoC/IoCcontainer.h"

#include "middleware.h"

namespace json = boost::json;      // from <boost/json.hpp>

std::string read_dependencies(std::string path)
{

    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Не удалось открыть файл" << std::endl;
        return "";
    }

    // Читаем содержимое файла в строку
    std::string json_str((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    // Парсим JSON
    json::value j = json::parse(json_str);

    // Работа с данными
    // ...
    try{    
        unsigned short port = j.at("port").as_int64();

        /// make server
        IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "HttpServer", 
            make_container (  
                function<IServerPtr()>([port]()
                { 	
                    return make_shared<Server>( port );
                })
        ))->Execute();

        /// make chain of responsibilities       
        json::array plugin_pathes = j.at("plugins").as_array();
        IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Http.Middleware", 
            make_container (  
                function<IMiddlewarePtr(ContextPtr)>([plugin_pathes](ContextPtr context)
                {                     
                    IMiddlewarePtr pNext = nullptr;
                    for( int i = plugin_pathes.size()-1; i >= 0; --i )
                    {
                        json::value path = plugin_pathes[i];

                        string path2so = path.at("handler").as_string().c_str();
                                    
                        ICommandPtr cmd = make_shared<ConditionCmd>( path2so, context );
                        pNext = make_shared<MidlewareOne>( pNext, cmd );                    
                    }

                    return pNext;
                })
        ))->Execute();
    }
    catch(exception &e)
    {
        std::cout<<e.what()<<std::endl;
    }

    return "";
}



#endif