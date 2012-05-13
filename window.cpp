

#include "window.hpp"

#include <iostream>
#include <sstream>

#include <gtkmm/treemodel.h>

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
  window->set_title("Fausty");
  
  refBuilder->get_widget("windowBox", windowBox);
  
  refBuilder->get_widget("topBox", topBox);
  
  refBuilder->get_widget("mainBox", mainBox);
  
  refBuilder->get_widget("errorLabel", errorLabel);
  windowBox->show_all();
  
  // combo box widget
  refBuilder->get_widget("faustTargetBox", faustTargetBox);
  
  listStore = Gtk::ListStore::create( m_Columns );
  faustTargetBox->set_model( listStore );
  
  //Glib::RefPtr<Gtk::ListStore> listStore;
  Gtk::TreeModel::Row row = *( listStore->append() );
  row[m_Columns.m_col_id] = 0;
  row[m_Columns.m_col_name] = "Jack GTK";
  
  row = *( listStore->append());
  row[m_Columns.m_col_id] = 1;
  row[m_Columns.m_col_name] = "LV2 Effect";
  
  row = *( listStore->append());
  row[m_Columns.m_col_id] = 2;
  row[m_Columns.m_col_name] = "LV2 Synth";
  
  // can show numbers beside the names, but no need
  //faustTargetBox->pack_start(m_Columns.m_col_id);
  faustTargetBox->pack_start(m_Columns.m_col_name);
  faustTargetBox->set_active(0);
  
  faustTargetBox->signal_changed().connect( sigc::mem_fun(*this, &GWindow::targetChanged) );
  
  
  // compile widgets
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
  mainBox->pack_start( *image,     false, false );
  
  // get the file, and read it into the buffer
  //std::string dspCode = Glib::file_get_contents ( "test.dsp" );
  
  buffer->set_text ( "process = +;" );
  
  projectName   = "test";
  projectTarget = FAUST_TARGET_JACK_GTK;
  
  // get the SVG for the default buffer up
  compile();
  
  window->set_default_size(650,400);
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
  cout << "Generating Faust dsp code..." << flush;
  
  // logic behind "faust" compiler command:
  // are we running JACK & GTK, or LV2 targets?
  
  string architecture;
  
  switch ( projectTarget )
  {
    case FAUST_TARGET_JACK_GTK:     architecture = "jack-gtk.cpp"; break;
    case FAUST_TARGET_LV2_SYNTH:    architecture = "lv2synth.cpp"; break;
    case FAUST_TARGET_LV2_EFFECT:   architecture = "lv2.cpp";      break;
  }
  
  cout << "Architecture is " << architecture << endl;
  
  std::stringstream command;
  command << "faust -svg -a ";
  command << architecture;
  command << " -o ";
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
  {
    // faust error, send to label box
    std::cout << "Error: " << errString << endl;
    errorLabel->set_text( errString );
  }
  else
  {
    std::cout << "SVG diagram & C++ code generated successfully!" << std::endl;
    stringstream filename;
    filename << projectName;
    filename << ".dsp";
    
    // store the compiled buffer, so we can compare with it later
    lastCompiledString = Glib::file_get_contents ( filename.str() );
    
    // clear the errors
    errorLabel->set_text( "" );
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

void GWindow::targetChanged()
{
  // the rows in the combobox match the numbers of the Target enum, so
  // we just set the target to a static cast of the row
  int row = faustTargetBox->get_active_row_number();
  
  projectTarget = static_cast<FaustTarget>(row);
  
  cout << "Target changed to " << row << endl;
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
