#ifndef ICONHELPER_H
#define ICONHELPER_H

#include <gtkmm.h>
#include <string>
#include <unordered_map>

class IconHelper {
public:
    static void init_icons();
    static Gtk::Image* create_icon(const std::string& name, int size = 20, const std::string& color = "#333");
    static Gtk::Button* create_icon_button(const std::string& name, const std::string& text, int icon_size = 20);

    // 全局模式设置
    static void set_dark_mode(bool dark);

    static const std::string FOLDER_OPEN;
    static const std::string EDIT;
    static const std::string REFRESH_CW;
    static const std::string SETTINGS;
    static const std::string POWER;
    static const std::string X_CIRCLE;
    static const std::string TRASH_2;
    static const std::string FILE_EXPORT;
    static const std::string BUG;
    static const std::string INFO;
    static const std::string CHECK;
    static const std::string ARROW_RIGHT;
    static const std::string EYE;
    static const std::string SEARCH;
    static const std::string MOON;
    static const std::string SUN;
    static const std::string CHECK_CIRCLE;
    static const std::string TIMES;
    static const std::string MINIMIZE_2;
    static const std::string ALERT_TRIANGLE;

private:
    static std::unordered_map<std::string, std::string> svg_map;
    static bool dark_mode;
    static std::string wrap_svg(const std::string& path, int size, const std::string& color);
};

#endif
