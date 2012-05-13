

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
  
  refBuilder->get_widget("applyButton"  , compileButton);
  refBuilder->get_widget("compileButton", exportButton );
  refBuilder->get_widget("executeButton", executeButton);
  
  
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::writeFile     ) );
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::compile       ) );
  compileButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::updateDiagram ) );
  
  exportButton->signal_clicked().connect ( sigc::mem_fun( this, &GWindow::exportCompile ) );
  
  executeButton->signal_clicked().connect( sigc::mem_fun( this, &GWindow::run           ) );
  
  
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
  cout << "Generating Faust dsp code..." << endl;
  
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
  {
    std::cout << "SVG diagram & C++ code generated successfully!" << std::endl;
    stringstream filename;
    filename << projectName;
    filename << ".dsp";
    
    // store the compiled buffer, so we can compare with it later
    lastCompiledString = Glib::file_get_contents ( filename.str() );
  }
  return;
}

void GWindow::exportCompile()
{
  cout << "Compiling C++ code now..." << endl;
  
  // example command line to compile a faust app using the JACK GTK architecture
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
    std::cout << "C++ code compiled into binary!" << std::endl;
  
  return;
}

void GWindow::run()
{
  // spawn or kill?
  if ( !executeButton->get_active() )
  {
    cout << "Killing spawned process..." << flush;
    
    // doesn't seem to work...
    Glib::spawn_close_pid(pid);
    
    // so do a sync call to "kill" to make sure its gone
    int returnStatus = 0;
    std::string outString, errString;
    stringstream command;
    command << "kill " << pid;
    cout << "  running " << command.str() << flush;
    Glib::spawn_command_line_sync( command.str() , &outString, &errString, &returnStatus );
    cout << " :)" << endl;
    
    return;
  }
  
  
  
  // ensure that we have the lates version to run
  if ( lastCompiledString != buffer->get_text() )
  {
    writeFile();
    compile();
    exportCompile();
  }
  
  int returnStatus = 0;
  std::string outString, errString;
  
  stringstream command;
  command << "./";
  command << projectName;
  
  // works, but all output gets dumped to Fausty's stdout... nasty
  // also can't kill the process when its going...
  //Glib::spawn_command_line_async 	( command.str() );
  
  string workingDir = "./";
  
  vector<string> commandVector;
  commandVector.push_back("./test");
  
  cout << "Spawning async process now..." << flush;
  Glib::spawn_async( workingDir, commandVector, Glib::SPAWN_DO_NOT_REAP_CHILD | Glib::SPAWN_SEARCH_PATH | Glib::SPAWN_STDERR_TO_DEV_NULL | Glib::SPAWN_STDOUT_TO_DEV_NULL, sigc::mem_fun( this, &GWindow::nullSetupFunction), &pid );
  cout << "done! Pid = " << pid << " :)" << endl;
}

void GWindow::nullSetupFunction()
{
  // for spawning async processes
}

void GWindow::setLocation(std::string loc)
{
  workingDirectory = loc;
  cout << "Fausty: Working directory " << workingDirectory << endl;
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
