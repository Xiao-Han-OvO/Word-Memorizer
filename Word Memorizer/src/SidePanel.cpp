#include "SidePanel.h"
#include "IconHelper.h"

SidePanel::SidePanel(WordManager& wm, SettingsManager& sm)
    : Gtk::Box(Gtk::ORIENTATION_VERTICAL, 0),
      wordManager(wm),
      settingsManager(sm) {
    
    set_size_request(220, -1);
    get_style_context()->add_class("side-panel");
    set_border_width(16);
    
    auto* progressTitle = Gtk::manage(new Gtk::Label("学习进度"));
    progressTitle->get_style_context()->add_class("stat-label");
    progressTitle->set_halign(Gtk::ALIGN_START);
    progressTitle->set_margin_bottom(8);
    pack_start(*progressTitle, Gtk::PACK_SHRINK);
    
    progressBar.get_style_context()->add_class("glass-progress");
    progressBar.set_show_text(true);
    progressBar.set_fraction(0.0);
    progressBar.set_size_request(-1, 16);
    progressBar.set_margin_bottom(16);
    pack_start(progressBar, Gtk::PACK_SHRINK);
    
    auto* statsFrame = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 8));
    
    auto* masteredCard = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 8));
    masteredCard->get_style_context()->add_class("stat-card");
    masteredCard->pack_start(*IconHelper::create_icon(IconHelper::CHECK, 18), Gtk::PACK_SHRINK);
    masteredLabel.get_style_context()->add_class("stat-value");
    masteredLabel.set_label("0");
    masteredCard->pack_start(masteredLabel, Gtk::PACK_SHRINK);
    auto* masteredText = Gtk::manage(new Gtk::Label("已掌握"));
    masteredText->get_style_context()->add_class("stat-label");
    masteredCard->pack_end(*masteredText, Gtk::PACK_SHRINK);
    statsFrame->pack_start(*masteredCard, Gtk::PACK_SHRINK);
    
    auto* totalCard = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 8));
    totalCard->get_style_context()->add_class("stat-card");
    totalCard->pack_start(*IconHelper::create_icon(IconHelper::INFO, 18), Gtk::PACK_SHRINK);
    totalLabel.get_style_context()->add_class("stat-value");
    totalLabel.set_label("0");
    totalCard->pack_start(totalLabel, Gtk::PACK_SHRINK);
    auto* totalText = Gtk::manage(new Gtk::Label("总单词"));
    totalText->get_style_context()->add_class("stat-label");
    totalCard->pack_end(*totalText, Gtk::PACK_SHRINK);
    statsFrame->pack_start(*totalCard, Gtk::PACK_SHRINK);
    
    auto* wrongCard = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 8));
    wrongCard->get_style_context()->add_class("stat-card");
    wrongCard->pack_start(*IconHelper::create_icon(IconHelper::X_CIRCLE, 18), Gtk::PACK_SHRINK);
    wrongLabel.get_style_context()->add_class("stat-value");
    wrongLabel.set_label("0");
    wrongCard->pack_start(wrongLabel, Gtk::PACK_SHRINK);
    auto* wrongText = Gtk::manage(new Gtk::Label("错词"));
    wrongText->get_style_context()->add_class("stat-label");
    wrongCard->pack_end(*wrongText, Gtk::PACK_SHRINK);
    statsFrame->pack_start(*wrongCard, Gtk::PACK_SHRINK);
    
    auto* remainingCard = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 8));
    remainingCard->get_style_context()->add_class("stat-card");
    remainingCard->pack_start(*IconHelper::create_icon(IconHelper::SEARCH, 18), Gtk::PACK_SHRINK);
    remainingLabel.get_style_context()->add_class("stat-value");
    remainingLabel.set_label("0");
    remainingCard->pack_start(remainingLabel, Gtk::PACK_SHRINK);
    auto* remainingText = Gtk::manage(new Gtk::Label("剩余"));
    remainingText->get_style_context()->add_class("stat-label");
    remainingCard->pack_end(*remainingText, Gtk::PACK_SHRINK);
    statsFrame->pack_start(*remainingCard, Gtk::PACK_SHRINK);
    
    pack_start(*statsFrame, Gtk::PACK_SHRINK);
    
    auto* separator = Gtk::manage(new Gtk::Separator(Gtk::ORIENTATION_HORIZONTAL));
    separator->set_margin_top(16);
    separator->set_margin_bottom(16);
    pack_start(*separator, Gtk::PACK_SHRINK);
    
    currentStatusLabel.set_line_wrap(true);
    currentStatusLabel.set_margin_bottom(16);
    currentStatusLabel.set_halign(Gtk::ALIGN_START);
    pack_start(currentStatusLabel, Gtk::PACK_SHRINK);
    
    // 使用 Gtk::Button 的 set_image() 方法，以便后续替换
    darkModeButton.set_image(*IconHelper::create_icon(IconHelper::MOON, 18));
    darkModeButton.set_tooltip_text("切换深色模式");
    darkModeButton.get_style_context()->add_class("glass-btn");
    darkModeButton.set_margin_bottom(4);
    pack_start(darkModeButton, Gtk::PACK_SHRINK);
    
    settingsButton.set_image(*IconHelper::create_icon(IconHelper::SETTINGS, 18));
    settingsButton.set_tooltip_text("设置");
    settingsButton.get_style_context()->add_class("glass-btn");
    settingsButton.set_margin_bottom(4);
    pack_start(settingsButton, Gtk::PACK_SHRINK);
    
    wrongWordsButton.set_image(*IconHelper::create_icon(IconHelper::X_CIRCLE, 18));
    wrongWordsButton.set_tooltip_text("错词本");
    wrongWordsButton.get_style_context()->add_class("glass-btn");
    pack_start(wrongWordsButton, Gtk::PACK_SHRINK);
    
    darkModeButton.signal_clicked().connect(sigc::mem_fun(*this, &SidePanel::on_dark_mode_clicked));
    settingsButton.signal_clicked().connect(sigc::mem_fun(*this, &SidePanel::on_settings_clicked));
    wrongWordsButton.signal_clicked().connect(sigc::mem_fun(*this, &SidePanel::on_wrong_words_clicked));
    
    show_all();
}

