#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <gtkmm.h>
#include "SettingsManager.h"

// 设置对话框类
class SettingsDialog : public Gtk::Dialog {
private:
	SettingsManager& settingsManager;
	
	Gtk::Box mainBox;
	Gtk::Label titleLabel;
	
	// 主题设置
	Gtk::Frame themeFrame;
	Gtk::Box themeBox;
	Gtk::Box darkModeBox;
	Gtk::Label darkModeLabel;
	Gtk::Switch darkModeSwitch;
	
	// 颜色主题设置
	Gtk::Frame colorThemeFrame;
	Gtk::Box colorThemeBox;
	Gtk::Label colorThemeLabel;
	Gtk::ComboBoxText colorThemeCombo;
	
	// 颜色设置
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
	void on_dark_mode_toggled();
	void on_color_theme_changed();
	void load_current_colors();
	void load_current_theme();
};

#endif