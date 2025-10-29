#include "MainWindow.h"
#include <iostream>
#include <iomanip>

// 在构造函数中添加设置菜单项
MainWindow::MainWindow() 
    : mainBox(Gtk::ORIENTATION_VERTICAL),
      completionBox(Gtk::ORIENTATION_VERTICAL),
      wordBox(Gtk::ORIENTATION_VERTICAL),
      inputBox(Gtk::ORIENTATION_HORIZONTAL),
      controlBox(Gtk::ORIENTATION_HORIZONTAL),
      attemptCount(0) {
    
    // 设置窗口属性
    set_title("单词背诵工具 - 交互版");
    set_default_size(500, 400);
    
    // 在菜单中添加设置项
    fileMenu.set_label("文件");
    openMenuItem.set_label("打开单词库");
    resetProgressMenuItem.set_label("重置进度");
    settingsMenuItem.set_label("主题设置");  // 新增
    exitMenuItem.set_label("退出");
    
    fileSubmenu.append(openMenuItem);
    fileSubmenu.append(resetProgressMenuItem);
    fileSubmenu.append(settingsMenuItem);  // 新增
    fileSubmenu.append(exitMenuItem);
    fileMenu.set_submenu(fileSubmenu);
    
    menuBar.append(fileMenu);

	 
    // 连接设置菜单信号
    settingsMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_settings));
    
    // 应用主题颜色
    apply_theme_colors();
    
    // 设置单词显示区域
    wordBox.set_spacing(5);
    wordBox.set_border_width(10);
    
    posLabel.set_justify(Gtk::JUSTIFY_CENTER);
    posLabel.override_font(Pango::FontDescription("Sans 16"));
    posLabel.override_color(Gdk::RGBA("blue"));
    
    meaningLabel.set_justify(Gtk::JUSTIFY_CENTER);
    meaningLabel.override_font(Pango::FontDescription("Sans 18"));
    
    exampleLabel.set_justify(Gtk::JUSTIFY_CENTER);
    exampleLabel.override_font(Pango::FontDescription("Sans 12"));
    exampleLabel.set_line_wrap(true);
    
    wordBox.pack_start(posLabel, Gtk::PACK_SHRINK);
    wordBox.pack_start(meaningLabel, Gtk::PACK_SHRINK);
    wordBox.pack_start(exampleLabel, Gtk::PACK_SHRINK);
    
    // 设置输入区域
    inputBox.set_spacing(10);
    inputBox.set_border_width(10);
    
    inputHintLabel.set_label("请输入英文单词:");
    inputHintLabel.override_font(Pango::FontDescription("Sans 14"));
    
    answerEntry.set_placeholder_text("在这里输入答案...");
    answerEntry.set_size_request(200, -1);
    answerEntry.override_font(Pango::FontDescription("Sans 14"));
    
    submitButton.set_label("提交答案");
    
    inputBox.pack_start(inputHintLabel, Gtk::PACK_SHRINK);
    inputBox.pack_start(answerEntry, Gtk::PACK_SHRINK);
    inputBox.pack_start(submitButton, Gtk::PACK_SHRINK);
    
    // 设置反馈区域
    feedbackLabel.set_justify(Gtk::JUSTIFY_CENTER);
    feedbackLabel.override_font(Pango::FontDescription("Sans 14"));
    
    attemptLabel.set_justify(Gtk::JUSTIFY_CENTER);
    attemptLabel.override_font(Pango::FontDescription("Sans 12"));
    
    // 设置进度标签
    progressLabel.set_justify(Gtk::JUSTIFY_CENTER);
    progressLabel.override_font(Pango::FontDescription("Sans 12"));
    progressLabel.override_color(Gdk::RGBA("gray"));
    
    // 设置控制按钮
    controlBox.set_spacing(10);
    controlBox.set_border_width(10);
    
    showAnswerButton.set_label("查看答案");
    nextWordButton.set_label("下一个单词");
    
    controlBox.pack_start(showAnswerButton, Gtk::PACK_EXPAND_WIDGET);
    controlBox.pack_start(nextWordButton, Gtk::PACK_EXPAND_WIDGET);
    
    // 学习界面主布局
    mainBox.set_spacing(10);
    mainBox.pack_start(menuBar, Gtk::PACK_SHRINK);
    mainBox.pack_start(wordBox, Gtk::PACK_SHRINK);
    mainBox.pack_start(inputBox, Gtk::PACK_SHRINK);
    mainBox.pack_start(feedbackLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(attemptLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(progressLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(controlBox, Gtk::PACK_SHRINK);
    mainBox.pack_start(statusbar, Gtk::PACK_SHRINK);
    
    // 设置完成页面
    completionBox.set_spacing(20);
    completionBox.set_border_width(30);
    
    completionTitle.set_label("🎉 恭喜你！ 🎉");
    completionTitle.override_font(Pango::FontDescription("Sans Bold 24"));
    completionTitle.override_color(Gdk::RGBA("green"));
    
    completionStats.set_justify(Gtk::JUSTIFY_CENTER);
    completionStats.override_font(Pango::FontDescription("Sans 16"));
    
    restartButton.set_label("重新开始学习");
    restartButton.override_font(Pango::FontDescription("Sans Bold 14"));
    restartButton.set_size_request(200, 50);
    
    reviewWrongWordsButton.set_label("复习错词本");
    reviewWrongWordsButton.override_font(Pango::FontDescription("Sans 14"));
    reviewWrongWordsButton.set_size_request(200, 40);
    
    completionBox.pack_start(completionTitle, Gtk::PACK_SHRINK);
    completionBox.pack_start(completionStats, Gtk::PACK_SHRINK);
    completionBox.pack_start(restartButton, Gtk::PACK_SHRINK);
    completionBox.pack_start(reviewWrongWordsButton, Gtk::PACK_SHRINK);
    
    // 初始显示学习界面
    add(mainBox);
    
    // 连接信号
    openMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_open_file));
    resetProgressMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_reset_progress));
    exitMenuItem.signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));
    
    submitButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_submit_answer));
    showAnswerButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_show_answer));
    nextWordButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_next_word));
    answerEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_entry_activate));
    
    restartButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_restart));
    reviewWrongWordsButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_review_wrong_words));
    
    // 初始状态
    reset_attempt();
    update_display();
    
    show_all_children();
    statusbar.push("请通过菜单加载单词库文件");
}

