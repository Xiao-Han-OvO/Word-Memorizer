#include "GlassTheme.h"
#include <fstream>
#include <sstream>
#include <iostream>

Glib::RefPtr<Gtk::CssProvider> GlassTheme::css_provider;
Glib::RefPtr<Gdk::Screen> GlassTheme::screen;
std::string GlassTheme::base_css;
std::string GlassTheme::dark_css;

void GlassTheme::init() {
    // 加载 CSS 文件（相对于可执行文件路径，这里假设从 resources/ 加载）
    // 实际运行时需根据安装路径调整，这里先提供默认实现
    base_css = load_file("resources/style.css");
    dark_css = load_file("resources/style-dark.css");
    
    if (base_css.empty()) {
        // 内置最小备用样式
        base_css = ".light-glass { background: #f5f7fa; }";
    }
    if (dark_css.empty()) {
        dark_css = ".dark-glass { background: #1a202c; }";
    }
    
    css_provider = Gtk::CssProvider::create();
}

void GlassTheme::apply_theme(bool dark, const std::string& accent_color) {
    if (!css_provider) init();
    
    // 移除旧提供者
    if (screen) {
        Gtk::StyleContext::remove_provider_for_screen(screen, css_provider);
    }
    
    // 组合 CSS
    std::string combined = replace_accent(base_css, accent_color);
    if (dark) {
        combined += "\n" + replace_accent(dark_css, accent_color);
    }
    
    try {
        css_provider->load_from_data(combined);
    } catch (const Gtk::CssProviderError& ex) {
        std::cerr << "CSS Error: " << ex.what() << std::endl;
    }
    
    if (screen) {
        Gtk::StyleContext::add_provider_for_screen(screen, css_provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

void GlassTheme::set_screen(const Glib::RefPtr<Gdk::Screen>& scr) {
    screen = scr;
}

void GlassTheme::add_provider(const std::string& css_data) {
    auto provider = Gtk::CssProvider::create();
    try {
        provider->load_from_data(css_data);
    } catch (...) {}
    if (screen) {
        Gtk::StyleContext::add_provider_for_screen(screen, provider,
            GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
}

std::string GlassTheme::load_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) return "";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

std::string GlassTheme::replace_accent(const std::string& css, const std::string& accent) {
    std::string result = css;
    size_t pos = 0;
    while ((pos = result.find("@accent_color", pos)) != std::string::npos) {
        result.replace(pos, 13, accent);
        pos += accent.length();
    }
    return result;
}
