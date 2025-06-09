#ifndef __DEPENDENCYRESOLVER__
#define __DEPENDENCYRESOLVER__

#include "IResolverContainer.h"
#include "RegisterCommand.h"

#include <map>
#include <thread>

using namespace std;


class IDependencyResolver
{
protected:
    /* data */
public:
    virtual  ~IDependencyResolver() = default;

    virtual IResolverContainerPtr Resolve(string dependency) = 0;
};

class DependencyResolver: public IDependencyResolver
{
    thread_local static IObjectPtr _pCurrentScope;

    IObjectPtr _pRootScope;

    public:
    DependencyResolver()
    {
        _pCurrentScope = nullptr;
        _pRootScope = make_shared<map< string, IResolverContainerPtr>>();

        // _dependencies["IoC.Scope.Current.Set"] - получает скоуп и делает его текущим
        map< string, IResolverContainerPtr>* pRootScope = _pRootScope.get();

        (*_pRootScope)["IoC.Scope.Current.Set"] = make_container (
            function< void(IObjectPtr)> (
                [&](IObjectPtr current)
                {
                    _pCurrentScope = current;
                }
            )
        );

        // _dependencies["IoC.Scope.Current.Clear"] - очищает текущий скоуп
        (*_pRootScope)["IoC.Scope.Current.Clear"] = make_container (
            function< void(void)> (
                [&](void)
                {
                    _pCurrentScope = nullptr;
                }
            )
        );

        // _dependencies["IoC.Scope.Current"] - возвращает текущий скоуп
        (*_pRootScope)["IoC.Scope.Current"] = make_container (
            function< IObjectPtr(void)> (
                [&](void)
                {
                    return  (_pCurrentScope == nullptr) ? _pRootScope : _pCurrentScope;
                }
            )
        );
        
        //  _dependencies["IoC.Scope.Parent"] = exception для рутового скоупа
        (*_pRootScope)["IoC.Scope.Parent"] = make_container (
            function< IObjectPtr(void)>( []()
            {
                throw exception();
                return nullptr;
            })
        );
           
        //  _dependencies["IoC.Scope.Create.Empty"] = создание пустого скоупа
        (*_pRootScope)["IoC.Scope.New.Empty"] = make_container (
            function< IObjectPtr(void)> ( []() 
            { 
                return  make_shared<map< string, IResolverContainerPtr>>();
            })
        );

        //  _dependencies["IoC.Scope.Create"] - создание скоупа с перентом, если указан
        (*_pRootScope)["IoC.Scope.New"] = make_container(
            function< IObjectPtr(IObjectPtr)> (
                [&](IObjectPtr parent)
                {
                    shared_ptr<ResolverContainer<function<IObjectPtr(void)>>> container 
                                    = dynamic_pointer_cast<ResolverContainer<function<IObjectPtr(void)>>> ((*_pRootScope)["IoC.Scope.New.Empty"]);
                    IObjectPtr pCreatingScope = container->get()();
                    
                    if( parent == nullptr )
                    {
                        shared_ptr<ResolverContainer<function< IObjectPtr(void)>>> pContainer 
                                    = dynamic_pointer_cast<ResolverContainer<function<IObjectPtr(void)>>>((*_pRootScope)["IoC.Scope.Current"]);
            
                        parent = pContainer->get()(); 
                        (*pCreatingScope)["IoC.Scope.Parent"] = make_container (
                            function<IObjectPtr(void)>([parent]()
                            {      
                                return parent;
                            })
                        );
                    }
                    else
                    {
                        (*pCreatingScope)["IoC.Scope.Parent"] = make_container (
                            function<IObjectPtr(void)>([parent]()
                            { 
                                return parent;
                            })
                        );
                    }
                    return  pCreatingScope;
                }
            )
        );

        // инициализация базовых потребностей
        (*_pRootScope)["IoC.Register"] =  make_container ( 
            function< ICommandPtr ( string, IResolverContainerPtr )>(
                [&](string dependency, IResolverContainerPtr pResolver)
                {
                    return make_shared<RegisterCommand> ((_pCurrentScope != nullptr)? _pCurrentScope : _pRootScope, 
                        dependency, pResolver );
                }
             )
        );  
    }

    IResolverContainerPtr Resolve( string dependency )
    {
        shared_ptr<ResolverContainer<function< IObjectPtr(void)>>> pContainer =
            dynamic_pointer_cast<ResolverContainer<function<IObjectPtr(void)>>> ((*_pRootScope)["IoC.Scope.Current"]);
        return Resolve(pContainer->get()(), dependency);
    }

    IResolverContainerPtr Resolve( IObjectPtr dependencies, string dependency )
    {
        
        IObjectPtr _dependencies = dependencies;
        while( 1 )
        {
            if( _dependencies->count(dependency) != 0 )
            {
               //cout<< "Resolve:" << dependency<<"scope"<< dependencies <<endl;
               return (*_dependencies)[dependency];
            }
            else  
            {
                /// обратиться к родительскому скоупу
                shared_ptr<ResolverContainer<function< IObjectPtr(void)>>> pContainer =
                    dynamic_pointer_cast<ResolverContainer<function<IObjectPtr(void)>>> ((*_dependencies)["IoC.Scope.Parent"]);                        
                
                _dependencies = pContainer->get()();
               
                return Resolve( _dependencies,dependency);
            }
        }

        return nullptr;
    }
};

thread_local IObjectPtr DependencyResolver::_pCurrentScope = nullptr;

#endif