
#include <iostream>
#include <boost/json.hpp>
//#include <memory>
#include <functional>

#include "iconditioncontext.h"

using namespace std;



extern "C"  void Handle( IConditionContextPtr context )
{
        std::string condition = context->getConditionValue();
        bool state = context->getContext()->getNext();

        int  m_age = atoi(condition.c_str());
        int age = 18;
        
        if( condition.find_last_of("+") != std::string::npos )
        { 
                state = (age >= m_age) ;  
        }            
        else 
        if ( condition.find_last_of("-") != std::string::npos )
        {
                state = ( age <= m_age);  
        }
        else    state = (age == m_age) ;   

        context->getContext()->setNext( state );
        
}