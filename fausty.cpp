
// Author: Harry van Haaren
// compile:  g++ -o fausty fausty.cpp  `pkg-config --cflags --libs gtkmm-2.4 gtksourceviewmm-2.0`

#include <iostream>
#include <sstream>

#include <gtkmm.h>

#include "window.hpp"

using namespace std;

// if compiling test.dsp, name should just be "test"
void compile(std::string name)
{
  std::stringstream command;
  
  command << "faust -svg -a jack-gtk.cpp -o ";
  command << name;
  command << ".cpp ";
  command << name;
  command << ".dsp";
  
  int returnStatus = 0;
  std::string outString, errString;
  
  Glib::spawn_command_line_sync( command.str() , &outString, &errString, &returnStatus );
  
  if ( outString.size() > 0 )
    std::cout << "Output: " << outString << endl;
  else if ( errString.size() > 0 )
    std::cout << "Error: " << errString << endl;
  else
    std::cout << "Faust compiled successfully!" << std::endl;
  
  return;
}

void updateDiagram(Gtk::Image* image)
{
  image->set("test-svg/process.svg");
  
  return;
}


int main(int argc, char** argv)
{
  // setup GUI enviroment
  Gtk::Main kit(argc, argv);
  
  GWindow window;
  
  kit.run( window.getWindow() );
}
