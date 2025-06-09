
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
        int weekday = local_time->tm_wday;

        std::string condition = context->condition;
        if( (condition == "mon") &&  (weekday == 1))
        {
            context->cntxt->next = true ;
            return;
        } 

        if( (condition == "tue") &&  (weekday == 2))
        {
            context->cntxt->next = true ;
            return;
        }

        if( (condition == "wed") &&  (weekday == 3))
        {
            context->cntxt->next = true ;
            return;
        }

        if( (condition == "thu") &&  (weekday == 4))
        {
            context->cntxt->next = true ;
            return;
        } 

        if( (condition == "fri") &&  (weekday == 5))
        {
            context->cntxt->next = true ;
            return;
        } 

        if( (condition == "sat") &&  (weekday == 6))
        {
            context->cntxt->next = true ;
            return;
        } 

        if( (condition == "sun") &&  (weekday == 7))
        {
            context->cntxt->next = true ;
            return;
        } 

        context->cntxt->next = false ;
            return;
}