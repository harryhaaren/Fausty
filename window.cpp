

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
  
  
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::writeFile     ) );
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::compile       ) );
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::updateDiagram ) );
  
  exportButton->signal_clicked().connect ( sigc::mem_fun( this, &GWindow::exportCompile ) );
  
  
  gtksourceview::init ();
  buffer = sourceview.get_source_buffer () ;
  
  if (!buffer) {
    cerr << "gtksourceview::SourceView::get_source_buffer () failed" << std::endl ;
  }
  
  mainBox->pack_start( sourceview, true, true );
  mainBox->pack_start( *image,     true, true );
  
  // get the file, and read it into the buffer
  //std::string dspCode = Glib::file_get_contents ( "test.dsp" );
  
  buffer->set_text ( "process = +;" );
  
  projectName = "test";
  
  window->show_all();
}

void GWindow::writeFile()
{
  bool success = g_file_set_contents( "test.dsp",
                                      buffer->get_text().c_str(),
                                      buffer->get_text().size(),
                                      0 );
  
  cout << "Writing file = " << success << endl;
  
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

void GWindow::exportCompile()
{
  // g++ -I/usr/lib/faust/ -lpthread `pkg-config --cflags --libs jack gtk+-2.0` -o $1 $1.cpp
  
  int returnStatus = 0;
  std::string outString, errString;
  
  stringstream pkgConfig;
  pkgConfig << "pkg-config --cflags --libs jack gtk+-2.0";
  string pkgConfigString;
  Glib::spawn_command_line_sync( pkgConfig.str() , &pkgConfigString, &errString, &returnStatus );
  
  stringstream command;
  command << "g++ -I/usr/lib/faust/ -lpthread ";
  command << pkgConfigString;
  command << " -o ";
  command << projectName;
  command << " ";
  command << projectName;
  command << ".cpp";
  
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
