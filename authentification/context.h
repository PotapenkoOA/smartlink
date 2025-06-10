#ifndef __THE_CONTEXT__
#define __THE_CONTEXT__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>

#include "icontext.h"

class Context: public IContext
{
    bool m_nextState;
    std::shared_ptr<http::response<http::string_body>> m_res;
    std::shared_ptr<http::request<http::string_body>>  m_req;

    public:
        Context(  std::shared_ptr<http::request<http::string_body>>  req, std::shared_ptr<http::response<http::string_body>> res )
        {
            m_res = res;
            m_req = req;
            m_nextState = true;
        }
        void setNext(bool nextState) 
        {
            m_nextState = nextState;
        }

        bool getNext(void) 
        {
            return m_nextState;
        }

        void setResponse( http::response<http::string_body> res ) 
        {
            *m_res = res;
        }

        http::request<http::string_body> getRequest()
        {
            return *m_req;
        }
};

#endif