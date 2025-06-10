
#ifndef __SESSION__
#define __SESSION__

#include <boost/asio.hpp>

using tcp = boost::asio::ip::tcp;  // from <boost/asio/ip/tcp.hpp>

#include "routinehandler.h"

class StartSessionCmd: public ICommand
{
    std::unique_ptr<ICommand> routin;    
    public:
    StartSessionCmd(tcp::socket  socket)   
    {
       routin = std::make_unique<RoutineHandler>(std::move(socket));
    }
    
    void Execute()
    {        
        std::thread{ 
          [&](ICommandPtr routin)
            {
                routin->Execute();
            }, std::move(routin)
         }.detach();
    }
};

#endif