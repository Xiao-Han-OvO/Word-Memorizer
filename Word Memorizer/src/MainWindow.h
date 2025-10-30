#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "WordManager.h"
#include "SettingsManager.h"
#include "SettingsDialog.h"

class UsageDialog;
class AboutDialog;  // 新增前向声明

class WordManagerDialog : public Gtk::Dialog {
private:
    Gtk::Box mainBox;
    Gtk::ScrolledWindow scrolledWindow;
    Gtk::Box wordsContainer;
    Gtk::Button addButton;
    Gtk::Button saveAllButton;
    Gtk::Button closeButton;
    
    std::vector<Gtk::Widget*> wordCards;
    std::string currentFilename;

public:
    WordManagerDialog(Gtk::Window& parent);
    virtual ~WordManagerDialog();
    
    void loadWordsFromFile(const std::string& filename);
    void saveWordsToFile(const std::string& filename);
    void removeWordCard(Gtk::Widget* card);

private:
    void on_remove_button_clicked();  // 新增
    void on_add_button_clicked();
    void on_save_all_clicked();
    void on_close_clicked();
    void addWordCard(const std::string& word = "", const std::string& pos = "", 
                    const std::string& meaning = "", const std::string& example = "");
};

class MainWindow : public Gtk::Window {
private:
    // 拖动状态变量
    bool dragging;
    int drag_x, drag_y;
    // 成员变量
    WordManager wordManager;
    SettingsManager settingsManager;
    Word currentWord;
    int attemptCount;
    
    // 主容器和完成页面容器
    Gtk::Box mainBox;
    Gtk::Box completionBox;
    Gtk::ScrolledWindow scrolledWindow;
    Gtk::Label contentLabel;
    Gtk::Button closeButton;
    
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
    Gtk::Label statusLabel;
    Gtk::Label progressLabel;
    Gtk::ProgressBar progressBar;
    
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
    Gtk::MenuItem editWordsMenuItem;
    Gtk::MenuItem resetProgressMenuItem;
    Gtk::MenuItem settingsMenuItem;
    Gtk::MenuItem exitMenuItem;
    
    Gtk::MenuItem viewMenu;
    Gtk::Menu viewSubmenu;
    Gtk::MenuItem wrongWordsMenuItem;
    Gtk::MenuItem clearWrongWordsMenuItem;
    Gtk::MenuItem exportWrongWordsMenuItem;
    Gtk::MenuItem debugMenuItem;
    
    Gtk::MenuItem helpMenu;
    Gtk::Menu helpSubmenu;
    Gtk::MenuItem usageMenuItem;
    Gtk::MenuItem aboutMenuItem;

public:
    MainWindow();
    virtual ~MainWindow();

protected:
    virtual bool on_key_press_event(GdkEventKey* event) override;

private:
    // 信号处理函数
    void on_open_file();
    void on_edit_words();
    void on_next_word();
    void on_submit_answer();
    void on_show_answer();
    void on_entry_activate();
    void on_restart();
    void on_review_wrong_words();
    void on_clear_wrong_words();
    void on_export_wrong_words();
    void on_reset_progress();
    void on_settings();
    void on_debug_info();
    void on_about();
    void update_display();
    void reset_attempt();
    void show_completion_page();
    void show_learning_page();
    void update_progress();
    void apply_dialog_theme(Gtk::Window& dialog);
    void apply_theme_colors();
    void apply_theme();
    void toggle_dark_mode();
    void update_css_style();
    void on_usage();  // 新增使用说明对话框
    bool is_word_completed() const {
        return !answerEntry.get_sensitive(); // 输入框禁用表示单词已完成
    }
};

#endif
