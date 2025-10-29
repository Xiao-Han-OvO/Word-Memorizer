#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <gtkmm.h>
#include "SettingsManager.h"

class SettingsDialog : public Gtk::Dialog {
private:
    SettingsManager& settingsManager;
    
    Gtk::Box mainBox;
    Gtk::Label titleLabel;
    
    Gtk::Frame colorFrame;
    Gtk::Box colorBox;
    
    Gtk::Box posColorBox;
    Gtk::Label posColorLabel;
    Gtk::ColorButton posColorButton;
    
    Gtk::Box correctColorBox;
    Gtk::Label correctColorLabel;
    Gtk::ColorButton correctColorButton;
    
    Gtk::Box errorColorBox;
    Gtk::Label errorColorLabel;
    Gtk::ColorButton errorColorButton;
    
    Gtk::Box answerColorBox;
    Gtk::Label answerColorLabel;
    Gtk::ColorButton answerColorButton;
    
    Gtk::Box buttonBox;
    Gtk::Button saveButton;
    Gtk::Button cancelButton;
    Gtk::Button resetButton;

public:
    SettingsDialog(Gtk::Window& parent, SettingsManager& settings);
    virtual ~SettingsDialog();

private:
    void on_save_clicked();
    void on_cancel_clicked();
    void on_reset_clicked();
    void load_current_colors();
};

#endif