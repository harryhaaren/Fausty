
/*
  This file is part of Fausty.
  Copyright 2012, Harry van Haaren

  Fausty is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fausty is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Luppp.  If not, see <http://www.gnu.org/licenses/>.
*/

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
    
    Gtk::ScrolledWindow* svgScrollwindow;
    Gtk::ScrolledWindow* sourceviewScrollwindow;
    
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

