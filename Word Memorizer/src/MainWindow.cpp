#include "MainWindow.h"
#include "SettingsDialog.h"
#include "UsageDialog.h"
#include "AboutDialog.h"  // 新增
#include <iostream>
#include <iomanip>
#include <algorithm>

MainWindow::MainWindow() 
    : mainBox(Gtk::ORIENTATION_VERTICAL),
      completionBox(Gtk::ORIENTATION_VERTICAL),
      wordBox(Gtk::ORIENTATION_VERTICAL),
      inputBox(Gtk::ORIENTATION_HORIZONTAL),
      controlBox(Gtk::ORIENTATION_HORIZONTAL),
      attemptCount(0),
      dragging(false),
      drag_x(0),
      drag_y(0) {
    
    // 设置窗口属性 - 移除标题栏
    set_title("单词记忆大师");
    set_default_size(600, 500);
    set_decorated(false); // 移除标题栏和边框
    
    // 为主窗口添加自定义类
    get_style_context()->add_class("custom-window");
    
    // 加载CSS样式
    auto cssProvider = Gtk::CssProvider::create();
    try {
        cssProvider->load_from_data(
            // 基础字体设置
            "* {"
            "   font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, 'Helvetica Neue', Arial, sans-serif;"
            "}"
            
            // 为主窗口添加特定类，只对主窗口应用圆角和无边框
            ".custom-window {"
            // "   border-radius: 12px;"
            "   box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);"
            "}"
            
            // 标题栏样式 - 只对主窗口应用
            ".custom-titlebar {"
            "   background: transparent;"
            "   padding: 2px 8px;"
            "   min-height: 24px;"
            "   border: none;"
            "   box-shadow: none;"
            "}"
            
            ".custom-titlebar-drag-area {"
            "   background: transparent;"
            "   border: none;"
            "}"
            
            ".custom-titlebar-label {"
            "   font-size: 17.5px;"
            "   font-weight: 600;"
            "   color: #666;"
            "}"
            
            // 窗口控制按钮 - 只对主窗口应用
            ".custom-window-control {"
            "   background: transparent;"
            "   border: none;"
            "   border-radius: 50%;"
            "   padding: 0;"
            "   min-width: 16px;"
            "   min-height: 16px;"
            "   margin: 0 3px;"
            "   font-size: 21px;"
            "   font-weight: bold;"
            "   color: #666;"
            "   transition: all 0.15s ease-in-out;"
            "}"
            
            ".custom-window-control:hover {"
            "   background: rgba(0, 0, 0, 0.1);"
            "   color: #333;"
            "}"
            
            ".custom-window-control.close:hover {"
            "   background: #e74c3c;"
            "   color: white;"
            "}"
            
            // 浅色模式
            ".light-mode {"
            "   background-color: #ffffff;"
            "   color: #1a202c;"
            "}"
            
            ".light-mode .card {"
            "   background: #ffffff;"
            "   border: 1px solid #e2e8f0;"
            "   border-radius: 8px;"
            "   padding: 20px;"
            "   margin: 10px 0;"
            "   box-shadow: 0 1px 3px rgba(0, 0, 0, 0.1);"
            "}"
            
            ".light-mode button {"
            "   background: #f6f8fa;"
            "   border: 1px solid #d1d5da;"
            "   border-radius: 6px;"
            "   color: #24292e;"
            "   padding: 8px 16px;"
            "}"
            
            ".light-mode button:hover {"
            "   background: #f3f4f6;"
            "}"
            
            ".light-mode button.primary {"
            "   background: #2ea44f;"
            "   border-color: #2c974b;"
            "   color: white;"
            "}"
            
            ".light-mode button.primary:hover {"
            "   background: #2c974b;"
            "}"
            
            ".light-mode entry {"
            "   background: #fafbfc;"
            "   border: 1px solid #e1e4e8;"
            "   border-radius: 6px;"
            "   color: #24292e;"
            "   padding: 8px 12px;"
            "}"
            
            ".light-mode entry:focus {"
            "   border-color: #0366d6;"
            "   box-shadow: 0 0 0 3px rgba(3, 102, 214, 0.3);"
            "}"
            
            ".light-mode label {"
            "   color: #24292e;"
            "}"
            
            ".light-mode label.title {"
            "   font-size: 24px;"
            "   font-weight: 600;"
            "}"
            
            ".light-mode label.subtitle {"
            "   font-size: 18px;"
            "   font-weight: 500;"
            "   color: #2d3748;"
            "}"
            
            ".light-mode label.highlight {"
            "   color: #0366d6;"
            "   font-weight: 500;"
            "}"
            
            ".light-mode label.muted {"
            "   color: #6b7280;"
            "}"
            
            ".light-mode progressbar {"
            "   border-radius: 4px;"
            "}"
            
            ".light-mode progressbar trough {"
            "   background: #e1e4e8;"
            "   border-radius: 4px;"
            "}"
            
            ".light-mode progressbar progress {"
            "   background: #0366d6;"
            "   border-radius: 4px;"
            "}"
            
            ".light-mode statusbar {"
            "   background: #f6f8fa;"
            "   border-top: 1px solid #e1e4e8;"
            "   padding: 8px;"
            "}"
            
            // 深色模式
            ".dark-mode {"
            "   background-color: #1a202c;"
            "   color: #f7fafc;"
            "}"
            
            ".dark-mode .custom-titlebar-label {"
            "   color: #a0aec0;"
            "}"
            
            ".dark-mode .custom-window-control {"
            "   color: #a0aec0;"
            "}"
            
            ".dark-mode .custom-window-control:hover {"
            "   background: rgba(255, 255, 255, 0.1);"
            "   color: #e2e8f0;"
            "}"
            
            ".dark-mode .custom-window-control.close:hover {"
            "   background: #e74c3c;"
            "   color: white;"
            "}"
            
            ".dark-mode .card {"
            "   background: #2d3748;"
            "   border: 1px solid #4a5568;"
            "   border-radius: 8px;"
            "   padding: 20px;"
            "   margin: 10px 0;"
            "   box-shadow: 0 1px 3px rgba(0, 0, 0, 0.3);"
            "}"
            
            ".dark-mode button {"
            "   background: #2d3748;"
            "   border: 1px solid #4a5568;"
            "   border-radius: 6px;"
            "   color: #e2e8f0;"
            "   padding: 8px 16px;"
            "}"
            
            ".dark-mode button:hover {"
            "   background: #4a5568;"
            "}"
            
            ".dark-mode button.primary {"
            "   background: #2c974b;"
            "   border-color: #2c974b;"
            "   color: white;"
            "}"
            
            ".dark-mode button.primary:hover {"
            "   background: #2a8f47;"
            "}"
            
            ".dark-mode entry {"
            "   background: #2d3748;"
            "   border: 1px solid #4a5568;"
            "   border-radius: 6px;"
            "   color: #e2e8f0;"
            "   padding: 8px 12px;"
            "}"
            
            ".dark-mode entry:focus {"
            "   border-color: #63b3ed;"
            "   box-shadow: 0 0 0 3px rgba(99, 179, 237, 0.3);"
            "}"
            
            ".dark-mode label {"
            "   color: #f7fafc;"
            "}"
            
            ".dark-mode label.title {"
            "   font-size: 24px;"
            "   font-weight: 600;"
            "}"
            
            ".dark-mode label.subtitle {"
            "   font-size: 18px;"
            "   font-weight: 500;"
            "   color: #e2e8f0;"
            "}"
            
            ".dark-mode label.highlight {"
            "   color: #63b3ed;"
            "   font-weight: 500;"
            "}"
            
            ".dark-mode label.muted {"
            "   color: #a0aec0;"
            "}"
            
            ".dark-mode progressbar {"
            "   border-radius: 4px;"
            "}"
            
            ".dark-mode progressbar trough {"
            "   background: #4a5568;"
            "   border-radius: 4px;"
            "}"
            
            ".dark-mode progressbar progress {"
            "   background: #63b3ed;"
            "   border-radius: 4px;"
            "}"
            
            ".dark-mode statusbar {"
            "   background: #2d3748;"
            "   border-top: 1px solid #4a5568;"
            "   padding: 8px;"
            "}"
        );
        
        apply_theme();

        Gtk::StyleContext::add_provider_for_screen(
            Gdk::Screen::get_default(),
            cssProvider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
    } catch (const Gtk::CssProviderError& ex) {
        std::cerr << "CSS加载错误: " << ex.what() << std::endl;
    }
    
    // 创建自定义标题栏
    Gtk::EventBox* titlebarEventBox = Gtk::manage(new Gtk::EventBox());
    titlebarEventBox->set_hexpand(true);
    titlebarEventBox->get_style_context()->add_class("custom-titlebar-drag-area");
    
    Gtk::Box* titlebarBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
    titlebarBox->set_border_width(4);
    titlebarBox->set_halign(Gtk::ALIGN_FILL);
    titlebarBox->set_hexpand(true);
    titlebarBox->get_style_context()->add_class("custom-titlebar");
    
    // 窗口标题
    Gtk::Label* windowTitle = Gtk::manage(new Gtk::Label("单词记忆大师"));
    windowTitle->set_halign(Gtk::ALIGN_START);
    windowTitle->set_hexpand(true);
    windowTitle->get_style_context()->add_class("custom-titlebar-label");
    
    // 窗口控制按钮
    Gtk::Box* windowControls = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 2));
    
    // 最小化按钮
    Gtk::Button* minimizeBtn = Gtk::manage(new Gtk::Button("−"));
    minimizeBtn->get_style_context()->add_class("custom-window-control");
    minimizeBtn->set_tooltip_text("最小化");
    minimizeBtn->set_size_request(16, 16);
    
    // 关闭按钮
    Gtk::Button* closeBtn = Gtk::manage(new Gtk::Button("×"));
    closeBtn->get_style_context()->add_class("custom-window-control");
    closeBtn->get_style_context()->add_class("close");
    closeBtn->set_tooltip_text("关闭");
    closeBtn->set_size_request(16, 16);
    
    windowControls->pack_start(*minimizeBtn, Gtk::PACK_SHRINK);
    windowControls->pack_start(*closeBtn, Gtk::PACK_SHRINK);
    
    titlebarBox->pack_start(*windowTitle, Gtk::PACK_EXPAND_WIDGET);
    titlebarBox->pack_end(*windowControls, Gtk::PACK_SHRINK);
    
    titlebarEventBox->add(*titlebarBox);
    
    // 为标题栏添加拖动功能
    titlebarEventBox->add_events(Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);
    
    // 鼠标按下事件 - 开始拖动
    titlebarEventBox->signal_button_press_event().connect([this](GdkEventButton* event) {
        if (event->button == GDK_BUTTON_PRIMARY) {
            dragging = true;
            
            // 获取窗口位置
            int x, y;
            get_position(x, y);
            
            // 计算鼠标相对于窗口的位置
            drag_x = event->x_root - x;
            drag_y = event->y_root - y;
            
            return true;
        }
        return false;
    });
    
    // 鼠标释放事件 - 停止拖动
    titlebarEventBox->signal_button_release_event().connect([this](GdkEventButton* event) {
        if (event->button == GDK_BUTTON_PRIMARY) {
            dragging = false;
            return true;
        }
        return false;
    });
    
    // 鼠标移动事件 - 处理拖动
    titlebarEventBox->signal_motion_notify_event().connect([this](GdkEventMotion* event) {
        if (dragging) {
            // 计算新窗口位置
            int new_x = event->x_root - drag_x;
            int new_y = event->y_root - drag_y;
            
            // 移动窗口
            move(new_x, new_y);
            return true;
        }
        return false;
    });
    
    // 创建菜单栏
    fileMenu.set_label("文件");
    openMenuItem.set_label("打开单词库");
    editWordsMenuItem.set_label("管理单词");
    resetProgressMenuItem.set_label("重置进度");
    settingsMenuItem.set_label("主题设置");
    exitMenuItem.set_label("退出");
    
    fileSubmenu.append(openMenuItem);
    fileSubmenu.append(editWordsMenuItem);
    fileSubmenu.append(resetProgressMenuItem);
    fileSubmenu.append(settingsMenuItem);
    fileSubmenu.append(exitMenuItem);
    fileMenu.set_submenu(fileSubmenu);
    
    viewMenu.set_label("查看");
    wrongWordsMenuItem.set_label("查看错词本");
    clearWrongWordsMenuItem.set_label("清空错词本");
    exportWrongWordsMenuItem.set_label("导出错词本");
    debugMenuItem.set_label("调试信息");
    
    viewSubmenu.append(wrongWordsMenuItem);
    viewSubmenu.append(clearWrongWordsMenuItem);
    viewSubmenu.append(exportWrongWordsMenuItem);
    viewSubmenu.append(debugMenuItem);
    viewMenu.set_submenu(viewSubmenu);
    
    // 在 helpMenu 部分添加 usageMenuItem
    helpMenu.set_label("帮助");
    usageMenuItem.set_label("使用说明");  // 新增
    aboutMenuItem.set_label("关于");

    helpSubmenu.append(usageMenuItem);    // 新增
    helpSubmenu.append(aboutMenuItem);
    helpMenu.set_submenu(helpSubmenu);

    menuBar.append(fileMenu);
    menuBar.append(viewMenu);
    menuBar.append(helpMenu);
    
    // 设置单词显示区域
    wordBox.set_spacing(15);
    wordBox.set_border_width(20);
    wordBox.set_halign(Gtk::ALIGN_CENTER);
    wordBox.get_style_context()->add_class("card");
    
    posLabel.set_justify(Gtk::JUSTIFY_CENTER);
    posLabel.override_font(Pango::FontDescription("Sans Bold 14"));
    posLabel.set_margin_bottom(10);
    posLabel.get_style_context()->add_class("highlight");
    
    meaningLabel.set_justify(Gtk::JUSTIFY_CENTER);
    meaningLabel.override_font(Pango::FontDescription("Sans Bold 20"));
    meaningLabel.set_margin_bottom(15);
    meaningLabel.get_style_context()->add_class("title");
    
    exampleLabel.set_justify(Gtk::JUSTIFY_CENTER);
    exampleLabel.override_font(Pango::FontDescription("Sans 12"));
    exampleLabel.set_line_wrap(true);
    exampleLabel.set_max_width_chars(50);
    exampleLabel.get_style_context()->add_class("subtitle");
    
    wordBox.pack_start(posLabel, Gtk::PACK_SHRINK);
    wordBox.pack_start(meaningLabel, Gtk::PACK_SHRINK);
    wordBox.pack_start(exampleLabel, Gtk::PACK_SHRINK);
    
    // 设置输入区域
    inputBox.set_spacing(15);
    inputBox.set_border_width(20);
    inputBox.set_halign(Gtk::ALIGN_CENTER);
    
    inputHintLabel.set_label("请输入英文单词:");
    inputHintLabel.override_font(Pango::FontDescription("Sans 14"));
    
    answerEntry.set_placeholder_text("在这里输入答案...");
    answerEntry.set_size_request(250, 40);
    answerEntry.override_font(Pango::FontDescription("Sans 14"));
    answerEntry.set_halign(Gtk::ALIGN_CENTER);
    
    submitButton.set_label("提交答案");
    submitButton.override_font(Pango::FontDescription("Sans Bold 12"));
    submitButton.set_size_request(120, 40);
    submitButton.get_style_context()->add_class("primary");
    
    inputBox.pack_start(inputHintLabel, Gtk::PACK_SHRINK);
    inputBox.pack_start(answerEntry, Gtk::PACK_SHRINK);
    inputBox.pack_start(submitButton, Gtk::PACK_SHRINK);
    
    // 设置反馈区域
    feedbackLabel.set_justify(Gtk::JUSTIFY_CENTER);
    feedbackLabel.override_font(Pango::FontDescription("Sans Bold 16"));
    feedbackLabel.set_margin_top(10);
    feedbackLabel.set_margin_bottom(10);
    
    attemptLabel.set_justify(Gtk::JUSTIFY_CENTER);
    attemptLabel.override_font(Pango::FontDescription("Sans 12"));
    
    statusLabel.set_justify(Gtk::JUSTIFY_CENTER);
    statusLabel.override_font(Pango::FontDescription("Sans 11"));
    statusLabel.set_margin_bottom(10);
    
    // 进度条设置
    progressBar.set_show_text(true);
    progressBar.set_fraction(0.0);
    progressBar.set_size_request(300, 20);
    progressBar.set_halign(Gtk::ALIGN_CENTER);
    progressBar.set_margin_bottom(5);
    
    progressLabel.set_justify(Gtk::JUSTIFY_CENTER);
    progressLabel.override_font(Pango::FontDescription("Sans Bold 13"));
    progressLabel.set_margin_bottom(15);
    
    // 设置控制按钮
    controlBox.set_spacing(15);
    controlBox.set_border_width(20);
    controlBox.set_halign(Gtk::ALIGN_CENTER);
    
    showAnswerButton.set_label("查看答案");
    showAnswerButton.override_font(Pango::FontDescription("Sans 12"));
    showAnswerButton.set_size_request(120, 40);
    
    nextWordButton.set_label("下一个单词");
    nextWordButton.override_font(Pango::FontDescription("Sans Bold 12"));
    nextWordButton.set_size_request(120, 40);
    nextWordButton.get_style_context()->add_class("primary");
    
    controlBox.pack_start(showAnswerButton, Gtk::PACK_SHRINK);
    controlBox.pack_start(nextWordButton, Gtk::PACK_SHRINK);
    
    // 设置状态栏
    statusbar.set_hexpand(true);
    
    // 学习界面主布局
    mainBox.set_spacing(10);
    mainBox.pack_start(*titlebarEventBox, Gtk::PACK_SHRINK); // 添加可拖动的标题栏
    mainBox.pack_start(menuBar, Gtk::PACK_SHRINK);
    mainBox.pack_start(wordBox, Gtk::PACK_SHRINK);
    mainBox.pack_start(inputBox, Gtk::PACK_SHRINK);
    mainBox.pack_start(feedbackLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(attemptLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(statusLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(progressBar, Gtk::PACK_SHRINK);
    mainBox.pack_start(progressLabel, Gtk::PACK_SHRINK);
    mainBox.pack_start(controlBox, Gtk::PACK_SHRINK);
    
    // 添加扩展空间，将状态栏推到底部
    Gtk::Box* spacer = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL));
    spacer->set_vexpand(true);
    mainBox.pack_start(*spacer, Gtk::PACK_EXPAND_WIDGET);
    
    // 添加状态栏
    mainBox.pack_end(statusbar, Gtk::PACK_SHRINK);
    
    // 设置完成页面
    completionBox.set_spacing(25);
    completionBox.set_border_width(40);
    completionBox.set_halign(Gtk::ALIGN_CENTER);
    completionBox.set_valign(Gtk::ALIGN_CENTER);
    
    completionTitle.set_label("恭喜你！");
    completionTitle.override_font(Pango::FontDescription("Sans Bold 28"));
    
    completionStats.set_justify(Gtk::JUSTIFY_CENTER);
    completionStats.override_font(Pango::FontDescription("Sans 16"));
    
    restartButton.set_label("重新开始学习");
    restartButton.override_font(Pango::FontDescription("Sans Bold 14"));
    restartButton.set_size_request(200, 50);
    restartButton.get_style_context()->add_class("primary");
    
    reviewWrongWordsButton.set_label("复习错词本");
    reviewWrongWordsButton.override_font(Pango::FontDescription("Sans 14"));
    reviewWrongWordsButton.set_size_request(200, 45);
    
    completionBox.pack_start(completionTitle, Gtk::PACK_SHRINK);
    completionBox.pack_start(completionStats, Gtk::PACK_SHRINK);
    completionBox.pack_start(restartButton, Gtk::PACK_SHRINK);
    completionBox.pack_start(reviewWrongWordsButton, Gtk::PACK_SHRINK);
    
    // 初始显示学习界面
    add(mainBox);
    
    // 连接信号
    openMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_open_file));
    editWordsMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_edit_words));
    resetProgressMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_reset_progress));
    settingsMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_settings));
    exitMenuItem.signal_activate().connect(sigc::ptr_fun(&Gtk::Main::quit));
    
    wrongWordsMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_review_wrong_words));
    clearWrongWordsMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_clear_wrong_words));
    exportWrongWordsMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_export_wrong_words));
    debugMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_debug_info));
    
    submitButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_submit_answer));
    showAnswerButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_show_answer));
    nextWordButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_next_word));
    answerEntry.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_entry_activate));
    
    restartButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_restart));
    reviewWrongWordsButton.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_review_wrong_words));
    
    usageMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_usage));  // 新增
    aboutMenuItem.signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_about));
    
    // 连接窗口控制按钮信号
    minimizeBtn->signal_clicked().connect([this]() {
        iconify(); // 最小化窗口
    });
    
    closeBtn->signal_clicked().connect([this]() {
        hide(); // 隐藏窗口（关闭）
    });
    
    // 启用键盘事件
    add_events(Gdk::KEY_PRESS_MASK);
    
    // 初始状态
    reset_attempt();
    update_display();
    
    // 应用主题
    apply_theme();
    
    show_all_children();
    statusbar.push("请通过菜单加载单词库文件开始学习");
}

