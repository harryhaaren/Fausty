
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
  
  kit.run( window.getWindow() );
}
