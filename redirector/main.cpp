
#include <iostream>

using std::cout, std::endl;

#include "get_settings.h"


// Main function to set up the server
int main() {

    read_dependencies("/home/vboxuser/Documents/smart_link/redirector2/settings/settings.json");
        
    try {
        IServerPtr psrv = IoC::Resolve<IServerPtr>( "HttpServer" );  
       
        psrv->Loop();

      
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}