MainWindow::~MainWindow() {}

void MainWindow::on_usage() {
    UsageDialog usageDialog(*this);
    apply_dialog_theme(usageDialog);
    usageDialog.run();
}

void MainWindow::apply_dialog_theme(Gtk::Window& dialog) {
    if (settingsManager.isDarkModeEnabled()) {
        dialog.get_style_context()->add_class("dark-mode");
    } else {
        dialog.get_style_context()->add_class("light-mode");
    }
}

void MainWindow::on_about() {
    AboutDialog aboutDialog(*this);
    apply_dialog_theme(aboutDialog);
    aboutDialog.run();
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
    
    apply_dialog_theme(dialog);
    
    int result = dialog.run();
    
    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        if (wordManager.loadWordsFromFile(filename)) {
            show_learning_page();
            on_next_word();
            statusbar.push("已加载单词库: " + filename);
        } else {
            Gtk::MessageDialog errorDialog(*this, "无法加载单词库文件", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            apply_dialog_theme(errorDialog);
            errorDialog.run();
        }
    }
}

void MainWindow::on_edit_words() {
    WordManagerDialog dialog(*this);
    apply_dialog_theme(dialog);
    
    // 可以选择加载当前单词库或创建新的
    Gtk::MessageDialog choiceDialog(*this, "选择操作", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_NONE, true);
    choiceDialog.add_button("创建新单词库", 1);
    choiceDialog.add_button("加载现有单词库", 2);
    choiceDialog.add_button("取消", 3);
    
    apply_dialog_theme(choiceDialog);
    
    int result = choiceDialog.run();
    
    if (result == 1) {
        // 创建新的，直接显示空的管理器
        dialog.run();
    } else if (result == 2) {
        // 加载现有单词库
        Gtk::FileChooserDialog fileDialog("选择单词库文件", Gtk::FILE_CHOOSER_ACTION_OPEN);
        fileDialog.set_transient_for(*this);
        
        fileDialog.add_button("取消", Gtk::RESPONSE_CANCEL);
        fileDialog.add_button("打开", Gtk::RESPONSE_OK);
        
        auto filter_text = Gtk::FileFilter::create();
        filter_text->set_name("文本文件");
        filter_text->add_pattern("*.txt");
        fileDialog.add_filter(filter_text);
        
        apply_dialog_theme(fileDialog);
        
        int fileResult = fileDialog.run();
        
        if (fileResult == Gtk::RESPONSE_OK) {
            std::string filename = fileDialog.get_filename();
            dialog.loadWordsFromFile(filename);
            dialog.run();
        }
    }
    // 如果选择取消，什么都不做
}

