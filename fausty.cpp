
/*
  This file is part of Fausty.
  Copyright 2012, Harry van Haaren

  Fausty is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fausty is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Luppp.  If not, see <http://www.gnu.org/licenses/>.
*/

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
