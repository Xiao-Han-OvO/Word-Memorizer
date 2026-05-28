#include "MainWindow.h"
#include "SettingsDialog.h"
#include "UsageDialog.h"
#include "AboutDialog.h"
#include "IconHelper.h"
#include "GlassTheme.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

MainWindow::MainWindow()
    : mainContainer(Gtk::ORIENTATION_VERTICAL),
      contentArea(Gtk::ORIENTATION_HORIZONTAL),
      leftBox(Gtk::ORIENTATION_VERTICAL),
      sidePanel(wordManager, settingsManager),
      learningPage(Gtk::ORIENTATION_VERTICAL),
      completionPage(Gtk::ORIENTATION_VERTICAL),
      placeholderPage(Gtk::ORIENTATION_VERTICAL),
      wordBox(Gtk::ORIENTATION_VERTICAL),
      inputBox(Gtk::ORIENTATION_HORIZONTAL),
      controlBox(Gtk::ORIENTATION_HORIZONTAL),
      attemptCount(0),
      dragging(false),
      drag_x(0),
      drag_y(0) {

    set_title("VocabMemster");
    set_default_size(900, 600);
    set_decorated(false);
    get_style_context()->add_class("light-glass");

    // ========== 顶部全局标题栏 ==========
    auto* titlebarEventBox = Gtk::manage(new Gtk::EventBox());
    titlebarEventBox->set_hexpand(true);
    titlebarEventBox->get_style_context()->add_class("custom-titlebar-drag-area");

    auto* titlebarBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
    titlebarBox->set_border_width(4);
    titlebarBox->get_style_context()->add_class("custom-titlebar");

    auto* windowTitle = Gtk::manage(new Gtk::Label("VocabMemster"));
    windowTitle->set_halign(Gtk::ALIGN_START);
    windowTitle->set_hexpand(true);
    windowTitle->get_style_context()->add_class("custom-titlebar-label");

    auto* windowControls = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2));
    auto* minimizeBtn = Gtk::manage(new Gtk::Button("−"));
    minimizeBtn->get_style_context()->add_class("custom-window-control");
    minimizeBtn->set_tooltip_text("最小化");
    auto* closeBtn = Gtk::manage(new Gtk::Button("×"));
    closeBtn->get_style_context()->add_class("custom-window-control");
    closeBtn->get_style_context()->add_class("close");
    closeBtn->set_tooltip_text("关闭");

    windowControls->pack_start(*minimizeBtn, Gtk::PACK_SHRINK);
    windowControls->pack_start(*closeBtn, Gtk::PACK_SHRINK);
    titlebarBox->pack_start(*windowTitle, Gtk::PACK_EXPAND_WIDGET);
    titlebarBox->pack_end(*windowControls, Gtk::PACK_SHRINK);
    titlebarEventBox->add(*titlebarBox);

    // 拖动
    titlebarEventBox->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);
    titlebarEventBox->signal_button_press_event().connect([this](GdkEventButton* event) {
        if (event->button == GDK_BUTTON_PRIMARY) {
            dragging = true;
            int x, y;
            get_position(x, y);
            drag_x = event->x_root - x;
            drag_y = event->y_root - y;
            return true;
        }
        return false;
    });
    titlebarEventBox->signal_button_release_event().connect([this](GdkEventButton* event) {
        if (event->button == GDK_BUTTON_PRIMARY) { dragging = false; return true; }
        return false;
    });
    titlebarEventBox->signal_motion_notify_event().connect([this](GdkEventMotion* event) {
        if (dragging) {
            int new_x = event->x_root - drag_x;
            int new_y = event->y_root - drag_y;
            move(new_x, new_y);
            return true;
        }
        return false;
    });
    minimizeBtn->signal_clicked().connect([this]() { iconify(); });
    closeBtn->signal_clicked().connect([this]() { hide(); });

    mainContainer.pack_start(*titlebarEventBox, Gtk::PACK_SHRINK);

    // ========== 左侧导航栏（始终可见，不随页面切换隐藏） ==========
    auto* navBar = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    navBar->get_style_context()->add_class("side-navbar");
    navBar->set_size_request(60, -1);

    auto make_nav_button = [](const std::string& icon, const std::string& label) -> Gtk::Button* {
        auto* btn = Gtk::manage(new Gtk::Button());
        btn->get_style_context()->add_class("nav-button");
        auto* box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 2));
        box->pack_start(*IconHelper::create_icon(icon, 20), Gtk::PACK_SHRINK);
        auto* lbl = Gtk::manage(new Gtk::Label(label));
        lbl->get_style_context()->add_class("nav-button-label");
        box->pack_start(*lbl, Gtk::PACK_SHRINK);
        btn->add(*box);
        btn->show_all();
        return btn;
    };

    auto* openBtn = make_nav_button(IconHelper::FOLDER_OPEN, "打开");
    auto* helpBtn = make_nav_button(IconHelper::INFO, "帮助");
    auto* aboutBtn = make_nav_button(IconHelper::INFO, "关于");

    navBar->pack_start(*openBtn, Gtk::PACK_SHRINK);
    navBar->pack_start(*helpBtn, Gtk::PACK_SHRINK);
    navBar->pack_start(*aboutBtn, Gtk::PACK_SHRINK);

    openBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_open_file));
    helpBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_usage));
    aboutBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_about));

    // ========== 学习页 ==========
    leftBox.set_orientation(Gtk::ORIENTATION_VERTICAL);

    wordBox.set_spacing(15);
    wordBox.set_border_width(20);
    wordBox.set_halign(Gtk::ALIGN_CENTER);
    wordBox.get_style_context()->add_class("glass-card");

    posLabel.set_justify(Gtk::JUSTIFY_CENTER);
    posLabel.override_font(Pango::FontDescription("Sans Bold 14"));
    posLabel.set_margin_bottom(10);
    meaningLabel.set_justify(Gtk::JUSTIFY_CENTER);
    meaningLabel.override_font(Pango::FontDescription("Sans Bold 20"));
    meaningLabel.set_margin_bottom(15);
    exampleLabel.set_justify(Gtk::JUSTIFY_CENTER);
    exampleLabel.override_font(Pango::FontDescription("Sans 12"));
    exampleLabel.set_line_wrap(true);
    exampleLabel.set_max_width_chars(40);

    wordBox.pack_start(posLabel, Gtk::PACK_SHRINK);
    wordBox.pack_start(meaningLabel, Gtk::PACK_SHRINK);
    wordBox.pack_start(exampleLabel, Gtk::PACK_SHRINK);
    leftBox.pack_start(wordBox, Gtk::PACK_SHRINK);

    inputBox.set_spacing(10);
    inputBox.set_border_width(15);
    inputBox.set_halign(Gtk::ALIGN_CENTER);
    inputHintLabel.set_label("请输入英文单词:");
    answerEntry.get_style_context()->add_class("glass-entry");
    answerEntry.set_placeholder_text("在这里输入答案...");
    answerEntry.set_size_request(200, 36);
    submitButton.get_style_context()->add_class("glass-btn");
    submitButton.set_label("提交");
    inputBox.pack_start(inputHintLabel, Gtk::PACK_SHRINK);
    inputBox.pack_start(answerEntry, Gtk::PACK_SHRINK);
    inputBox.pack_start(submitButton, Gtk::PACK_SHRINK);
    leftBox.pack_start(inputBox, Gtk::PACK_SHRINK);

    feedbackLabel.get_style_context()->add_class("feedback-label");
    feedbackLabel.set_justify(Gtk::JUSTIFY_CENTER);
    attemptLabel.set_justify(Gtk::JUSTIFY_CENTER);
    leftBox.pack_start(feedbackLabel, Gtk::PACK_SHRINK);
    leftBox.pack_start(attemptLabel, Gtk::PACK_SHRINK);

    controlBox.set_spacing(10);
    controlBox.set_border_width(10);
    controlBox.set_halign(Gtk::ALIGN_CENTER);
    showAnswerButton.get_style_context()->add_class("glass-btn-secondary");
    showAnswerButton.set_label("查看答案");
    nextWordButton.get_style_context()->add_class("glass-btn");
    nextWordButton.set_label("下一个单词");
    controlBox.pack_start(showAnswerButton, Gtk::PACK_SHRINK);
    controlBox.pack_start(nextWordButton, Gtk::PACK_SHRINK);
    leftBox.pack_start(controlBox, Gtk::PACK_SHRINK);

    learningPage.pack_start(leftBox, Gtk::PACK_EXPAND_WIDGET);

    // ========== 占位页 ==========
    auto* placeholderBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    placeholderBox->set_halign(Gtk::ALIGN_CENTER);
    placeholderBox->set_valign(Gtk::ALIGN_CENTER);
    placeholderBox->set_vexpand(true);
    placeholderBox->get_style_context()->add_class("placeholder-page");
    auto* alertIcon = IconHelper::create_icon(IconHelper::ALERT_TRIANGLE, 64);
    alertIcon->get_style_context()->add_class("placeholder-icon");
    placeholderBox->pack_start(*alertIcon, Gtk::PACK_SHRINK);
    auto* placeholderLabel = Gtk::manage(new Gtk::Label("暂未打开单词本\n通过左侧「打开」按钮选择词库开始学习"));
    placeholderLabel->set_justify(Gtk::JUSTIFY_CENTER);
    placeholderLabel->override_font(Pango::FontDescription("Sans 14"));
    placeholderLabel->get_style_context()->add_class("placeholder-text");
    placeholderBox->pack_start(*placeholderLabel, Gtk::PACK_SHRINK);
    placeholderPage.pack_start(*placeholderBox, Gtk::PACK_EXPAND_WIDGET);

    // ========== 完成页 ==========
    auto* completionContent = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    completionContent->set_halign(Gtk::ALIGN_CENTER);
    completionContent->set_valign(Gtk::ALIGN_CENTER);
    completionContent->set_vexpand(true);
    completionContent->get_style_context()->add_class("glass-card");
    completionTitle.set_label("恭喜你！");
    completionTitle.override_font(Pango::FontDescription("Sans Bold 28"));
    completionStats.set_justify(Gtk::JUSTIFY_CENTER);
    completionStats.override_font(Pango::FontDescription("Sans 16"));
    restartButton.get_style_context()->add_class("glass-btn");
    restartButton.set_label("重新开始");
    reviewWrongWordsButton.get_style_context()->add_class("glass-btn-secondary");
    reviewWrongWordsButton.set_label("复习错词本");
    completionContent->pack_start(completionTitle, Gtk::PACK_SHRINK);
    completionContent->pack_start(completionStats, Gtk::PACK_SHRINK);
    completionContent->pack_start(restartButton, Gtk::PACK_SHRINK);
    completionContent->pack_start(reviewWrongWordsButton, Gtk::PACK_SHRINK);
    completionPage.pack_start(*completionContent, Gtk::PACK_EXPAND_WIDGET);

    pageStack.add(placeholderPage, "placeholder");
    pageStack.add(learningPage, "learning");
    pageStack.add(completionPage, "completion");
    pageStack.set_visible_child("placeholder");

    // ========== 内容区：导航栏 + 页面栈 + 右侧面板（全局固定布局） ==========
    contentArea.pack_start(*navBar, Gtk::PACK_SHRINK);
    contentArea.pack_start(pageStack, Gtk::PACK_EXPAND_WIDGET);
    contentArea.pack_start(sidePanel, Gtk::PACK_SHRINK);
    mainContainer.pack_start(contentArea, Gtk::PACK_EXPAND_WIDGET);

    add(mainContainer);

    // 信号连接
    submitButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_submit_answer));
    showAnswerButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_show_answer));
    nextWordButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_next_word));
    answerEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_entry_activate));
    restartButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_restart));
    reviewWrongWordsButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_review_wrong_words));

    sidePanel.signal_toggle_dark_mode().connect(sigc::mem_fun(*this, &MainWindow::toggle_dark_mode));
    sidePanel.signal_open_settings().connect(sigc::mem_fun(*this, &MainWindow::on_settings));
    sidePanel.signal_open_wrong_words().connect(sigc::mem_fun(*this, &MainWindow::on_review_wrong_words));

    apply_theme();
    reset_attempt();
    show_all_children();
    sidePanel.update_stats();
}

