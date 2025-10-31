#include "AboutDialog.h"  // 修改这里，从 "MainWindow.h" 改为 "AboutDialog.h"
#include <iostream>

AboutDialog::AboutDialog(Gtk::Window& parent)
   : Dialog("关于", parent, true),
   mainBox(Gtk::ORIENTATION_VERTICAL) {
   
   // 移除标题栏和边框
   set_decorated(false);
   set_default_size(400, 300);
   set_border_width(20);
   set_resizable(false);
   
   // 创建自定义标题栏
   Gtk::Box* titlebarBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
   titlebarBox->set_border_width(8);
   titlebarBox->set_halign(Gtk::ALIGN_FILL);
   titlebarBox->set_hexpand(true);
   titlebarBox->get_style_context()->add_class("custom-titlebar");
   
   // 窗口标题
   Gtk::Label* dialogTitle = Gtk::manage(new Gtk::Label("关于"));
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
   
   // 设置标题
   titleLabel.set_label("VocabMemster");
   titleLabel.override_font(Pango::FontDescription("Sans Bold 20"));
   titleLabel.set_margin_bottom(10);
   titleLabel.get_style_context()->add_class("title");
   
   // 版本信息
   versionLabel.set_label("Ver 2.1.0 Stable");
   versionLabel.override_font(Pango::FontDescription("Sans 12"));
   versionLabel.set_margin_bottom(10);
   versionLabel.get_style_context()->add_class("subtitle");
   
   // 描述
   descriptionLabel.set_label("简约高效的单词背诵工具");
   descriptionLabel.set_justify(Gtk::JUSTIFY_CENTER);
   descriptionLabel.set_line_wrap(true);
   descriptionLabel.set_max_width_chars(40);
   descriptionLabel.set_margin_bottom(15);
   descriptionLabel.get_style_context()->add_class("muted");
   
   // 作者信息
   authorLabel.set_label("开发者: Federico Prask");
   authorLabel.override_font(Pango::FontDescription("Sans 10"));
   authorLabel.set_margin_bottom(10);
   authorLabel.get_style_context()->add_class("muted");
   
   // 许可证信息
   licenseLabel.set_label("许可证: MIT License");
   licenseLabel.override_font(Pango::FontDescription("Sans 10"));
   licenseLabel.set_margin_bottom(20);
   licenseLabel.get_style_context()->add_class("muted");
   
   // 关闭按钮
   closeButton.set_label("关闭");
   closeButton.set_size_request(100, 35);
   closeButton.get_style_context()->add_class("primary");
   
   // 布局
   mainBox.set_spacing(5);
   mainBox.pack_start(*titlebarBox, Gtk::PACK_SHRINK);  // 添加自定义标题栏
   mainBox.pack_start(titleLabel, Gtk::PACK_SHRINK);
   mainBox.pack_start(versionLabel, Gtk::PACK_SHRINK);
   mainBox.pack_start(descriptionLabel, Gtk::PACK_SHRINK);
   mainBox.pack_start(authorLabel, Gtk::PACK_SHRINK);
   mainBox.pack_start(licenseLabel, Gtk::PACK_SHRINK);
   mainBox.pack_start(closeButton, Gtk::PACK_SHRINK);
   
   // 为主盒子添加卡片样式
   mainBox.get_style_context()->add_class("card");
   
   get_content_area()->pack_start(mainBox);
   
   // 连接信号
   closeButton.signal_clicked().connect([this]() {
      response(Gtk::RESPONSE_OK);
   });
   
   closeBtn->signal_clicked().connect([this]() {
      response(Gtk::RESPONSE_OK);
   });
   
   show_all_children();
}

AboutDialog::~AboutDialog() {}