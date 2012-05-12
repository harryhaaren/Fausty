
// Author: Harry van Haaren
// compile:  g++ -o fausty fausty.cpp  `pkg-config --cflags --libs gtkmm-2.4 gtksourceviewmm-2.0`

#include <iostream>
#include <sstream>

#include <gtkmm.h>
#include <gtksourceviewmm.h>

using namespace std;

// if compiling test.dsp, name should just be "test"
void compile(std::string name)
{
  std::stringstream command;
  
  command << "faust -a jack-gtk.cpp -o ";
  command << name;
  command << ".cpp ";
  command << name;
  command << ".dsp";
  
  int returnStatus = -1234;
  std::string cout, cerr;
  
  Glib::spawn_command_line_sync( command.str() , &cout, &cerr, &returnStatus );
  
  std::cout << "Faust returned " << returnStatus << std::endl << cout << std::endl<< std::endl << cerr << std::endl;;
}


int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);
  
  gtksourceview::init () ;
  
  Gtk::Window window;
  Gtk::Button button("Compile");
  button.signal_clicked().connect( sigc::bind (sigc::ptr_fun( &compile ), "test") );
  
  gtksourceview::SourceView sourceview ;
  
  Glib::RefPtr<gtksourceview::SourceBuffer> buffer = sourceview.get_source_buffer () ;
  
  if (!buffer) {
    cerr << "gtksourceview::SourceView::get_source_buffer () failed" << std::endl ;
    return -1;
  }
  
  Gtk::Image* image = Gtk::manage(new Gtk::Image());
  image->set( "test.svg" );
  
  
  buffer->set_text ("coin") ;
  
  Gtk::HBox box;
  box.add(sourceview);
  box.add(button);
  box.add(*image);
  
  window.add(box);
  window.set_default_size(400,200);
  window.set_title("Fausty");
  window.show_all();
  
  kit.run();
}