MainWindow::~MainWindow() {}


// 添加设置处理方法
void MainWindow::on_settings() {
    SettingsDialog dialog(*this, settingsManager);
    int result = dialog.run();
    
    if (result == Gtk::RESPONSE_OK) {
        // 用户点击了保存，重新应用主题颜色
        apply_theme_colors();
    }
}

// 应用主题颜色
void MainWindow::apply_theme_colors() {
    // 应用词性颜色
    posLabel.override_color(Gdk::RGBA(settingsManager.getPosColor()));
    
    // 注意：其他颜色会在相应的方法中动态应用
    // 例如，在 on_submit_answer 和 on_show_answer 中应用正确/错误/答案颜色
}

void MainWindow::on_open_file() {
    Gtk::FileChooserDialog dialog("选择单词库文件", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);
    
    dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
    dialog.add_button("打开", Gtk::RESPONSE_OK);
    
    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("文本文件");
    filter_text->add_pattern("*.txt");
    dialog.add_filter(filter_text);
    
    int result = dialog.run();
    
    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        if (wordManager.loadWordsFromFile(filename)) {
            show_learning_page();
            on_next_word();
            statusbar.push("已加载单词库: " + filename);
        } else {
            Gtk::MessageDialog errorDialog(*this, "无法加载单词库文件", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            errorDialog.run();
        }
    }
}

void MainWindow::on_next_word() {
    if (wordManager.allWordsMastered()) {
        show_completion_page();
        return;
    }
    
    currentWord = wordManager.getRandomWord();
    if (!currentWord.word.empty()) {
        reset_attempt();
        update_display();
        update_progress();
    }
}

