#include "SettingsDialog.h"
#include <iostream>

SettingsDialog::SettingsDialog(Gtk::Window& parent, SettingsManager& settings)
    : Dialog("主题设置", parent, true),
      settingsManager(settings),
      mainBox(Gtk::ORIENTATION_VERTICAL),
      colorBox(Gtk::ORIENTATION_VERTICAL),
      posColorBox(Gtk::ORIENTATION_HORIZONTAL),
      correctColorBox(Gtk::ORIENTATION_HORIZONTAL),
      errorColorBox(Gtk::ORIENTATION_HORIZONTAL),
      answerColorBox(Gtk::ORIENTATION_HORIZONTAL),
      buttonBox(Gtk::ORIENTATION_HORIZONTAL) {
    
    set_default_size(400, 300);
    set_border_width(10);
    
    // 标题
    titleLabel.set_label("自定义主题颜色");
    titleLabel.override_font(Pango::FontDescription("Sans Bold 16"));
    mainBox.pack_start(titleLabel, Gtk::PACK_SHRINK);
    
    // 颜色设置框架
    colorFrame.set_label("颜色设置");
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
    
    buttonBox.pack_start(saveButton, Gtk::PACK_EXPAND_WIDGET);
    buttonBox.pack_start(resetButton, Gtk::PACK_EXPAND_WIDGET);
    buttonBox.pack_start(cancelButton, Gtk::PACK_EXPAND_WIDGET);
    
    mainBox.pack_start(buttonBox, Gtk::PACK_SHRINK);
    
    get_content_area()->pack_start(mainBox);
    
    // 连接信号
    saveButton.signal_clicked().connect(sigc::mem_fun(*this, &SettingsDialog::on_save_clicked));
    cancelButton.signal_clicked().connect(sigc::mem_fun(*this, &SettingsDialog::on_cancel_clicked));
    resetButton.signal_clicked().connect(sigc::mem_fun(*this, &SettingsDialog::on_reset_clicked));
    
    // 加载当前颜色
    load_current_colors();
    
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

void SettingsDialog::on_save_clicked() {
    // 保存颜色设置
    settingsManager.setPosColor(posColorButton.get_rgba().to_string());
    settingsManager.setCorrectColor(correctColorButton.get_rgba().to_string());
    settingsManager.setErrorColor(errorColorButton.get_rgba().to_string());
    settingsManager.setAnswerColor(answerColorButton.get_rgba().to_string());
    
    response(Gtk::RESPONSE_OK);
}

void SettingsDialog::on_cancel_clicked() {
    response(Gtk::RESPONSE_CANCEL);
}

void SettingsDialog::on_reset_clicked() {
    // 重置为默认颜色
    Gdk::RGBA color;
    
    color.set("blue");
    posColorButton.set_rgba(color);
    
    color.set("green");
    correctColorButton.set_rgba(color);
    
    color.set("red");
    errorColorButton.set_rgba(color);
    
    color.set("blue");
    answerColorButton.set_rgba(color);
}