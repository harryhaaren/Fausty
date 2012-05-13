
// Author: Harry van Haaren
// compile:  ./waf configure
//           ./waf
// run:      ./fausty

#include <iostream>

#include <gtkmm.h>
#include "window.hpp"


int main(int argc, char** argv)
{
  // setup GUI enviroment
  Gtk::Main kit(argc, argv);
  
  GWindow window;
  
  // for spawning async processes later
  std::string location = argv[0];
  window.setLocation(location);
  
  kit.run( window.getWindow() );
}
