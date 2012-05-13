

#ifndef FAUSTY_WINDOW
#define FAUSTY_WINDOW

#include <gtkmm.h>

#include <gtksourceviewmm.h>

class GWindow
{
  public:
    GWindow();
    
    Gtk::Window& getWindow();
    
  private:
    std::string projectName;
    
    Gtk::Window* window;
    
    Gtk::Box*    topBox;
    Gtk::Box*    mainBox;
    
    Gtk::Button* exportButton;
    Gtk::Button* compileButton;

    Gtk::Image* image;
    
    gtksourceview::SourceView sourceview;
    
    void compile();
    void updateDiagram();
    
  
};

#endif

