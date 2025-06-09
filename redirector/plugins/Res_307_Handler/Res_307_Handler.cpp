
#include <iostream>
#include <boost/json.hpp>
//#include <memory>
#include <functional>

#include "context.h"

using namespace std;


extern "C"  void Handle( ContextPtr context )
{
        if(context->req.method() != http::verb::get)
        {
                context->next = true;
                return;
        }

        context->next = false;
        context->res.result(http::status::temporary_redirect);
        //context->res.res{http::status::temporary_redirect, context->req.version()};
        //res.set(http::field::server, "C++ Beast");
        context->res.set(http::field::content_type, "application/json");
        context->res.set(http::field::location, "//ru.wikipedia.org/wiki/Спокойной_ночи,_малыши!");
        //context->res.body() = "//ya.ru";
        context->res.prepare_payload();
}