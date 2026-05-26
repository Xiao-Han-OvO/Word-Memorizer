#ifndef MAINWINDOW_UBUNTU20_H
#define MAINWINDUBUNTU20_H

#include <gtkmm.h>
#include "WordManager.h"

class MainWindow : public Gtk::Window {
private:
    WordManager wordManager;
    Word currentWord;
    int attemptCount;
    
    // 简化界面组件
    Gtk::Box mainBox;
    Gtk::MenuBar menuBar;
    Gtk::Label meaningLabel;
    Gtk::Label exampleLabel;
    Gtk::Entry answerEntry;
    Gtk::Label feedbackLabel;
    Gtk::Label progressLabel;
    Gtk::Button submitButton;
    Gtk::Button nextButton;
    Gtk::Button showAnswerButton;
    
    Gtk::MenuItem fileMenu;
    Gtk::Menu fileSubmenu;
    Gtk::MenuItem openMenuItem;
    Gtk::MenuItem exitMenuItem;

public:
    MainWindow();
    virtual ~MainWindow();

private:
    void on_open_file();
    void on_next_word();
    void on_submit_answer();
    void on_show_answer();
    void update_display();
    void update_progress();
};

#endif