void MainWindow::on_next_word() {
    // 如果当前单词曾经拼错过但用户没有正确回答，确保它被记录到错词本
    if (!currentWord.word.empty() && 
        wordManager.hasFailed(currentWord.word) && 
        answerEntry.get_sensitive()) { // 输入框可用表示用户没有正确回答
        // 确保错词被记录
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
    }
}

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
        Gdk::RGBA correctColor;
        if (correctColor.set(settingsManager.getCorrectColor())) {
            feedbackLabel.override_color(correctColor);
        }
        
        // 禁用输入和按钮
        answerEntry.set_sensitive(false);
        submitButton.set_sensitive(false);
        showAnswerButton.set_sensitive(false);
        
        update_progress();
        
        // 检查是否已完成所有单词
        if (wordManager.allWordsMastered()) {
            // 延迟显示完成页面，让用户看到最后一个单词的正确反馈
            Glib::signal_timeout().connect_once(
                sigc::mem_fun(*this, &MainWindow::show_completion_page), 1000);
        }
    } else {
        // 答案错误，立即记录到错词本
        wordManager.addWrongWord(currentWord);
        // 同时记录失败状态
        wordManager.addFailedWord(currentWord.word);
        
        attemptCount++;
        feedbackLabel.set_label("✗ 错误！请再试一次");
        Gdk::RGBA errorColor;
        if (errorColor.set(settingsManager.getErrorColor())) {
            feedbackLabel.override_color(errorColor);
        }
        attemptLabel.set_label("尝试次数: " + std::to_string(attemptCount));
        
        // 清空输入框并聚焦
        answerEntry.set_text("");
        answerEntry.grab_focus();
    }
}

