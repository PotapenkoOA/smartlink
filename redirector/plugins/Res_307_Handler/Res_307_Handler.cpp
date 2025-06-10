
#include <iostream>
#include <boost/json.hpp>
//#include <memory>
#include <functional>

#include "icontext.h"

using namespace std;


extern "C"  void Handle( IContextPtr context )
{
        http::response<http::string_body> res;
        http::request<http::string_body> req = context->getRequest();

        if(req.method() != http::verb::get)
        {
                context->setNext(true) ;
                return;
        }

        context->setNext(false) ;
        res.result(http::status::temporary_redirect);
        res.set(http::field::content_type, "application/json");
        res.set(http::field::location, "//ru.wikipedia.org/wiki/Спокойной_ночи,_малыши!");
        res.prepare_payload();
}