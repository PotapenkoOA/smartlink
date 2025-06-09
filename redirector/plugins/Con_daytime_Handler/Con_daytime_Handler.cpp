
#include <boost/beast.hpp>
#include <boost/url.hpp>

namespace beast = boost::beast;
namespace http = beast::http;
namespace urls = boost::urls;

#include <string>
#include <list>
#include <iostream>
#include <memory>
#include <ctime>


#include "context.h"

using namespace std;


extern "C"  void Handle( ConditionContextPtr context )
{
        std::time_t now = std::time(nullptr);
    
    // Преобразуем в локальное время
        std::tm* local_time = std::localtime(&now);
    
    // День недели (0-6, где 0 - воскресенье)
        int daytime = local_time->tm_hour > 12;

        //std::cout<<local_time->tm_hour<<std::endl;

        std::string condition = context->condition;
        if( ( condition == "pm" ) && (daytime == 1) )
        {
            context->cntxt->next = true ;
            return;
        } 

        if( ( condition == "am" ) && (daytime == 0) )
        {
            context->cntxt->next = true ;
            return;
        }

        context->cntxt->next = false ;
            return;

}

int main()
{
    ConditionContextPtr context = make_shared<ConditionContext>();
    context->cntxt = make_shared<Context>();
    context->condition = "pm";
    Handle( context );
    std::cout<<context->cntxt->next<<std::endl;
}