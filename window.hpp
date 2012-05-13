

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
    
    Gtk::Window* window;
    Gtk::Button* exportButton;
    Gtk::Button* compileButton;

    Gtk::Image* image;
    
    gtksourceview::SourceView sourceview;
    
    
  
};

#endif

