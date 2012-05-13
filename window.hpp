

#ifndef FAUSTY_WINDOW
#define FAUSTY_WINDOW

#include <gtkmm.h>

#include <gtksourceviewmm.h>

class GWindow
{
  public:
    GWindow();
    
    Gtk::Window& getWindow();
    
    void setLocation(std::string);
    
  private:
    std::string workingDirectory;
    std::string projectName;
    
    std::string lastCompiledString;
    
    Gtk::Window* window;
    
    Gtk::Box*    topBox;
    Gtk::Box*    mainBox;
    
    Gtk::Label*  sideLabel;
    
    Gtk::Button* exportButton;
    Gtk::Button* compileButton;
    
    Gtk::ToggleButton* executeButton;

    Gtk::Image* image;
    
    gtksourceview::SourceView sourceview;
    Glib::RefPtr<gtksourceview::SourceBuffer> buffer;
    
    // for stopping the child process
    Glib::Pid pid;
    
    void run();
    void compile();
    void writeFile();
    void exportCompile();
    void updateDiagram();
    
    // for spawning async functions
    void nullSetupFunction();
};

#endif

