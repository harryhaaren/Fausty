

#include "window.hpp"

#include <iostream>
#include <sstream>

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
  
  refBuilder->get_widget("topBox", topBox);
  
  refBuilder->get_widget("mainBox", mainBox);
  
  refBuilder->get_widget("applyButton", compileButton);
  
  refBuilder->get_widget("exportButton", exportButton);
  
  
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::compile       ) );
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::updateDiagram ) );
  
  /*
  
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
  
  projectName = "test";
  
  
  window->show_all();
}

void GWindow::compile()
{
  std::stringstream command;
  
  command << "faust -svg -a jack-gtk.cpp -o ";
  command << projectName;
  command << ".cpp ";
  command << projectName;
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

void GWindow::updateDiagram()
{
  stringstream dir;
  dir << projectName;
  dir << "-svg/process.svg";
  image->set( dir.str() );
  return;
}

Gtk::Window& GWindow::getWindow()
{
  return *window;
}