void MainWindow::on_show_answer() {
    // 额外的安全检查：如果已经回答正确，不应该执行任何操作
    if (is_word_completed()) {
        std::cout << "警告：尝试在单词已完成时显示答案" << std::endl;
        return; // 单词已完成，直接返回
    }
    
    feedbackLabel.set_label("正确答案: " + currentWord.word);
    Gdk::RGBA answerColor;
    if (answerColor.set(settingsManager.getAnswerColor())) {
        feedbackLabel.override_color(answerColor);
    }
    
    // 禁用所有输入和按钮
    answerEntry.set_sensitive(false);
    submitButton.set_sensitive(false);
    showAnswerButton.set_sensitive(false);  // 禁用自身
    
    // 添加到错词本
    wordManager.addWrongWord(currentWord);
    // 同时记录失败状态
    wordManager.addFailedWord(currentWord.word);
}

void MainWindow::on_entry_activate() {
    on_submit_answer();
}

void MainWindow::on_restart() {
    wordManager.clearMasteredWords();
    wordManager.clearWrongWords();
    show_learning_page();
    on_next_word();
    statusbar.push("进度已重置，重新开始学习");
}

void MainWindow::on_export_wrong_words() {
    auto wrongWords = wordManager.getWrongWords();
    
    if (wrongWords.empty()) {
        Gtk::MessageDialog infoDialog(*this, "错词本为空，无法导出", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        apply_dialog_theme(infoDialog);
        infoDialog.run();
        return;
    }
    
    // 选择导出文件
    Gtk::FileChooserDialog dialog("导出错词本", Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);
    
    dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
    dialog.add_button("保存", Gtk::RESPONSE_OK);
    
    auto filter_text = Gtk::FileFilter::create();
    filter_text->set_name("文本文件");
    filter_text->add_pattern("*.txt");
    dialog.add_filter(filter_text);
    
    // 设置默认文件名
    dialog.set_current_name("wrong_words.txt");
    
    apply_dialog_theme(dialog);
    
    int result = dialog.run();
    
    if (result == Gtk::RESPONSE_OK) {
        std::string filename = dialog.get_filename();
        
        // 确保文件扩展名
        if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
            filename += ".txt";
        }
        
        // 导出错词到文件
        std::ofstream file(filename);
        
        if (file.is_open()) {
            for (const auto& word : wrongWords) {
                file << word.word << "|" << word.pos << "|" << word.meaning << "|" << word.example << std::endl;
            }
            file.close();
            
            Gtk::MessageDialog successDialog(*this, 
                "错词本已成功导出到文件: " + filename, 
                false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
            apply_dialog_theme(successDialog);
            successDialog.run();
        } else {
            Gtk::MessageDialog errorDialog(*this, "无法保存文件", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
            apply_dialog_theme(errorDialog);
            errorDialog.run();
        }
    }
}

void MainWindow::on_review_wrong_words() {
    auto wrongWords = wordManager.getWrongWords();
    
    if (wrongWords.empty()) {
        Gtk::MessageDialog infoDialog(*this, "错词本为空", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
        apply_dialog_theme(infoDialog);
        infoDialog.run();
        return;
    }
    
    // 创建错词本对话框
    Gtk::Dialog wrongWordsDialog("错词本", *this, true);
    wrongWordsDialog.set_default_size(700, 500);
    
    // 创建主容器
    Gtk::Box mainBox(Gtk::ORIENTATION_VERTICAL, 10);
    mainBox.set_border_width(15);
    
    // 标题和统计
    std::stringstream headerText;
    headerText << "错词本 - 共 " << wrongWords.size() << " 个单词";
    Gtk::Label headerLabel(headerText.str());
    headerLabel.override_font(Pango::FontDescription("Sans Bold 16"));
    mainBox.pack_start(headerLabel, Gtk::PACK_SHRINK);
    
    // 创建滚动窗口
    Gtk::ScrolledWindow scrolledWindow;
    scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    scrolledWindow.set_min_content_height(350);
    
    // 创建单词网格
    Gtk::Grid wordsGrid;
    wordsGrid.set_row_spacing(10);
    wordsGrid.set_column_spacing(15);
    wordsGrid.set_border_width(10);
    wordsGrid.set_halign(Gtk::ALIGN_CENTER);
    
    // 显示每个错词
    for (size_t i = 0; i < wrongWords.size(); ++i) {
        const auto& word = wrongWords[i];
        
        // 创建单词卡片
        Gtk::Frame* wordFrame = Gtk::manage(new Gtk::Frame());
        wordFrame->set_shadow_type(Gtk::SHADOW_ETCHED_IN);
        
        Gtk::Box* wordBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_VERTICAL, 8));
        wordBox->set_border_width(12);
        
        // 单词和词性
        Gtk::Box* wordInfoBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 8));
        Gtk::Label* wordLabel = Gtk::manage(new Gtk::Label(word.word));
        wordLabel->override_font(Pango::FontDescription("Sans Bold 14"));
        
        Gtk::Label* posLabel = Gtk::manage(new Gtk::Label("[" + word.pos + "]"));
        Gdk::RGBA posColor;
        if (posColor.set(settingsManager.getPosColor())) {
            posLabel->override_color(posColor);
        }
        
        wordInfoBox->pack_start(*wordLabel, Gtk::PACK_SHRINK);
        wordInfoBox->pack_start(*posLabel, Gtk::PACK_SHRINK);
        
        // 中文意思
        Gtk::Label* meaningLabel = Gtk::manage(new Gtk::Label("含义: " + word.meaning));
        meaningLabel->set_alignment(0, 0.5);
        meaningLabel->override_font(Pango::FontDescription("Sans 12"));
        
        // 例句
        Gtk::Label* exampleLabel = Gtk::manage(new Gtk::Label("例句: " + word.example));
        exampleLabel->set_alignment(0, 0.5);
        exampleLabel->override_font(Pango::FontDescription("Sans 11"));
        exampleLabel->set_line_wrap(true);
        exampleLabel->set_max_width_chars(40);
        
        wordBox->pack_start(*wordInfoBox, Gtk::PACK_SHRINK);
        wordBox->pack_start(*meaningLabel, Gtk::PACK_SHRINK);
        wordBox->pack_start(*exampleLabel, Gtk::PACK_SHRINK);
        
        wordFrame->add(*wordBox);
        
        // 将卡片添加到网格（2列布局）
        int row = i / 2;
        int col = i % 2;
        wordsGrid.attach(*wordFrame, col, row, 1, 1);
    }
    
    // 将网格放入滚动窗口
    scrolledWindow.add(wordsGrid);
    
    // 创建按钮
    Gtk::ButtonBox buttonBox(Gtk::ORIENTATION_HORIZONTAL);
    buttonBox.set_layout(Gtk::BUTTONBOX_END);
    buttonBox.set_spacing(10);
    
    Gtk::Button closeButton("关闭");
    Gtk::Button clearButton("清空错词本");
    Gtk::Button exportButton("导出错词本");
    
    buttonBox.pack_start(exportButton, Gtk::PACK_SHRINK);
    buttonBox.pack_start(clearButton, Gtk::PACK_SHRINK);
    buttonBox.pack_start(closeButton, Gtk::PACK_SHRINK);
    
    // 布局
    mainBox.pack_start(scrolledWindow, Gtk::PACK_EXPAND_WIDGET);
    mainBox.pack_start(buttonBox, Gtk::PACK_SHRINK);
    
    wrongWordsDialog.get_content_area()->pack_start(mainBox);
    
    // 应用主题
    apply_dialog_theme(wrongWordsDialog);
    
    // 连接信号
    closeButton.signal_clicked().connect([&wrongWordsDialog]() {
        wrongWordsDialog.response(Gtk::RESPONSE_OK);
    });
    
    clearButton.signal_clicked().connect([this, &wrongWordsDialog]() {
        Gtk::MessageDialog confirmDialog(wrongWordsDialog, "确定要清空错词本吗？", false, 
                                        Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO, true);
        this->apply_dialog_theme(confirmDialog);  // 使用 this-> 明确调用
        if (confirmDialog.run() == Gtk::RESPONSE_YES) {
            wordManager.clearWrongWords();
            wrongWordsDialog.response(Gtk::RESPONSE_OK);
            statusbar.push("错词本已清空");
        }
    });
    
    exportButton.signal_clicked().connect([this, &wrongWordsDialog]() {
        wrongWordsDialog.response(Gtk::RESPONSE_OK);
        on_export_wrong_words();
    });
    
    wrongWordsDialog.show_all_children();
    wrongWordsDialog.run();
}

