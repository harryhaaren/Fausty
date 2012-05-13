

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
    enum FaustTarget {
      FAUST_TARGET_JACK_GTK = 0,
      FAUST_TARGET_LV2_EFFECT,
      FAUST_TARGET_LV2_SYNTH,
    };
    
    class TargetColums : public Gtk::TreeModel::ColumnRecord
    {
      public:
        TargetColums() { add(m_col_id); add(m_col_name); }

        Gtk::TreeModelColumn<int> m_col_id;
        Gtk::TreeModelColumn<Glib::ustring> m_col_name;
    };

    TargetColums m_Columns;
    
    FaustTarget projectTarget;
    
    std::string workingDirectory;
    std::string projectName;
    
    std::string lastCompiledString;
    
    Gtk::Window* window;
    Gtk::Box*    windowBox;
    
    Gtk::Box*    topBox;
    Gtk::Box*    mainBox;
    
    Gtk::Label*  sideLabel;
    Gtk::Label*  errorLabel;
    
    Gtk::ComboBox* faustTargetBox;
    Glib::RefPtr<Gtk::ListStore> listStore;
    
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
    
    // for target dropdown
    void targetChanged();
};

#endif

