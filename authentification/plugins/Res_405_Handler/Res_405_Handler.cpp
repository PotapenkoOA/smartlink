
#include <iostream>
#include <boost/json.hpp>

#include <functional>

#include "icontext.h"

using namespace std;


extern "C"  void Handle( IContextPtr context )
{
        context->setNext(false) ;
        http::response<http::string_body> res;
        res.result(http::status::method_not_allowed);
        res.set(http::field::content_type, "text/plain");
        res.body() = "Method Not Allowed ";
        res.prepare_payload();
        context->setResponse(res);
}