void MainWindow::on_clear_wrong_words() {
    wordManager.clearWrongWords();
    statusbar.push("错词本已清空");
}

void MainWindow::on_reset_progress() {
    wordManager.clearMasteredWords();
    wordManager.clearWrongWords();
    update_progress();
    statusbar.push("学习进度已重置");
}

void MainWindow::on_settings() {
    SettingsDialog dialog(*this, settingsManager);
    apply_dialog_theme(dialog);
    
    int result = dialog.run();
    
    if (result == Gtk::RESPONSE_OK) {
        // 用户点击了保存，重新应用主题
        apply_theme();
        
        // 如果当前有单词显示，也更新其颜色
        if (!currentWord.word.empty()) {
            // 重新设置词性标签颜色
            Gdk::RGBA posColor;
            if (posColor.set(settingsManager.getPosColor())) {
                posLabel.override_color(posColor);
            }
        }
        
        // 显示确认消息
        statusbar.push("主题设置已更新");
    } else {
        // 如果取消，恢复之前的设置
        settingsManager.loadSettings();
    }
}

void MainWindow::on_debug_info() {
    std::stringstream debugInfo;
    debugInfo << "调试信息:\n\n";
    debugInfo << "总单词数: " << wordManager.getTotalWords() << "\n";
    debugInfo << "已掌握单词: " << wordManager.getMasteredWordsCount() << "\n";
    debugInfo << "错词数量: " << wordManager.getWrongWordsCount() << "\n";
    debugInfo << "剩余单词: " << wordManager.getRemainingWordsCount() << "\n";
    
    debugInfo << "\n当前单词: ";
    if (!currentWord.word.empty()) {
        debugInfo << currentWord.word << " [" << currentWord.pos << "]";
        debugInfo << "\n是否曾经失败: " << (wordManager.hasFailed(currentWord.word) ? "是" : "否");
    } else {
        debugInfo << "无";
    }
    
    debugInfo << "\n\n颜色设置:\n";
    debugInfo << "词性颜色: " << settingsManager.getPosColor() << "\n";
    debugInfo << "正确颜色: " << settingsManager.getCorrectColor() << "\n";
    debugInfo << "错误颜色: " << settingsManager.getErrorColor() << "\n";
    debugInfo << "答案颜色: " << settingsManager.getAnswerColor() << "\n";
    
    Gtk::MessageDialog dialog(*this, debugInfo.str(), false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
    apply_dialog_theme(dialog);
    dialog.set_title("调试信息");
    dialog.run();
}

void MainWindow::update_display() {
    if (!currentWord.word.empty()) {
        posLabel.set_label("[" + currentWord.pos + "]");
        meaningLabel.set_label(currentWord.meaning);
        exampleLabel.set_label("例句: " + currentWord.example);
        
        // 更新状态标签
        if (wordManager.hasFailed(currentWord.word)) {
            statusLabel.set_label("⚠️ 这个单词你曾经拼错过");
            statusLabel.override_color(Gdk::RGBA("orange"));
        } else {
            statusLabel.set_label("");
        }
        
        // 重置反馈
        feedbackLabel.set_label("");
        attemptLabel.set_label("");
        
        // 启用所有输入和按钮
        answerEntry.set_sensitive(true);
        submitButton.set_sensitive(true);
        showAnswerButton.set_sensitive(true);
        
        answerEntry.set_text("");
        answerEntry.grab_focus();
    } else {
        posLabel.set_label("");
        meaningLabel.set_label("请加载单词库文件");
        exampleLabel.set_label("");
        feedbackLabel.set_label("");
        attemptLabel.set_label("");
        statusLabel.set_label("");
        
        // 当没有单词时，禁用输入和按钮
        answerEntry.set_sensitive(false);
        submitButton.set_sensitive(false);
        showAnswerButton.set_sensitive(false);
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
        
        // 更新进度条
        progressBar.set_fraction(progress);
        progressBar.set_text(Glib::ustring::compose("%1%", percentage));
        
        std::stringstream progressText;
        progressText << "进度: " << wordManager.getMasteredWordsCount() << "/" 
                    << wordManager.getTotalWords() << " (" << percentage << "%)";
        
        progressLabel.set_label(progressText.str());
        
        // 更新状态栏
        std::stringstream statusText;
        statusText << "✅ 已掌握: " << wordManager.getMasteredWordsCount() 
                  << " / 📝 总单词: " << wordManager.getTotalWords() 
                  << " | ❌ 错词: " << wordManager.getWrongWordsCount()
                  << " | ⏳ 剩余: " << wordManager.getRemainingWordsCount();
        
        statusbar.push(statusText.str());
    } else {
        progressBar.set_fraction(0.0);
        progressBar.set_text("0%");
        progressLabel.set_label("进度: 0/0 (0%)");
        statusbar.push("请加载单词库开始学习");
    }
}

void MainWindow::apply_theme_colors() {
    // 应用词性颜色
    Gdk::RGBA posColor;
    if (posColor.set(settingsManager.getPosColor())) {
        posLabel.override_color(posColor);
    } else {
        // 如果颜色设置失败，使用主题主色调
        posColor.set(settingsManager.getPrimaryColor());
        posLabel.override_color(posColor);
    }
    
    // 其他标签使用系统默认颜色
    // 不设置任何颜色，让系统主题决定
}

void MainWindow::update_css_style() {
    auto cssProvider = Gtk::CssProvider::create();
    
    // 获取当前主题颜色
    std::string primaryColor = settingsManager.getPrimaryColor();
    std::string secondaryColor = settingsManager.getSecondaryColor();
    std::string accentColor = settingsManager.getAccentColor();
    std::string backgroundColor = settingsManager.getBackgroundColor();
    std::string textColor = settingsManager.getTextColor();
    std::string cardBackgroundColor = settingsManager.getCardBackgroundColor();
    std::string borderColor = settingsManager.getBorderColor();
    
    // 构建动态CSS
    std::string dynamicCSS = 
        "/* 动态主题样式 */\n"
        ".primary-color { color: " + primaryColor + "; }\n"
        ".secondary-color { color: " + secondaryColor + "; }\n"
        ".accent-color { color: " + accentColor + "; }\n"
        ".primary-bg { background-color: " + primaryColor + "; }\n"
        ".secondary-bg { background-color: " + secondaryColor + "; }\n"
        ".accent-bg { background-color: " + accentColor + "; }\n"
        
        // 按钮主色调
        "button.primary {\n"
        "   background: " + primaryColor + ";\n"
        "   border-color: " + secondaryColor + ";\n"
        "   color: white;\n"
        "}\n"
        "button.primary:hover {\n"
        "   background: " + secondaryColor + ";\n"
        "}\n"
        
        // 进度条颜色
        "progressbar progress {\n"
        "   background: " + primaryColor + ";\n"
        "}\n"
        
        // 输入框焦点颜色
        "entry:focus {\n"
        "   border-color: " + primaryColor + ";\n"
        "   box-shadow: 0 0 0 3px " + accentColor + "30;\n"
        "}\n"
        
        // 高亮标签颜色
        ".highlight {\n"
        "   color: " + primaryColor + ";\n"
        "}\n";
    
    try {
        cssProvider->load_from_data(dynamicCSS);
        
        // 添加到样式提供器
        Gtk::StyleContext::add_provider_for_screen(
            Gdk::Screen::get_default(),
            cssProvider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION
        );
    } catch (const Gtk::CssProviderError& ex) {
        std::cerr << "动态CSS加载错误: " << ex.what() << std::endl;
    }
}

void MainWindow::apply_theme() {
    bool useDarkMode = settingsManager.isDarkModeEnabled();
    
    auto styleContext = get_style_context();
    
    // 移除现有主题类
    styleContext->remove_class("light-mode");
    styleContext->remove_class("dark-mode");
    
    // 添加新主题类
    if (useDarkMode) {
        styleContext->add_class("dark-mode");
    } else {
        styleContext->add_class("light-mode");
    }
    
    // 重新应用颜色设置
    apply_theme_colors();
    
    // 更新动态CSS样式
    update_css_style();
}


void MainWindow::toggle_dark_mode() {
    bool currentMode = settingsManager.isDarkModeEnabled();
    settingsManager.setDarkModeEnabled(!currentMode);
    apply_theme();
}

bool MainWindow::on_key_press_event(GdkEventKey* event) {
    // 检查是否按下 Ctrl+Tab - 切换到下一个单词
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_Tab) {
        // 确保当前有单词正在学习，且单词未完成
        if (!currentWord.word.empty()/* && answerEntry.get_sensitive()*/) {
            on_next_word();
            return true; // 事件已处理
        }
    }
    
    // 检查是否按下 Alt+S - 显示答案
    if ((event->state & GDK_MOD1_MASK) && (event->keyval == GDK_KEY_s || event->keyval == GDK_KEY_S)) {
        // 确保当前有单词正在学习，且单词未完成
        if (!currentWord.word.empty() && answerEntry.get_sensitive()) {
            on_show_answer();
            return true; // 事件已处理
        }
    }
    
    // 检查是否按下 Ctrl+Shift+D - 切换深色模式（原有功能）
    if ((event->state & GDK_CONTROL_MASK) && (event->state & GDK_SHIFT_MASK)) {
        if (event->keyval == GDK_KEY_d || event->keyval == GDK_KEY_D) {
            toggle_dark_mode();
            return true;
        }
    }
    
    // 调用基类处理其他事件
    return Gtk::Window::on_key_press_event(event);
}