MainWindow::~MainWindow() {}

void MainWindow::on_open_file() {
    Gtk::FileChooserDialog dialog("选择单词库文件", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
    dialog.add_button("打开", Gtk::RESPONSE_OK);
    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("文本文件");
    filter_text->add_pattern("*.txt");
    dialog.add_filter(filter_text);
    apply_dialog_theme(dialog);
    if (dialog.run() == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        if (wordManager.loadWordsFromFile(filename)) {
            show_learning_page();
            on_next_word();
        }
    }
}

void MainWindow::on_edit_words() {
    WordManagerDialog dialog(*this);
    apply_dialog_theme(dialog);
    dialog.run();
}

void MainWindow::on_next_word() {
    if (!currentWord.word.empty() && wordManager.hasFailed(currentWord.word) && answerEntry.get_sensitive()) {
        wordManager.addWrongWord(currentWord);
    }
    if (wordManager.allWordsMastered()) {
        show_completion_page();
        return;
    }
    currentWord = wordManager.getRandomWord();
    if (!currentWord.word.empty()) {
        reset_attempt();
        update_display();
        update_progress();
        sidePanel.update_stats();
    }
}

void MainWindow::on_submit_answer() {
    std::string answer = answerEntry.get_text();
    std::string answerLower = answer;
    std::transform(answerLower.begin(), answerLower.end(), answerLower.begin(), ::tolower);
    std::string correctLower = currentWord.word;
    std::transform(correctLower.begin(), correctLower.end(), correctLower.begin(), ::tolower);

    if (answerLower == correctLower) {
        wordManager.addMasteredWord(currentWord.word);
        feedbackLabel.set_label("✓ 正确！答案: " + currentWord.word);
        answerEntry.set_sensitive(false);
        submitButton.set_sensitive(false);
        showAnswerButton.set_sensitive(false);
        update_progress();
        sidePanel.update_stats();
        if (wordManager.allWordsMastered()) {
            Glib::signal_timeout().connect_once(sigc::mem_fun(*this, &MainWindow::show_completion_page), 1000);
        }
    } else {
        wordManager.addWrongWord(currentWord);
        wordManager.addFailedWord(currentWord.word);
        attemptCount++;
        feedbackLabel.set_label("✗ 错误！请再试一次");
        attemptLabel.set_label("尝试次数: " + std::to_string(attemptCount));
        answerEntry.set_text("");
        answerEntry.grab_focus();
        sidePanel.update_stats();
    }
}

void MainWindow::on_show_answer() {
    if (is_word_completed()) return;
    feedbackLabel.set_label("正确答案: " + currentWord.word);
    answerEntry.set_sensitive(false);
    submitButton.set_sensitive(false);
    showAnswerButton.set_sensitive(false);
    wordManager.addWrongWord(currentWord);
    wordManager.addFailedWord(currentWord.word);
    sidePanel.update_stats();
}

void MainWindow::on_entry_activate() { on_submit_answer(); }

void MainWindow::on_restart() {
    wordManager.clearMasteredWords();
    wordManager.clearWrongWords();
    show_learning_page();
    on_next_word();
    sidePanel.update_stats();
}

void MainWindow::on_export_wrong_words() {
    auto wrongWords = wordManager.getWrongWords();
    if (wrongWords.empty()) return;
    Gtk::FileChooserDialog dialog("导出错词本", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
    dialog.add_button("保存", Gtk::RESPONSE_OK);
    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("文本文件");
    filter_text->add_pattern("*.txt");
    dialog.add_filter(filter_text);
    dialog.set_current_name("wrong_words.txt");
    apply_dialog_theme(dialog);
    if (dialog.run() == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") filename += ".txt";
        std::ofstream file(filename);
        for (const auto& word : wrongWords)
            file << word.word << "|" << word.pos << "|" << word.meaning << "|" << word.example << std::endl;
    }
}

void MainWindow::on_review_wrong_words() {
    auto wrongWords = wordManager.getWrongWords();
    if (wrongWords.empty()) {
        Gtk::MessageDialog d(*this, "错词本为空", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        apply_dialog_theme(d);
        d.run();
        return;
    }
    Gtk::Dialog dialog("错词本", *this, true);
    dialog.set_default_size(600, 450);
    dialog.get_style_context()->add_class("dialog-box");

    Gtk::Box content(Gtk::ORIENTATION_VERTICAL, 10);
    content.set_border_width(16);

    std::stringstream header;
    header << "共 " << wrongWords.size() << " 个错词";
    auto* headerLabel = Gtk::manage(new Gtk::Label(header.str()));
    headerLabel->override_font(Pango::FontDescription("Sans Bold 16"));
    content.pack_start(*headerLabel, Gtk::PACK_SHRINK);

    Gtk::ScrolledWindow scrolled;
    scrolled.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    Gtk::Box wordsBox(Gtk::ORIENTATION_VERTICAL, 8);

    for (const auto& w : wrongWords) {
        auto* card = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 4));
        card->get_style_context()->add_class("glass-card");
        card->set_border_width(12);

        auto* wordLabel = Gtk::manage(new Gtk::Label(w.word + "  [" + w.pos + "]"));
        wordLabel->override_font(Pango::FontDescription("Sans Bold 14"));
        wordLabel->set_halign(Gtk::ALIGN_START);

        auto* meaningLabel = Gtk::manage(new Gtk::Label("释义: " + w.meaning));
        meaningLabel->set_halign(Gtk::ALIGN_START);

        auto* exampleLabel = Gtk::manage(new Gtk::Label("例句: " + w.example));
        exampleLabel->set_halign(Gtk::ALIGN_START);
        exampleLabel->set_line_wrap(true);

        card->pack_start(*wordLabel, Gtk::PACK_SHRINK);
        card->pack_start(*meaningLabel, Gtk::PACK_SHRINK);
        card->pack_start(*exampleLabel, Gtk::PACK_SHRINK);
        wordsBox.pack_start(*card, Gtk::PACK_SHRINK);
    }
    scrolled.add(wordsBox);
    content.pack_start(scrolled, Gtk::PACK_EXPAND_WIDGET);

    // 底部按钮
    Gtk::Box buttonBox(Gtk::ORIENTATION_HORIZONTAL, 10);
    buttonBox.set_halign(Gtk::ALIGN_END);
    auto* exportBtn = Gtk::manage(new Gtk::Button("导出"));
    exportBtn->get_style_context()->add_class("glass-btn-secondary");
    auto* clearBtn = Gtk::manage(new Gtk::Button("清空"));
    clearBtn->get_style_context()->add_class("glass-btn");
    auto* closeBtn = Gtk::manage(new Gtk::Button("关闭"));
    closeBtn->get_style_context()->add_class("glass-btn");

    buttonBox.pack_start(*exportBtn, Gtk::PACK_SHRINK);
    buttonBox.pack_start(*clearBtn, Gtk::PACK_SHRINK);
    buttonBox.pack_start(*closeBtn, Gtk::PACK_SHRINK);
    content.pack_start(buttonBox, Gtk::PACK_SHRINK);

    dialog.get_content_area()->pack_start(content);
    dialog.show_all();
    apply_dialog_theme(dialog);

    exportBtn->signal_clicked().connect([this, &dialog]() {
        dialog.hide();
        on_export_wrong_words();
    });
    clearBtn->signal_clicked().connect([this, &dialog]() {
        wordManager.clearWrongWords();
        sidePanel.update_stats();
        dialog.response(Gtk::RESPONSE_OK);
    });
    closeBtn->signal_clicked().connect([&dialog]() {
        dialog.response(Gtk::RESPONSE_OK);
    });

    dialog.run();
}


void MainWindow::on_clear_wrong_words() {
    wordManager.clearWrongWords();
    sidePanel.update_stats();
}

void MainWindow::on_reset_progress() {
    wordManager.clearMasteredWords();
    wordManager.clearWrongWords();
    update_progress();
    sidePanel.update_stats();
}

void MainWindow::on_settings() {
    SettingsDialog dialog(*this, settingsManager);
    apply_dialog_theme(dialog);
    if (dialog.run() == Gtk::RESPONSE_OK) {
        apply_theme();
        sidePanel.update_stats();
    }
}

void MainWindow::on_debug_info() {
    std::stringstream ss;
    ss << "总单词: " << wordManager.getTotalWords() << "\n";
    ss << "已掌握: " << wordManager.getMasteredWordsCount() << "\n";
    ss << "错词: " << wordManager.getWrongWordsCount() << "\n";
    Gtk::MessageDialog d(*this, ss.str(), false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    apply_dialog_theme(d);
    d.run();
}

void MainWindow::on_about() {
    AboutDialog aboutDialog(*this);
    apply_dialog_theme(aboutDialog);
    aboutDialog.run();
}

void MainWindow::on_usage() {
    UsageDialog usageDialog(*this);
    apply_dialog_theme(usageDialog);
    usageDialog.run();
}

void MainWindow::update_display() {
    if (!currentWord.word.empty()) {
        posLabel.set_label("[" + currentWord.pos + "]");
        meaningLabel.set_label(currentWord.meaning);
        exampleLabel.set_label("例句: " + currentWord.example);
        sidePanel.set_current_word_status(wordManager.hasFailed(currentWord.word));
        feedbackLabel.set_label("");
        attemptLabel.set_label("");
        answerEntry.set_sensitive(true);
        submitButton.set_sensitive(true);
        showAnswerButton.set_sensitive(true);
        answerEntry.set_text("");
        answerEntry.grab_focus();
    }
}

void MainWindow::reset_attempt() {
    attemptCount = 0;
    attemptLabel.set_label("");
}

void MainWindow::show_completion_page() {
    std::stringstream stats;
    stats << "你已掌握所有 " << wordManager.getTotalWords() << " 个单词！\n错词: " << wordManager.getWrongWordsCount();
    completionStats.set_label(stats.str());
    pageStack.set_visible_child("completion");
    sidePanel.update_stats();
}

void MainWindow::show_learning_page() {
    pageStack.set_visible_child("learning");
}

void MainWindow::update_progress() {
    sidePanel.update_stats();
}

void MainWindow::apply_theme() {
    bool dark = settingsManager.isDarkModeEnabled();
    std::string accent = settingsManager.getPrimaryColor();
    GlassTheme::apply_theme(dark, accent);
    get_style_context()->remove_class("light-glass");
    get_style_context()->remove_class("dark-glass");
    get_style_context()->add_class(dark ? "dark-glass" : "light-glass");
    sidePanel.set_dark_mode_button(dark);
}

void MainWindow::toggle_dark_mode() {
    settingsManager.setDarkModeEnabled(!settingsManager.isDarkModeEnabled());
    apply_theme();
}

void MainWindow::apply_dialog_theme(Gtk::Window& dialog) {
    bool dark = settingsManager.isDarkModeEnabled();
    auto ctx = dialog.get_style_context();
    ctx->remove_class("light-glass");
    ctx->remove_class("dark-glass");
    ctx->add_class(dark ? "dark-glass" : "light-glass");
}

bool MainWindow::on_key_press_event(GdkEventKey* event) {
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_Tab) {
        on_next_word(); return true;
    }
    if ((event->state & GDK_MOD1_MASK) && (event->keyval == GDK_KEY_s || event->keyval == GDK_KEY_S)) {
        on_show_answer(); return true;
    }
    if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_SHIFT_MASK) && (event->keyval == GDK_KEY_d || event->keyval == GDK_KEY_D)) {
        toggle_dark_mode(); return true;
    }
    return Gtk::Window::on_key_press_event(event);
}
