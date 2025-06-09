#ifndef __THE_CONTEXT__
#define __THE_CONTEXT__

#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/json.hpp>

namespace beast = boost::beast;     // from <boost/beast.hpp>
namespace http = beast::http;      // from <boost/beast/http.hpp>
namespace json = boost::json;      // from <boost/json.hpp>

bool state_value = false;

struct Context
{
    bool next;

    http::response<http::string_body> res;
    http::request<http::string_body> req;
};

using ContextPtr = std::shared_ptr<Context>;



#endif