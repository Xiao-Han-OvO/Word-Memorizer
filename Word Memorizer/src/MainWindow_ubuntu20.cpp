#include "MainWindow_ubuntu20.h"
#include <iostream>
#include <sstream>

MainWindow::MainWindow() 
    : mainBox(Gtk::ORIENTATION_VERTICAL),
      attemptCount(0) {
    
    set_title("单词背诵工具 - Ubuntu 20.04 兼容版");
    set_default_size(500, 400);
    
    // 简化菜单
    fileMenu.set_label("文件");
    openMenuItem.set_label("打开单词库");
    exitMenuItem.set_label("退出");
    
    fileSubmenu.append(openMenuItem);
    fileSubmenu.append(exitMenuItem);
    fileMenu.set_submenu(fileSubmenu);
    menuBar.append(fileMenu);
    
    // 简化界面布局
    mainBox.set_spacing(10);
    mainBox.set_border_width(10);
    
    // 单词显示
    meaningLabel.set_label("请加载单词库文件");
    meaningLabel.set_justify(Gtk::JUSTIFY_CENTER);
    mainBox.pack_start(meaningLabel, Gtk::PACK_SHRINK);
    
    exampleLabel.set_label("");
    exampleLabel.set_justify(Gtk::JUSTIFY_CENTER);
    exampleLabel.set_line_wrap(true);
    mainBox.pack_start(exampleLabel, Gtk::PACK_SHRINK);
    
    // 输入区域
    answerEntry.set_placeholder_text("输入英文单词...");
    mainBox.pack_start(answerEntry, Gtk::PACK_SHRINK);
    
    // 按钮
    Gtk::Box buttonBox(Gtk::ORIENTATION_HORIZONTAL);
    buttonBox.set_spacing(5);
    
    submitButton.set_label("提交答案");
    nextButton.set_label("下一个");
    showAnswerButton.set_label("查看答案");
    
    buttonBox.pack_start(submitButton, Gtk::PACK_EXPAND_WIDGET);
    buttonBox.pack_start(nextButton, Gtk::PACK_EXPAND_WIDGET);
    buttonBox.pack_start(showAnswerButton, Gtk::PACK_EXPAND_WIDGET);
    
    mainBox.pack_start(buttonBox, Gtk::PACK_SHRINK);
    
    // 反馈和进度
    feedbackLabel.set_label("");
    feedbackLabel.set_justify(Gtk::JUSTIFY_CENTER);
    mainBox.pack_start(feedbackLabel, Gtk::PACK_SHRINK);
    
    progressLabel.set_label("");
    progressLabel.set_justify(Gtk::JUSTIFY_CENTER);
    mainBox.pack_start(progressLabel, Gtk::PACK_SHRINK);
    
    // 添加菜单
    mainBox.pack_start(menuBar, Gtk::PACK_SHRINK);
    
    add(mainBox);
    
    // 连接信号
    openMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_open_file));
    exitMenuItem.signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));
    submitButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_submit_answer));
    nextButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_next_word));
    showAnswerButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_show_answer));
    answerEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_submit_answer));
    
    show_all_children();
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
    
    int result = dialog.run();
    
    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        if (wordManager.loadWordsFromFile(filename)) {
            on_next_word();
        } else {
            Gtk::MessageDialog errorDialog(*this, "无法加载单词库文件", false, Gtk::MESSAGE_ERROR);
            errorDialog.run();
        }
    }
}

void MainWindow::on_next_word() {
    currentWord = wordManager.getRandomWord();
    if (!currentWord.word.empty()) {
        update_display();
        update_progress();
    }
}

void MainWindow::on_submit_answer() {
    std::string userAnswer = answerEntry.get_text();
    
    // 转换为小写进行比较
    std::string userAnswerLower = userAnswer;
    std::transform(userAnswerLower.begin(), userAnswerLower.end(), userAnswerLower.begin(), ::tolower);
    
    std::string correctAnswerLower = currentWord.word;
    std::transform(correctAnswerLower.begin(), correctAnswerLower.end(), correctAnswerLower.begin(), ::tolower);
    
    if (userAnswerLower == correctAnswerLower) {
        wordManager.addMasteredWord(currentWord.word);
        feedbackLabel.set_label("✓ 正确！答案: " + currentWord.word);
        answerEntry.set_sensitive(false);
        submitButton.set_sensitive(false);
        
        if (wordManager.allWordsMastered()) {
            feedbackLabel.set_label("🎉 恭喜！所有单词都已掌握！");
        } else {
            // 自动下一个单词
            Glib::signal_timeout().connect_once(
                sigc::mem_fun(*this, &MainWindow::on_next_word), 1000);
        }
    } else {
        attemptCount++;
        feedbackLabel.set_label("✗ 错误！请再试一次 (尝试: " + std::to_string(attemptCount) + ")");
        answerEntry.set_text("");
        answerEntry.grab_focus();
    }
    
    update_progress();
}

void MainWindow::on_show_answer() {
    feedbackLabel.set_label("正确答案: " + currentWord.word);
    answerEntry.set_sensitive(false);
    submitButton.set_sensitive(false);
    wordManager.addWrongWord(currentWord);
}

void MainWindow::update_display() {
    if (!currentWord.word.empty()) {
        meaningLabel.set_label("[" + currentWord.pos + "] " + currentWord.meaning);
        exampleLabel.set_label("例句: " + currentWord.example);
        
        feedbackLabel.set_label("");
        answerEntry.set_sensitive(true);
        submitButton.set_sensitive(true);
        answerEntry.set_text("");
        answerEntry.grab_focus();
        attemptCount = 0;
    }
}

void MainWindow::update_progress() {
    if (wordManager.getTotalWords() > 0) {
        float progress = wordManager.getProgress();
        int percentage = static_cast<int>(progress * 100);
        
        std::stringstream text;
        text << "进度: " << wordManager.getMasteredWordsCount() << "/" 
             << wordManager.getTotalWords() << " (" << percentage << "%)";
        progressLabel.set_label(text.str());
    }
}