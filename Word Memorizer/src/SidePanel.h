#ifndef SIDEPANEL_H
#define SIDEPANEL_H

#include <gtkmm.h>
#include "WordManager.h"
#include "SettingsManager.h"

class SidePanel : public Gtk::Box {
public:
    SidePanel(WordManager& wm, SettingsManager& sm);
    virtual ~SidePanel();

    void update_stats();
    void set_current_word_status(bool has_failed);
    void set_dark_mode_button(bool dark);

    // 信号：快捷操作
    sigc::signal<void> signal_toggle_dark_mode();
    sigc::signal<void> signal_open_settings();
    sigc::signal<void> signal_open_wrong_words();

private:
    WordManager& wordManager;
    SettingsManager& settingsManager;

    // 进度条
    Gtk::ProgressBar progressBar;
    // 统计标签
    Gtk::Label masteredLabel;
    Gtk::Label totalLabel;
    Gtk::Label wrongLabel;
    Gtk::Label remainingLabel;
    // 当前单词状态
    Gtk::Label currentStatusLabel;
    // 快捷按钮
    Gtk::Button darkModeButton;
    Gtk::Button settingsButton;
    Gtk::Button wrongWordsButton;

    void update_progress();
    void on_dark_mode_clicked();
    void on_settings_clicked();
    void on_wrong_words_clicked();

    sigc::signal<void> m_toggle_dark_mode;
    sigc::signal<void> m_open_settings;
    sigc::signal<void> m_open_wrong_words;
};

#endif
