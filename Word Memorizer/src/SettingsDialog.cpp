#include "SettingsDialog.h"
#include <iostream>

SettingsDialog::SettingsDialog(Gtk::Window& parent, SettingsManager& settings)
    : Dialog("主题设置", parent, true),
      settingsManager(settings),
      mainBox(Gtk::ORIENTATION_VERTICAL),
      themeBox(Gtk::ORIENTATION_VERTICAL),
      darkModeBox(Gtk::ORIENTATION_HORIZONTAL),
      colorThemeBox(Gtk::ORIENTATION_HORIZONTAL),
      colorBox(Gtk::ORIENTATION_VERTICAL),
      posColorBox(Gtk::ORIENTATION_HORIZONTAL),
      correctColorBox(Gtk::ORIENTATION_HORIZONTAL),
      errorColorBox(Gtk::ORIENTATION_HORIZONTAL),
      answerColorBox(Gtk::ORIENTATION_HORIZONTAL),
      buttonBox(Gtk::ORIENTATION_HORIZONTAL) {
    
    // 移除标题栏和边框
    set_decorated(false);
    set_default_size(400, 550);
    set_border_width(15);
    
    // 创建自定义标题栏
    Gtk::Box* titlebarBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
    titlebarBox->set_border_width(8);
    titlebarBox->set_halign(Gtk::ALIGN_FILL);
    titlebarBox->set_hexpand(true);
    titlebarBox->get_style_context()->add_class("custom-titlebar");
    
    // 窗口标题
    Gtk::Label* dialogTitle = Gtk::manage(new Gtk::Label("主题设置"));
    dialogTitle->set_halign(Gtk::ALIGN_START);
    dialogTitle->set_hexpand(true);
    dialogTitle->get_style_context()->add_class("custom-titlebar-label");
    
    // 关闭按钮
    Gtk::Button* closeBtn = Gtk::manage(new Gtk::Button("×"));
    closeBtn->get_style_context()->add_class("custom-window-control");
    closeBtn->get_style_context()->add_class("close");
    closeBtn->set_tooltip_text("关闭");
    closeBtn->set_size_request(16, 16);
    
    titlebarBox->pack_start(*dialogTitle, Gtk::PACK_EXPAND_WIDGET);
    titlebarBox->pack_end(*closeBtn, Gtk::PACK_SHRINK);
    
    // 标题
    titleLabel.set_label("主题设置");
    titleLabel.override_font(Pango::FontDescription("Sans Bold 18"));
    mainBox.pack_start(*titlebarBox, Gtk::PACK_SHRINK);  // 添加自定义标题栏
    mainBox.pack_start(titleLabel, Gtk::PACK_SHRINK);
    
    // 主题设置框架
    themeFrame.set_label("明暗主题");
    themeFrame.set_border_width(10);
    
    themeBox.set_spacing(10);
    themeBox.set_border_width(10);
    
    // 深色模式开关
    darkModeLabel.set_label("深色模式:");
    darkModeLabel.set_size_request(120, -1);
    
    darkModeBox.pack_start(darkModeLabel, Gtk::PACK_SHRINK);
    darkModeBox.pack_start(darkModeSwitch, Gtk::PACK_SHRINK);
    
    themeBox.pack_start(darkModeBox, Gtk::PACK_SHRINK);
    themeFrame.add(themeBox);
    
    mainBox.pack_start(themeFrame, Gtk::PACK_SHRINK);
    
    // 颜色主题设置框架
    colorThemeFrame.set_label("颜色主题");
    colorThemeFrame.set_border_width(10);
    
    colorThemeBox.set_spacing(10);
    colorThemeBox.set_border_width(10);
    
    // 颜色主题选择
    colorThemeLabel.set_label("主题风格:");
    colorThemeLabel.set_size_request(120, -1);
    
    // 添加颜色主题选项
    colorThemeCombo.append("blue", "蓝色主题");
    colorThemeCombo.append("red", "红色主题");
    colorThemeCombo.append("orange", "橙色主题");
    colorThemeCombo.append("green", "绿色主题");
    colorThemeCombo.append("cyan", "青色主题");
    colorThemeCombo.append("yellow", "黄色主题");
    colorThemeCombo.append("brown", "棕色主题");
    colorThemeCombo.append("mono", "黑白主题");
    colorThemeCombo.append("purple", "紫色主题");
    colorThemeCombo.append("pink", "粉色主题");
    
    colorThemeBox.pack_start(colorThemeLabel, Gtk::PACK_SHRINK);
    colorThemeBox.pack_start(colorThemeCombo, Gtk::PACK_EXPAND_WIDGET);
    
    colorThemeFrame.add(colorThemeBox);
    mainBox.pack_start(colorThemeFrame, Gtk::PACK_SHRINK);
    
    // 颜色设置框架
    colorFrame.set_label("自定义颜色");
    colorFrame.set_border_width(10);
    
    colorBox.set_spacing(10);
    colorBox.set_border_width(10);
    
    // 词性颜色设置
    posColorLabel.set_label("词性颜色:");
    posColorLabel.set_size_request(120, -1);
    posColorBox.pack_start(posColorLabel, Gtk::PACK_SHRINK);
    posColorBox.pack_start(posColorButton, Gtk::PACK_EXPAND_WIDGET);
    
    // 正确颜色设置
    correctColorLabel.set_label("正确颜色:");
    correctColorLabel.set_size_request(120, -1);
    correctColorBox.pack_start(correctColorLabel, Gtk::PACK_SHRINK);
    correctColorBox.pack_start(correctColorButton, Gtk::PACK_EXPAND_WIDGET);
    
    // 错误颜色设置
    errorColorLabel.set_label("错误颜色:");
    errorColorLabel.set_size_request(120, -1);
    errorColorBox.pack_start(errorColorLabel, Gtk::PACK_SHRINK);
    errorColorBox.pack_start(errorColorButton, Gtk::PACK_EXPAND_WIDGET);
    
    // 答案颜色设置
    answerColorLabel.set_label("答案颜色:");
    answerColorLabel.set_size_request(120, -1);
    answerColorBox.pack_start(answerColorLabel, Gtk::PACK_SHRINK);
    answerColorBox.pack_start(answerColorButton, Gtk::PACK_EXPAND_WIDGET);
    
    colorBox.pack_start(posColorBox, Gtk::PACK_SHRINK);
    colorBox.pack_start(correctColorBox, Gtk::PACK_SHRINK);
    colorBox.pack_start(errorColorBox, Gtk::PACK_SHRINK);
    colorBox.pack_start(answerColorBox, Gtk::PACK_SHRINK);
    
    colorFrame.add(colorBox);
    mainBox.pack_start(colorFrame, Gtk::PACK_SHRINK);
    
    // 按钮
    buttonBox.set_spacing(10);
    buttonBox.set_border_width(10);
    
    saveButton.set_label("保存");
    cancelButton.set_label("取消");
    resetButton.set_label("重置默认");
    
    saveButton.get_style_context()->add_class("primary");
    
    buttonBox.pack_start(saveButton, Gtk::PACK_EXPAND_WIDGET);
    buttonBox.pack_start(resetButton, Gtk::PACK_EXPAND_WIDGET);
    buttonBox.pack_start(cancelButton, Gtk::PACK_EXPAND_WIDGET);
    
    mainBox.pack_start(buttonBox, Gtk::PACK_SHRINK);
    
    get_content_area()->pack_start(mainBox);
    
    // 连接信号
    closeBtn->signal_clicked().connect([this]() {response(Gtk::RESPONSE_CANCEL);});
    saveButton.signal_clicked().connect(sigc::mem_fun(*this, &SettingsDialog::on_save_clicked));
    cancelButton.signal_clicked().connect(sigc::mem_fun(*this, &SettingsDialog::on_cancel_clicked));
    resetButton.signal_clicked().connect(sigc::mem_fun(*this, &SettingsDialog::on_reset_clicked));
    darkModeSwitch.property_active().signal_changed().connect(
        sigc::mem_fun(*this, &SettingsDialog::on_dark_mode_toggled));
    colorThemeCombo.signal_changed().connect(
        sigc::mem_fun(*this, &SettingsDialog::on_color_theme_changed));
    
    // 加载当前设置
    load_current_colors();
    load_current_theme();
    darkModeSwitch.set_active(settingsManager.isDarkModeEnabled());
    
    show_all_children();
}

