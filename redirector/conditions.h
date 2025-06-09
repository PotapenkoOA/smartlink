#ifndef __RULES_CONDITIONS_CMD__
#define __RULES_CONDITIONS_CMD__

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
#include <functional>


#include "context.h"
#include "icommand.h"

class AgeCheckUp:public ICommand
{
    int m_age;
    std::shared_ptr<std::function<bool(int)>> func;
    ContextPtr m_context;

    public:

    AgeCheckUp( std::string condition , ContextPtr context ){
        m_context = context;
        m_age = atoi(condition.c_str());
        
        if( condition.find_last_of("+") != std::string::npos )
        {
            func = std::make_shared<std::function<bool(int)>>([&]( int age ){ return age >= m_age;  });
        }    
        else if (condition.find_last_of("-") != std::string::npos )
        {
            func = std::make_shared<std::function<bool(int)>>( [&]( int age ){ return age <= m_age;  } );
        }
        else { 
            func = std::make_shared<std::function<bool(int)>>( [&]( int age ){ return age == m_age;  } );
        }
    }

    void Execute(){
    
        //if( m_context->req.target().contains("age") )
          //  m_age = atoi(m_context->req.target().at("age").as_string().c_str());
        
        m_context->next = ( *func)(18);    
        std::cout<<"AgeCheckUp"<<m_context->next<<std::endl;
    }
};


class WeekdayCheckUp: public ICommand
{
    int m_weekday;
    const char* days;
    ContextPtr m_context;

    bool checkUp( )
    {
        std::time_t now = std::time(nullptr);
    
    // Преобразуем в локальное время
        std::tm* local_time = std::localtime(&now);
    
    // День недели (0-6, где 0 - воскресенье)
        int weekday = local_time->tm_wday;
        return weekday == m_weekday;
    }

    public:
    WeekdayCheckUp( std::string condition, ContextPtr context ){
        m_context = context;

        if( condition == "mon" )
        {
            m_weekday = 1;
            return;
        } 

        if( condition == "tue" )
        {
            m_weekday = 2;
            return;
        }

        if( condition == "wed" )
        {
            m_weekday = 3;
            return;
        }

        if( condition == "thu" )
        {
            m_weekday = 4;
            return;
        } 

        if( condition == "fri" )
        {
            m_weekday = 5;
            return;
        }

        if( condition == "sat" )
        {
            m_weekday = 6;
            return;
        }

        if( condition == "sun" )
        {
            m_weekday = 0;
            return;
        }
    }

    void Execute(){
        
        m_context->next =checkUp();  
        std::cout<<"WeekdayCheckUp "<<m_context->next<<std::endl;  
    }
    
};

class DaytimeCheckUp:public ICommand
{
    ContextPtr m_context;
    int m_daytime;
    const char* days;

    bool checkUp( )
    {
        std::time_t now = std::time(nullptr);
    
    // Преобразуем в локальное время
        std::tm* local_time = std::localtime(&now);
    
    // интересно, по какому времени
        int weekday = local_time->tm_hour > 12;
        return weekday == m_daytime;
    }

    public:
    DaytimeCheckUp( std::string condition , ContextPtr context ){
        m_context = context; 

        if( condition == "pm" )
        {
            m_daytime = 1;
            return;
        } 

        if( condition == "am" )
        {
            m_daytime = 0;
            return;
        }
    }

    void Execute()
    {
        m_context->next = checkUp(); 
        std::cout<<"DaytimeCheckUp"<<m_context->next<<std::endl;        
    }
};




#endif