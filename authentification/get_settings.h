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
#include "iconditioncontext.h"
#include "soconditioncmd.h"
#include "soplugincmd.h"

#include "rules.h"
#include "seturlcmd.h"
namespace json = boost::json;      // from <boost/json.hpp>

using IComandList =  std::list<ICommandPtr>;
using IComandListPtr =  std::shared_ptr<list<ICommandPtr>>;

class ReadSettings{

    static void read_rules( json::value j )
    {
        try{
            /// make chain of rules       
            json::array rules = j.at("rules").as_array();
            IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>(
                "IoC.Register", 
                "Redirect.Rules", 
                make_container (  
                    std::function<ICommandPtr(IContextPtr)>( [rules](IContextPtr context)
                    {                     
                        IComandListPtr rules_list = make_shared<IComandList>();
                        for( auto rule :  rules )
                        {                                               
                            IComandListPtr plist = make_shared<IComandList>();
                            
                            if( rule.is_object() && rule.as_object().contains("conditions") )
                            {
                                json::object conditions = rule.at("conditions").as_object();
                                for( auto condition : conditions )
                                {
                                    IConditionContextPtr conContext = make_shared<ConditionContext>
                                                                        ( context, condition.value().as_string().c_str() );                           
                                    plist->push_back(
                                        make_shared<SoConditionCmd>( 
                                            "./plugins/libCon_" + string(condition.key()) + "_Handler.so", conContext));
                                }                           
                            }
                            
                            if( rule.as_object().contains("url") )
                            {
                                plist->push_back( 
                                    make_shared<SetUrlCmd>(
                                        rule.at("url").as_string().c_str(),context
                                    ));  
                            }
                            
                            rules_list->push_back( 
                                make_shared<RuleCmd>( plist, context ));
                        }
                        return make_shared<StopCheckUpCmd>( rules_list, context );
                    })
            ))->Execute();

        }
        catch(exception &e)
        {
            std::cout<<e.what()<<std::endl;
        }
    }

    static void make_chain_of_responsibility( json::value j )
    {
        /// make chain of responsibilities
        IResolverContainerPtr pDefaultCmdContainer = make_container (  
                    function<ICommandPtr(string, IContextPtr)>([]( string path, IContextPtr context )
                    {                   
                        return make_shared<ConditionCmd>( path, context );
                    })
        );

        json::array plugin_pathes = j.at("plugins").as_array();
        for( auto plugin : plugin_pathes )
        {
            if( plugin.is_object() && plugin.as_object().contains("rules") )
            {
                read_rules( plugin.as_object() );

                IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>(
                    "IoC.Register", 
                    "Http.Res."+to_string(plugin.at("code").as_int64()),  
                    make_container (  
                        function<ICommandPtr(string, IContextPtr)>([]( string path, IContextPtr context )
                        {                        
                            return IoC::Resolve<ICommandPtr>("Redirect.Rules");
                        })
                    ))->Execute(); 
            }                   
            else 
                IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>( 
                    "IoC.Register", 
                    "Http.Res." +to_string(plugin.at("code").as_int64()),
                     pDefaultCmdContainer)->Execute(); 
        }       
        
        IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "Http.Middleware", 
                make_container (  
                    function<IMiddlewarePtr(IContextPtr)>([plugin_pathes](IContextPtr context)
                    {                     
                        IMiddlewarePtr pNext = nullptr;
                        for( int i = plugin_pathes.size()-1; i >= 0; --i )
                        {
                            json::value path = plugin_pathes[i];
                            
                            string path2so = path.at("handler").as_string().c_str();
                            ICommandPtr cmd = IoC::Resolve<ICommandPtr>(
                                "Http.Res."+ to_string(path.at("code").as_int64()), path2so, context );                         
                            
                            pNext = make_shared<MidlewareOne>( pNext, cmd );                    
                        }

                        return pNext;
                    })
            ))->Execute();
    }

    static void make_server(json::value j)
    {
        unsigned short port = j.at("port").as_int64();
        IoC::Resolve<ICommandPtr, string, IResolverContainerPtr>("IoC.Register", "HttpServer",
            make_container(
                function<IServerPtr()>([port]()
                { return make_shared<Server>(port); })))
            ->Execute();
    }

public: 
    static std::string read_dependencies(std::string path)
    {
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Не удалось открыть файл" << std::endl;
            return "";
        }

        std::string json_str((std::istreambuf_iterator<char>(file)),
                            std::istreambuf_iterator<char>());
        try{  
            json::value j = json::parse(json_str);

            make_server( j );

            make_chain_of_responsibility( j );        
        }
        catch(exception &e)
        {
            std::cout<<e.what()<<std::endl;
        }
        return "";
    }
};

#endif