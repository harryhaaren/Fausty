
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
  Gtk::Main kit(argc, argv);
  
  gtksourceview::init () ;
  
  Gtk::Window window;
  Gtk::Button button("Compile");
  
  Gtk::Image* image = Gtk::manage(new Gtk::Image());
  image->set( "test.svg" );
  
  button.signal_clicked().connect( sigc::bind (sigc::ptr_fun( &compile       ), "test") );
  button.signal_clicked().connect( sigc::bind (sigc::ptr_fun( &updateDiagram ), image ) );
  
  gtksourceview::SourceView sourceview ;
  
  Glib::RefPtr<gtksourceview::SourceBuffer> buffer = sourceview.get_source_buffer () ;
  
  if (!buffer) {
    cerr << "gtksourceview::SourceView::get_source_buffer () failed" << std::endl ;
    return -1;
  }
  
  
  // get the file, and read it into the buffer
  std::string dspCode = Glib::file_get_contents ( "test.dsp" );
  
  buffer->set_text ( dspCode );
  
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
