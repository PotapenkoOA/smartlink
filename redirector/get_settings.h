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

#include "rules.h"
#include "conditions.h"
namespace json = boost::json;      // from <boost/json.hpp>

void read_rules( json::value j )
{
    try{
        /// make chain of rules       
        json::array rules = j.at("rules").as_array();
        IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Redirect.Rules", 
            make_container (  
                std::function<ICommandPtr(ContextPtr)>([rules](ContextPtr context)
                {                     
                    shared_ptr<list<ICommandPtr>> prules_list = make_shared<list<ICommandPtr>>();
                    for( auto rule :  rules )
                    {                                               
                        shared_ptr<list<ICommandPtr>> plist = make_shared<list<ICommandPtr>>();
                        
                        if( rule.as_object().contains("conditions") )
                        {
                            json::object conditions = rule.at("conditions").as_object();
                            {
                            std::string path = "/home/vboxuser/Documents/smart_link/redirector2/plugins/Con_Age_Handler/build/libCon_Age_Handler.so";
                            ConditionContext conContext = { context, conditions.at("age").as_string().c_str()};                           
                            plist->push_back(make_shared<SoConditionCmd>( path, conContext));
                            }
                            {
                            std::string path = "/home/vboxuser/Documents/smart_link/redirector2/plugins/Con_Weekday_Handler/build/libCon_Weekday_Handler.so";
                            ConditionContext conContext = { context, conditions.at("weekday").as_string().c_str()};     
                            plist->push_back(make_shared<SoConditionCmd>( path, conContext));
                            }           
                            {
                            std::string path = "/home/vboxuser/Documents/smart_link/redirector2/plugins/Con_Daytime_Handler/build/libCon_Daytime_Handler.so";
                            ConditionContext conContext = { context, conditions.at("daytime").as_string().c_str()};     
                            plist->push_back(make_shared<SoConditionCmd>( path, conContext));
                            }
                        }
                        
                        if( rule.as_object().contains("url") )
                        {
                            plist->push_back(make_shared<SetUrlCmd>(rule.at("url").as_string().c_str(),context));  
                        }
                        
                        prules_list->push_back( make_shared<RuleCmd>( plist, context ));
                    }

                    return make_shared<StopCheckUpCmd>( prules_list, context );
                })
        ))->Execute();


    }
    catch(exception &e)
    {
        std::cout<<e.what()<<std::endl;
    }
}

void make_chain_of_responsibility( json::value j )
{
    for( auto plugin : j.at("plugins").as_array() )
        {
            if( plugin.is_object() && plugin.as_object().contains("rules") )
            {
               read_rules( plugin.as_object() );
            }            
        }

    /// make chain of responsibilities
    IResolverContainerPtr pDefaultCmdContainer = make_container (  
                function<ICommandPtr(string, ContextPtr)>([]( string path, ContextPtr context )
                {                   
                    return make_shared<ConditionCmd>( path, context );
                })
    );
    IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Http.Res.401", pDefaultCmdContainer)->Execute(); 
    IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Http.Res.405", pDefaultCmdContainer)->Execute(); 
    IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Http.Res.307", //pDefaultCmdContainer)->Execute(); 
            make_container (  
                function<ICommandPtr(string, ContextPtr)>([]( string path, ContextPtr context )
                {
                    return IoC::Resolve<ICommandPtr>("Redirect.Rules", context);
                })
    ))->Execute(); 

    

        json::array plugin_pathes = j.at("plugins").as_array();
        IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Http.Middleware", 
            make_container (  
                function<IMiddlewarePtr(ContextPtr)>([plugin_pathes](ContextPtr context)
                {                     
                    IMiddlewarePtr pNext = nullptr;
                    for( int i = plugin_pathes.size()-1; i >= 0; --i )
                    {
                        json::value path = plugin_pathes[i];
                        //std::cout<<path.at("handler")<<std::endl;

                        string path2so = path.at("handler").as_string().c_str();
                        ICommandPtr cmd;
                       
                        cmd = IoC::Resolve<ICommandPtr>("Http.Res."+ to_string(path.at("code").as_int64()), path2so, context );                         
                        
                        pNext = make_shared<MidlewareOne>( pNext, cmd );                    
                    }

                    return pNext;
                })
        ))->Execute();
}

void make_server(json::value j)
{
    unsigned short port = j.at("port").as_int64();
    IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "HttpServer",
        make_container(
            function<IServerPtr()>([port]()
            { return make_shared<Server>(port); })))
        ->Execute();
}

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

    try{    
        make_server( j );

        make_chain_of_responsibility( j );        
    }
    catch(exception &e)
    {
        std::cout<<e.what()<<std::endl;
    }

    return "";
}



#endif