#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "WordManager.h"
#include "SettingsManager.h"
#include "SettingsDialog.h"

class MainWindow : public Gtk::Window {
private:
	// 添加设置管理器
	SettingsManager settingsManager;
	// 成员变量
	WordManager wordManager;
	Word currentWord;
	int attemptCount;
	
	// 主容器和完成页面容器
	Gtk::Box mainBox;
	Gtk::Box completionBox;
	
	// 学习界面组件
	Gtk::MenuBar menuBar;
	Gtk::Box wordBox;
	Gtk::Label posLabel;
	Gtk::Label meaningLabel;
	Gtk::Label exampleLabel;
	
	Gtk::Box inputBox;
	Gtk::Label inputHintLabel;
	Gtk::Entry answerEntry;
	Gtk::Button submitButton;
	
	Gtk::Label feedbackLabel;
	Gtk::Label attemptLabel;
	Gtk::Label progressLabel;  // 进度标签
	
	Gtk::Box controlBox;
	Gtk::Button showAnswerButton;
	Gtk::Button nextWordButton;
	
	Gtk::Statusbar statusbar;
	
	// 完成页面组件
	Gtk::Label completionTitle;
	Gtk::Label completionStats;
	Gtk::Button restartButton;
	Gtk::Button reviewWrongWordsButton;
	
	// 菜单项
	Gtk::MenuItem fileMenu;
	Gtk::Menu fileSubmenu;
	Gtk::MenuItem openMenuItem;
	Gtk::MenuItem exitMenuItem;
	Gtk::MenuItem resetProgressMenuItem;
	// 添加设置菜单项
	Gtk::MenuItem settingsMenuItem;

public:
	MainWindow();
	virtual ~MainWindow();

private:

	// 添加设置相关的方法
	void on_settings();
	void apply_theme_colors();
	// 信号处理函数
	void on_open_file();
	void on_next_word();
	void on_submit_answer();
	void on_show_answer();
	void on_entry_activate();
	void on_restart();
	void on_review_wrong_words();
	void on_reset_progress();
	void update_display();
	void reset_attempt();
	void show_completion_page();
	void show_learning_page();
	void update_progress();
};

#endif