// 更新 on_submit_answer 方法以使用主题颜色
void MainWindow::on_submit_answer() {
    std::string userAnswer = answerEntry.get_text();
    
    // 转换为小写进行比较（不区分大小写）
    std::string userAnswerLower = userAnswer;
    std::transform(userAnswerLower.begin(), userAnswerLower.end(), userAnswerLower.begin(), ::tolower);
    
    std::string correctAnswerLower = currentWord.word;
    std::transform(correctAnswerLower.begin(), correctAnswerLower.end(), correctAnswerLower.begin(), ::tolower);
    
    if (userAnswerLower == correctAnswerLower) {
        // 答案正确，标记为已掌握
        wordManager.addMasteredWord(currentWord.word);
        
        feedbackLabel.set_label("✓ 正确！答案: " + currentWord.word);
        feedbackLabel.override_color(Gdk::RGBA(settingsManager.getCorrectColor()));  // 使用主题颜色
        answerEntry.set_sensitive(false);
        submitButton.set_sensitive(false);
        
        update_progress();
        
        // 检查是否已完成所有单词
        if (wordManager.allWordsMastered()) {
            // 延迟显示完成页面，让用户看到最后一个单词的正确反馈
            Glib::signal_timeout().connect_once(
                sigc::mem_fun(*this, &MainWindow::show_completion_page), 1000);
        }
    } else {
        // 答案错误
        attemptCount++;
        feedbackLabel.set_label("✗ 错误！请再试一次");
        feedbackLabel.override_color(Gdk::RGBA(settingsManager.getErrorColor()));  // 使用主题颜色
        attemptLabel.set_label("尝试次数: " + std::to_string(attemptCount));
        
        // 清空输入框并聚焦
        answerEntry.set_text("");
        answerEntry.grab_focus();
    }
}

// 更新 on_show_answer 方法以使用主题颜色
void MainWindow::on_show_answer() {
    feedbackLabel.set_label("正确答案: " + currentWord.word);
    feedbackLabel.override_color(Gdk::RGBA(settingsManager.getAnswerColor()));
    answerEntry.set_sensitive(false);
    submitButton.set_sensitive(false);
    
    // 添加到错词本（会自动检查重复）
    wordManager.addWrongWord(currentWord);
}

void MainWindow::on_entry_activate() {
    on_submit_answer();
}

void MainWindow::on_restart() {
    wordManager.clearMasteredWords();
    show_learning_page();
    on_next_word();
    statusbar.push("进度已重置，重新开始学习");
}

