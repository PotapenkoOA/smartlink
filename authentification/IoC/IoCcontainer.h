#ifndef __IOCCONTAINER__
#define __IOCCONTAINER__

#include <string>
using namespace std;

#include "IResolverContainer.h"
#include "DependencyResolver.h"

class IoC
{     
    static IDependencyResolver* resolver;

public: 

    template<typename T, typename ...Args >
    static T Resolve( string dependency, Args... args )
    {        
        using ResolverContainerType = ResolverContainer<function<T(Args...)>>;
        shared_ptr<ResolverContainerType> container
                = dynamic_pointer_cast<ResolverContainerType>(IoC::resolver->Resolve(dependency));
        
        //cout<<dependency<<" "<< getType(IoC::resolver->Resolve(dependency))<<""<<IoC::resolver->Resolve(dependency)<<""<<container<<endl;  
        if(container)
            return  container->get()(args...);
        else throw std::bad_cast();  
    }
};

IDependencyResolver* IoC::resolver = new DependencyResolver();

#endif