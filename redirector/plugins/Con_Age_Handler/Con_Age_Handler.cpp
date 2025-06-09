
#include <iostream>
#include <boost/json.hpp>
//#include <memory>
#include <functional>

#include "context.h"

using namespace std;



extern "C"  void Handle( ConditionContextPtr context )
{
        int  m_age = atoi(context->condition.c_str());
        int age = 18;
        
        if( context->condition.find_last_of("+") != std::string::npos )
        { 
                context->cntxt->next = (age >= m_age) ;  
        }            
        else if (context->condition.find_last_of("-") != std::string::npos )
        {
                context->cntxt->next = ( age <= m_age);  
        }
        else    context->cntxt->next = (age == m_age) ;   
        
}