SettingsDialog::~SettingsDialog() {
    // 析构函数实现
}

void SettingsDialog::load_current_colors() {
    // 设置颜色按钮的当前值
    Gdk::RGBA color;
    
    color.set(settingsManager.getPosColor());
    posColorButton.set_rgba(color);
    
    color.set(settingsManager.getCorrectColor());
    correctColorButton.set_rgba(color);
    
    color.set(settingsManager.getErrorColor());
    errorColorButton.set_rgba(color);
    
    color.set(settingsManager.getAnswerColor());
    answerColorButton.set_rgba(color);
}

void SettingsDialog::load_current_theme() {
    std::string currentTheme = settingsManager.getColorTheme();
    colorThemeCombo.set_active_id(currentTheme);
}

void SettingsDialog::on_save_clicked() {
    // 保存颜色设置
    settingsManager.setPosColor(posColorButton.get_rgba().to_string());
    settingsManager.setCorrectColor(correctColorButton.get_rgba().to_string());
    settingsManager.setErrorColor(errorColorButton.get_rgba().to_string());
    settingsManager.setAnswerColor(answerColorButton.get_rgba().to_string());
    
    // 保存深色模式设置
    settingsManager.setDarkModeEnabled(darkModeSwitch.get_active());
    
    // 保存颜色主题
    if (colorThemeCombo.get_active_id() != "") {
        settingsManager.setColorTheme(colorThemeCombo.get_active_id());
    }
    
    response(Gtk::RESPONSE_OK);
}

void SettingsDialog::on_cancel_clicked() {
    response(Gtk::RESPONSE_CANCEL);
}

void SettingsDialog::on_reset_clicked() {
    // 重置为默认颜色
    Gdk::RGBA color;
    
    // 使用系统主题友好的颜色
    color.set("rgb(52,152,219)");  // 蓝色
    posColorButton.set_rgba(color);
    
    color.set("rgb(46,204,113)");  // 绿色
    correctColorButton.set_rgba(color);
    
    color.set("rgb(231,76,60)");   // 红色
    errorColorButton.set_rgba(color);
    
    color.set("rgb(155,89,182)");  // 紫色
    answerColorButton.set_rgba(color);
    
    // 重置颜色主题为蓝色
    colorThemeCombo.set_active_id("blue");
    
    // 重置深色模式为系统检测
    settingsManager.setDarkModeEnabled(settingsManager.detectSystemDarkMode());
    darkModeSwitch.set_active(settingsManager.isDarkModeEnabled());
}

void SettingsDialog::on_dark_mode_toggled() {
    // 实时切换深色模式预览
    settingsManager.setDarkModeEnabled(darkModeSwitch.get_active());
}

void SettingsDialog::on_color_theme_changed() {
    // 实时切换颜色主题预览
    if (colorThemeCombo.get_active_id() != "") {
        settingsManager.setColorTheme(colorThemeCombo.get_active_id());
    }
}