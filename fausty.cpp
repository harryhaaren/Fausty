
// Author: Harry van Haaren
// compile:  g++ -o fausty fausty.cpp  `pkg-config --cflags --libs gtkmm-2.4 gtksourceviewmm-2.0`

#include <iostream>

#include <gtkmm.h>
#include <gtksourceviewmm.h>



using namespace std;

int main(int argc, char** argv)
{
  Gtk::Main kit(argc, argv);
  
  gtksourceview::init () ;
  
  Gtk::Window window;
  
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
  box.add(*image);
  
  window.add(box);
  window.set_default_size(400,200);
  window.set_title("Fausty");
  window.show_all();
  
  kit.run();
}
