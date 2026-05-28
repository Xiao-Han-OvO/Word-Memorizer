#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm.h>
#include "WordManager.h"
#include "SettingsManager.h"
#include "SettingsDialog.h"
#include "SidePanel.h"

class UsageDialog;
class AboutDialog;
class WordManagerDialog;

class MainWindow : public Gtk::Window {
private:
    Gtk::Box mainContainer;       // 垂直容器：标题栏 + 内容区
    Gtk::Box contentArea;         // 水平容器：导航 + 页面栈 + 侧栏
    Gtk::Box leftBox;             // 学习页内容
    SidePanel sidePanel;

    Gtk::Stack pageStack;
    Gtk::Box learningPage;
    Gtk::Box completionPage;
    Gtk::Box placeholderPage;

    bool dragging;
    int drag_x, drag_y;

    WordManager wordManager;
    SettingsManager settingsManager;
    Word currentWord;
    int attemptCount;

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

    Gtk::Box controlBox;
    Gtk::Button showAnswerButton;
    Gtk::Button nextWordButton;

    Gtk::Label completionTitle;
    Gtk::Label completionStats;
    Gtk::Button restartButton;
    Gtk::Button reviewWrongWordsButton;

public:
    MainWindow();
    virtual ~MainWindow();

protected:
    bool on_key_press_event(GdkEventKey* event) override;

private:
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
    void on_usage();
    void update_display();
    void reset_attempt();
    void show_completion_page();
    void show_learning_page();
    void update_progress();
    void apply_theme();
    void toggle_dark_mode();
    void apply_dialog_theme(Gtk::Window& dialog);
    bool is_word_completed() const { return !answerEntry.get_sensitive(); }
};

class WordManagerDialog : public Gtk::Dialog {
private:
    Gtk::Box mainBox;
    Gtk::ScrolledWindow scrolledWindow;
    Gtk::Box wordsContainer;
    std::vector<Gtk::Widget*> wordCards;
    std::string currentFilename;

public:
    WordManagerDialog(Gtk::Window& parent);
    virtual ~WordManagerDialog();
    void loadWordsFromFile(const std::string& filename);
    void saveWordsToFile(const std::string& filename);
    void removeWordCard(Gtk::Widget* card);

private:
    void on_remove_button_clicked();
    void on_add_button_clicked();
    void on_save_all_clicked();
    void on_close_clicked();
    void addWordCard(const std::string& word = "", const std::string& pos = "", 
                    const std::string& meaning = "", const std::string& example = "");
};

#endif
