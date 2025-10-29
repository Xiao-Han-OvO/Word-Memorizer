#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "WordManager.h"
#include "SettingsManager.h"
#include "SettingsDialog.h"

class MainWindow : public Gtk::Window {
private:
    // 成员变量
    WordManager wordManager;
    SettingsManager settingsManager;
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
    Gtk::Label statusLabel;      // 状态标签
    Gtk::Label progressLabel;    // 进度标签
    
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
    Gtk::MenuItem resetProgressMenuItem;
    Gtk::MenuItem settingsMenuItem;
    Gtk::MenuItem exitMenuItem;
    
    Gtk::MenuItem viewMenu;
    Gtk::Menu viewSubmenu;
    Gtk::MenuItem wrongWordsMenuItem;
    Gtk::MenuItem clearWrongWordsMenuItem;
    Gtk::MenuItem debugMenuItem;

public:
    MainWindow();
    virtual ~MainWindow();

private:
    // 信号处理函数
    void on_open_file();
    void on_next_word();
    void on_submit_answer();
    void on_show_answer();
    void on_entry_activate();
    void on_restart();
    void on_review_wrong_words();
    void on_clear_wrong_words();
    void on_reset_progress();
    void on_settings();
    void on_debug_info();
    void update_display();
    void reset_attempt();
    void show_completion_page();
    void show_learning_page();
    void update_progress();
    void apply_theme_colors();
};

#endif