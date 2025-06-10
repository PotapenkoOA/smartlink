#ifndef __ICONTEXT__
#define __ICONTEXT__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>

class IContext
{
    public:
        virtual void setNext(bool) = 0;
        virtual bool getNext(void) = 0;
        virtual void setResponse( http::response<http::string_body> res ) = 0;
        virtual http::request<http::string_body> getRequest() = 0;
        
};

using IContextPtr = std::shared_ptr<IContext>;

#endif