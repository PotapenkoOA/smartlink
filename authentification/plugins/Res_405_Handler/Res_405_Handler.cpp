
#include <iostream>
#include <boost/json.hpp>
//#include <memory>
#include <functional>

#include "context.h"

using namespace std;


extern "C"  void Handle( ContextPtr context )
{
        context->next = false;
        context->res.result(http::status::method_not_allowed);
        context->res.set(http::field::content_type, "text/plain");
        context->res.body() = "Method Not Allowed ";
        context->res.prepare_payload();
}