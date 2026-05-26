#ifndef GLASSTHEME_H
#define GLASSTHEME_H

#include <gtkmm.h>
#include <string>

class GlassTheme {
public:
    static void init();
    static void apply_theme(bool dark, const std::string& accent_color);
    static void set_screen(const Glib::RefPtr<Gdk::Screen>& screen);
    static void add_provider(const std::string& css_data);
    
private:
    static Glib::RefPtr<Gtk::CssProvider> css_provider;
    static Glib::RefPtr<Gdk::Screen> screen;
    static std::string base_css;
    static std::string dark_css;
    
    static std::string load_file(const std::string& path);
    static std::string replace_accent(const std::string& css, const std::string& accent);
};

#endif
