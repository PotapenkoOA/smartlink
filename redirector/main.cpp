
#include <iostream>

using std::cout, std::endl;

#include "get_settings.h"


// Main function to set up the server
int main() {

    ReadSettings::read_dependencies("./settings.json");
        
    try {
        IServerPtr psrv = IoC::Resolve<IServerPtr>( "HttpServer" );  
       
        psrv->Loop();
      
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}