void MainWindow::on_review_wrong_words() {
    auto wrongWords = wordManager.getWrongWords();
    if (wrongWords.empty()) {
        Gtk::MessageDialog infoDialog(*this, "错词本为空", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        infoDialog.run();
        return;
    }
    
    // 创建错词本对话框
    Gtk::Dialog wrongWordsDialog("错词本", *this, true);
    wrongWordsDialog.set_default_size(600, 400);
    wrongWordsDialog.set_border_width(10);
    
    // 创建滚动窗口
    Gtk::ScrolledWindow scrolledWindow;
    scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    
    // 创建多列布局的容器
    Gtk::Box mainBox(Gtk::ORIENTATION_VERTICAL, 10);
    
    // 添加标题和统计信息
    std::stringstream headerText;
    headerText << "错词本 - 共 " << wrongWords.size() << " 个单词";
    Gtk::Label headerLabel(headerText.str());
    headerLabel.override_font(Pango::FontDescription("Sans Bold 14"));
    mainBox.pack_start(headerLabel, Gtk::PACK_SHRINK);
    
    // 创建单词列表的容器
    Gtk::Box wordsBox(Gtk::ORIENTATION_VERTICAL, 8);
    
    // 显示每个错词
    for (size_t i = 0; i < wrongWords.size(); ++i) {
        const auto& word = wrongWords[i];
        
        // 创建单个单词的框架
        Gtk::Frame wordFrame;
        wordFrame.set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        
        Gtk::Box wordBox(Gtk::ORIENTATION_VERTICAL, 5);
        wordBox.set_border_width(8);
        
        // 单词和词性
        Gtk::Box wordInfoBox(Gtk::ORIENTATION_HORIZONTAL, 10);
        Gtk::Label wordLabel(word.word);
        wordLabel.override_font(Pango::FontDescription("Sans Bold 12"));
        Gtk::Label posLabel("[" + word.pos + "]");
        posLabel.override_color(Gdk::RGBA(settingsManager.getPosColor()));
        
        wordInfoBox.pack_start(wordLabel, Gtk::PACK_SHRINK);
        wordInfoBox.pack_start(posLabel, Gtk::PACK_SHRINK);
        
        // 中文意思
        Gtk::Label meaningLabel("含义: " + word.meaning);
        meaningLabel.set_alignment(0, 0.5); // 左对齐
        
        // 例句
        Gtk::Label exampleLabel("例句: " + word.example);
        exampleLabel.set_alignment(0, 0.5); // 左对齐
        exampleLabel.set_line_wrap(true);
        
        wordBox.pack_start(wordInfoBox, Gtk::PACK_SHRINK);
        wordBox.pack_start(meaningLabel, Gtk::PACK_SHRINK);
        wordBox.pack_start(exampleLabel, Gtk::PACK_SHRINK);
        
        wordFrame.add(wordBox);
        wordsBox.pack_start(wordFrame, Gtk::PACK_SHRINK);
    }
    
    // 将单词列表放入滚动窗口
    scrolledWindow.add(wordsBox);
    
    // 创建按钮
    Gtk::ButtonBox buttonBox(Gtk::ORIENTATION_HORIZONTAL);
    buttonBox.set_layout(Gtk::BUTTONBOX_END);
    Gtk::Button closeButton("关闭");
    buttonBox.pack_start(closeButton, Gtk::PACK_SHRINK);
    
    // 布局
    mainBox.pack_start(scrolledWindow, Gtk::PACK_EXPAND_WIDGET);
    mainBox.pack_start(buttonBox, Gtk::PACK_SHRINK);
    
    wrongWordsDialog.get_content_area()->pack_start(mainBox);
    
    // 连接信号
    closeButton.signal_clicked().connect([&wrongWordsDialog]() {
        wrongWordsDialog.response(Gtk::RESPONSE_OK);
    });
    
    wrongWordsDialog.show_all_children();
    wrongWordsDialog.run();
}

void MainWindow::on_reset_progress() {
    wordManager.clearMasteredWords();
    update_progress();
    statusbar.push("学习进度已重置");
}

void MainWindow::update_display() {
    if (!currentWord.word.empty()) {
        posLabel.set_label("[" + currentWord.pos + "]");
        meaningLabel.set_label(currentWord.meaning);
        exampleLabel.set_label("例句: " + currentWord.example);
        
        // 重置反馈
        feedbackLabel.set_label("");
        attemptLabel.set_label("");
        
        // 启用输入
        answerEntry.set_sensitive(true);
        submitButton.set_sensitive(true);
        answerEntry.set_text("");
        answerEntry.grab_focus();
    } else {
        posLabel.set_label("");
        meaningLabel.set_label("请加载单词库文件");
        exampleLabel.set_label("");
        feedbackLabel.set_label("");
        attemptLabel.set_label("");
    }
}

void MainWindow::reset_attempt() {
    attemptCount = 0;
    attemptLabel.set_label("");
}

void MainWindow::show_completion_page() {
    // 切换到完成页面
    remove();
    add(completionBox);
    
    // 更新完成页面的统计信息
    std::stringstream stats;
    stats << "你已经掌握了所有 " << wordManager.getTotalWords() << " 个单词！\n\n";
    stats << "错词数量: " << wordManager.getWrongWordsCount() << "\n";
    stats << "你可以重新开始学习或复习错词本";
    
    completionStats.set_label(stats.str());
    
    show_all_children();
}

void MainWindow::show_learning_page() {
    // 切换回学习页面
    remove();
    add(mainBox);
    show_all_children();
}

void MainWindow::update_progress() {
    if (wordManager.getTotalWords() > 0) {
        float progress = wordManager.getProgress();
        int percentage = static_cast<int>(progress * 100);
        
        std::stringstream progressText;
        progressText << "进度: " << wordManager.getMasteredWordsCount() << "/" 
                    << wordManager.getTotalWords() << " (" << percentage << "%)";
        
        progressLabel.set_label(progressText.str());
        
        // 更新状态栏，添加错词统计
        std::stringstream statusText;
        statusText << "已掌握: " << wordManager.getMasteredWordsCount() 
                  << "/" << wordManager.getTotalWords() 
                  << " | 错词: " << wordManager.getWrongWordsCount()
                  << " | 剩余: " << wordManager.getRemainingWordsCount();
        
        statusbar.push(statusText.str());
    }
}