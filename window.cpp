

#include "window.hpp"

#include <iostream>

using namespace std;

GWindow::GWindow()
{
  cout << "GWindow()" << endl;
  
  image = Gtk::manage(new Gtk::Image());
  //image->set( "test.svg" );
  
  // get the GUI widgets
  Glib::RefPtr<Gtk::Builder> refBuilder;
  try {
    refBuilder = Gtk::Builder::create_from_file("ui.glade");
  }
  catch (Gtk::BuilderError e) {
    std::cout << "Gtk::BuilderError loading mainwindow glade file: " << e.what() << std::endl;
    exit(1);
  }
  catch (Glib::FileError e) {
    std::cout << "Gtk::FileError loading mainwindow glade file: " << e.what() << std::endl;
    exit(1);
  }
  
  // get the widgets
  refBuilder->get_widget("window", window);
  window->set_default_size(400,200);
  window->set_title("Fausty");
  
  
  /*
  button.signal_clicked().connect( sigc::bind (sigc::ptr_fun( &compile       ), "test") );
  button.signal_clicked().connect( sigc::bind (sigc::ptr_fun( &updateDiagram ), image ) );

  
  gtksourceview::init ();
  Glib::RefPtr<gtksourceview::SourceBuffer> buffer = sourceview.get_source_buffer () ;
  
  if (!buffer) {
    cerr << "gtksourceview::SourceView::get_source_buffer () failed" << std::endl ;
    return -1;
  }
  
  // get the file, and read it into the buffer
  //std::string dspCode = Glib::file_get_contents ( "test.dsp" );
  //buffer->set_text ( dspCode );
  
  */
  
  
  window->show_all();
}

Gtk::Window& GWindow::getWindow()
{
  return *window;
}