SidePanel::~SidePanel() {}

void SidePanel::update_stats() {
    int total = wordManager.getTotalWords();
    int mastered = wordManager.getMasteredWordsCount();
    int wrong = wordManager.getWrongWordsCount();
    int remaining = wordManager.getRemainingWordsCount();
    
    masteredLabel.set_label(std::to_string(mastered));
    totalLabel.set_label(std::to_string(total));
    wrongLabel.set_label(std::to_string(wrong));
    remainingLabel.set_label(std::to_string(remaining));
    
    update_progress();
}

void SidePanel::update_progress() {
    float progress = 0.0f;
    if (wordManager.getTotalWords() > 0) {
        progress = static_cast<float>(wordManager.getMasteredWordsCount()) / wordManager.getTotalWords();
    }
    progressBar.set_fraction(progress);
    int pct = static_cast<int>(progress * 100);
    progressBar.set_text(std::to_string(pct) + "%");
}

void SidePanel::set_current_word_status(bool has_failed) {
    if (has_failed) {
        currentStatusLabel.set_label("这个单词你曾经拼错过");
        currentStatusLabel.override_color(Gdk::RGBA("#e67e22"));
    } else {
        currentStatusLabel.set_label("");
    }
}

void SidePanel::set_dark_mode_button(bool dark) {
    // 使用 set_image 替换图标
    darkModeButton.set_image(*IconHelper::create_icon(
        dark ? IconHelper::SUN : IconHelper::MOON, 18));
}

void SidePanel::on_dark_mode_clicked() { m_toggle_dark_mode.emit(); }
void SidePanel::on_settings_clicked() { m_open_settings.emit(); }
void SidePanel::on_wrong_words_clicked() { m_open_wrong_words.emit(); }

sigc::signal<void> SidePanel::signal_toggle_dark_mode() { return m_toggle_dark_mode; }
sigc::signal<void> SidePanel::signal_open_settings() { return m_open_settings; }
sigc::signal<void> SidePanel::signal_open_wrong_words() { return m_open_wrong_words; }
