
#ifndef __IRESOLVECONTAINER__
#define __IRESOLVECONTAINER__

#include <map>
#include <string>
using namespace std;

class IResolverContainer
{
    public:
        virtual ~IResolverContainer() = default;
};

using IResolverContainerPtr = shared_ptr<IResolverContainer>;

template<typename Type >
class ResolverContainer: public IResolverContainer
{
    Type m_Resolver;

public:    
    ResolverContainer(Type resolver)
    {
        m_Resolver = resolver;
    }    

    Type get()
    {
        return m_Resolver;
    }
};

using IObjectPtr = shared_ptr<map< string, IResolverContainerPtr>>;

template<typename ResolverFuncPrototype>
IResolverContainerPtr make_container(ResolverFuncPrototype resolver)
{
    return static_pointer_cast<IResolverContainer>(
        make_shared<ResolverContainer <ResolverFuncPrototype>> (resolver) );
}

#endif