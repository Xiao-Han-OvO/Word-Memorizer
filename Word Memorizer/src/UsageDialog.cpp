#include "UsageDialog.h"
#include <iostream>


UsageDialog::UsageDialog(Gtk::Window& parent)
   : Dialog("使用说明", parent, true),
   mainBox(Gtk::ORIENTATION_VERTICAL) {
   
   // 移除标题栏和边框
   set_decorated(false);
   set_default_size(600, 500);
   set_border_width(20);
   
   // 创建自定义标题栏
   Gtk::Box* titlebarBox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 0));
   titlebarBox->set_border_width(8);
   titlebarBox->set_halign(Gtk::ALIGN_FILL);
   titlebarBox->set_hexpand(true);
   titlebarBox->get_style_context()->add_class("custom-titlebar");
   
   // 窗口标题
   Gtk::Label* dialogTitle = Gtk::manage(new Gtk::Label("使用说明"));
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
   Gtk::Label* titleLabel = Gtk::manage(new Gtk::Label("单词记忆大师 - 使用说明"));
   titleLabel->override_font(Pango::FontDescription("Sans Bold 18"));
   titleLabel->set_margin_bottom(15);
   titleLabel->get_style_context()->add_class("title");
   
   // 滚动窗口
   scrolledWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS);
   scrolledWindow.set_min_content_height(350);
   
   // 内容标签
   contentLabel.set_justify(Gtk::JUSTIFY_LEFT);
   contentLabel.set_line_wrap(true);
   contentLabel.set_selectable(true);
   
   // 设置使用说明内容
   Glib::ustring usageText = R"(
   <span font_desc="Sans Bold 16">📖 单词记忆大师使用指南</span>

   <span font_desc="Sans Bold 14" foreground="#3498db">🎯 基本操作</span>

   <b>• 开始学习</b>
      - 通过 <i>文件 → 打开单词库</i> 加载你的单词文件
      - 单词文件格式：单词|词性|释义|例句

   <b>• 回答单词</b>
      - 在输入框中输入英文单词
      - 按 <i>回车键</i> 或点击 <i>提交答案</i> 按钮

   <b>• 导航控制</b>
      - <i>下一个单词</i>：加载新的单词
      - <i>查看答案</i>：显示当前单词的正确答案

   <span font_desc="Sans Bold 14" foreground="#2ecc71">⌨️ 快捷键</span>

   • <b>Ctrl + Tab</b> - 切换到下一个单词
   • <b>Alt + S</b> - 显示当前单词答案  
   • <b>Ctrl + Shift + D</b> - 切换深色/浅色模式
   • <b>回车键</b> - 提交答案

   <span font_desc="Sans Bold 14" foreground="#e74c3c">📊 学习功能</span>

   <b>• 进度跟踪</b>
      - 实时显示学习进度百分比
      - 掌握单词数量统计
      - 错词自动记录

   <b>• 错词本</b>
      - 自动记录拼写错误的单词
      - 支持错词复习和导出
      - 可通过 <i>查看 → 查看错词本</i> 访问

   <span font_desc="Sans Bold 14" foreground="#9b59b6">🛠️ 高级功能</span>

   <b>• 单词管理</b>
      - 通过 <i>文件 → 管理单词</i> 编辑单词库
      - 添加、删除、修改单词
      - 导入/导出单词文件

   <b>• 进度管理</b>
      - 重置学习进度
      - 清空错词本
      - 导出学习数据

   <span font_desc="Sans Bold 14" foreground="#f39c12">💡 学习技巧</span>

   1. <b>定期复习</b>：建议每天坚持学习15-30分钟
   2. <b>主动回忆</b>：尽量先尝试回忆，再看答案
   3. <b>结合例句</b>：通过例句理解单词的用法
   4. <b>错词重点</b>：针对错词进行专项练习

   <span font_desc="Sans Bold 12" foreground="#7f8c8d">📝 文件格式说明</span>

   单词库文件为文本格式，每行一个单词，格式为：
   <tt>单词|词性|中文释义|例句</tt>

   示例：
   <tt>apple|n.|苹果|I eat an apple every day.</tt>
   <tt>run|v.|跑步|He runs in the park every morning.</tt>

   <span font_desc="Sans Bold 12" foreground="#446383ff">🆘 获取帮助</span>

   如遇问题，可通过以下方式：
   • 查看调试信息：<i>查看 → 调试信息</i>
   • 重置应用程序：<i>文件 → 重置进度</i>
   • 检查单词文件格式是否正确

   <span foreground="#2c3e50" font_desc="Sans Italic 10">版本 2.1.0 Alpha RC 2 - 设计用于高效单词记忆</span>
   )";

   contentLabel.set_markup(usageText);
   contentLabel.set_margin_bottom(15);
   
   // 将内容标签放入滚动窗口
   scrolledWindow.add(contentLabel);
   
   // 关闭按钮
   closeButton.set_label("关闭");
   closeButton.set_size_request(100, 35);
   closeButton.get_style_context()->add_class("primary");
   closeButton.set_margin_top(10);
   
   // 布局
   mainBox.set_spacing(10);
   mainBox.pack_start(*titlebarBox, Gtk::PACK_SHRINK);  // 添加自定义标题栏
   mainBox.pack_start(*titleLabel, Gtk::PACK_SHRINK);
   mainBox.pack_start(scrolledWindow, Gtk::PACK_EXPAND_WIDGET);
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

UsageDialog::~UsageDialog() {}
