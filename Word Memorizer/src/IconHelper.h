#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <gtkmm.h>
#include <string>

class IconHelper {
public:
    // 图标 Unicode 常量
    static const std::string ICON_OPEN_FILE;
    static const std::string ICON_EDIT;
    static const std::string ICON_RESET;
    static const std::string ICON_SETTINGS;
    static const std::string ICON_EXIT;
    static const std::string ICON_WRONG_WORDS;
    static const std::string ICON_CLEAR;
    static const std::string ICON_EXPORT;
    static const std::string ICON_DEBUG;
    static const std::string ICON_USAGE;
    static const std::string ICON_ABOUT;
    static const std::string ICON_SUBMIT;
    static const std::string ICON_NEXT;
    static const std::string ICON_SHOW_ANSWER;
    static const std::string ICON_RESTART;
    static const std::string ICON_REVIEW;
    static const std::string ICON_DARK_MODE;
    static const std::string ICON_CORRECT;
    static const std::string ICON_WRONG;
    static const std::string ICON_CHECK;
    static const std::string ICON_CLOSE;
    static const std::string ICON_MINIMIZE;
    static const std::string ICON_SEARCH_ALERT;
    
    // 创建图标标签
    static Gtk::Label* create_icon(const std::string& unicode, int size = 14);
    
    // 创建带图标和文字的按钮
    static Gtk::Button* create_icon_button(const std::string& unicode, 
                                           const std::string& text, 
                                           int icon_size = 14);
    
    // 初始化字体
    static void init_fonts();
};

#endif
