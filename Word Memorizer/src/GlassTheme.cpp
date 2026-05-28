#include "GlassTheme.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <linux/limits.h>
#include <vector>

Glib::RefPtr<Gtk::CssProvider> GlassTheme::css_provider;
Glib::RefPtr<Gdk::Screen> GlassTheme::screen;
std::string GlassTheme::base_css;
std::string GlassTheme::dark_css;

// 内置 CSS 内容（从项目 resources 中提取）
static const std::string BUILTIN_BASE_CSS = R"(
/* ========== 浅色 Liquid Glass 主题 ========== */
* {
  font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif;
}
.light-glass {
  background-color: rgba(245, 247, 250, 0.95);
  color: #1a202c;
}
.glass-card {
  background: rgba(255, 255, 255, 0.6);
  border: 1px solid rgba(255, 255, 255, 0.8);
  border-radius: 24px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.08);
  padding: 24px;
  margin: 20px;
  transition: all 0.2s ease;
}
.side-panel {
  background: rgba(255, 255, 255, 0.45);
  border: 1px solid rgba(255, 255, 255, 0.7);
  border-radius: 24px;
  margin: 16px 16px 16px 8px;
  padding: 24px 16px;
  box-shadow: -4px 0 20px rgba(0, 0, 0, 0.05);
}
.stat-card {
  background: rgba(255, 255, 255, 0.35);
  border-radius: 16px;
  padding: 12px;
  margin: 8px 0;
}
.stat-value {
  font-size: 24px;
  font-weight: bold;
  color: @accent_color;
}
.stat-label {
  font-size: 12px;
  color: #4a5568;
}
.glass-entry {
  background: rgba(255, 255, 255, 0.5);
  border: 1px solid rgba(0, 0, 0, 0.08);
  border-radius: 20px;
  padding: 10px 20px;
  font-size: 16px;
  transition: all 0.2s;
}
.glass-entry:focus {
  background: rgba(255, 255, 255, 0.8);
  border-color: @accent_color;
  box-shadow: 0 0 0 3px alpha(@accent_color, 0.2);
}
.glass-btn {
  background: @accent_color;
  color: white;
  border: none;
  border-radius: 20px;
  padding: 10px 24px;
  font-weight: bold;
  transition: all 0.2s;
}
.glass-btn:hover {
  background: shade(@accent_color, 0.9);
  box-shadow: 0 4px 12px alpha(@accent_color, 0.4);
}
.glass-btn-secondary {
  background: rgba(255, 255, 255, 0.7);
  color: @accent_color;
  border: 1px solid @accent_color;
  border-radius: 20px;
  padding: 10px 24px;
  transition: all 0.2s;
}
.glass-btn-secondary:hover {
  background: @accent_color;
  color: white;
}
.glass-progress trough {
  background: rgba(0, 0, 0, 0.05);
  border-radius: 12px;
  min-height: 12px;
}
.glass-progress progress {
  background: @accent_color;
  border-radius: 12px;
}
.custom-titlebar {
  background: rgba(255, 255, 255, 0.4);
  border-radius: 16px;
  margin: 8px;
}
.custom-titlebar-label {
  font-weight: bold;
  font-size: 14px;
  color: #2d3748;
}
menubar {
  background: rgba(255, 255, 255, 0.5);
  border-radius: 12px;
  margin: 4px 12px;
}
.feedback-label {
  font-size: 16px;
  font-weight: bold;
  margin: 10px 0;
}
.custom-window-control {
  border-radius: 8px;
  padding: 2px 8px;
  background: transparent;
}
.custom-window-control.close {
  color: #e53e3e;
}
.custom-window-control.close:hover {
  background: #e53e3e;
  color: white;
}
.placeholder-page {
  background: rgba(255, 255, 255, 0.4);
  border-radius: 24px;
  margin: 40px;
  padding: 40px;
}
.placeholder-icon {
  color: @accent_color;
  opacity: 0.6;
  margin-bottom: 16px;
}
.placeholder-text {
  color: #666;
  font-size: 14px;
}
.side-navbar {
  background: rgba(255, 255, 255, 0.3);
  border-right: 1px solid rgba(255, 255, 255, 0.5);
  padding: 12px 0;
}
.nav-button {
  background: transparent;
  border: none;
  border-radius: 12px;
  padding: 8px 4px;
  margin: 2px 8px;
  transition: all 0.2s;
}
.nav-button:hover {
  background: rgba(255, 255, 255, 0.5);
}
.nav-button-label {
  font-size: 10px;
  color: #555;
  margin-top: 2px;
}
.dialog-box {
  background: rgba(255, 255, 255, 0.7);
  border: 1px solid rgba(255, 255, 255, 0.9);
  border-radius: 20px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.1);
}
.icon-label {
  font-family: lucide;
}
)";

static const std::string BUILTIN_DARK_CSS = R"(
/* ========== 深色 Liquid Glass 主题 ========== */
.dark-glass {
  background-color: #1a1d23;
  color: #e0e0e0;
}
.dark-glass .glass-card {
  background: rgba(40, 44, 52, 0.75);
  border: 1px solid rgba(255, 255, 255, 0.08);
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.4);
}
.dark-glass .side-panel {
  background: rgba(30, 33, 40, 0.65);
  border-color: rgba(255, 255, 255, 0.08);
}
.dark-glass .stat-card {
  background: rgba(255, 255, 255, 0.06);
}
.dark-glass .stat-value {
  color: @accent_color;
}
.dark-glass .stat-label {
  color: #aaa;
}
.dark-glass .glass-entry {
  background: rgba(0, 0, 0, 0.3);
  border-color: rgba(255, 255, 255, 0.08);
  color: white;
}
.dark-glass .glass-entry:focus {
  background: rgba(0, 0, 0, 0.5);
  border-color: @accent_color;
  box-shadow: 0 0 0 3px alpha(@accent_color, 0.3);
}
.dark-glass .glass-btn {
  background: @accent_color;
  color: white;
}
.dark-glass .glass-btn-secondary {
  background: rgba(255, 255, 255, 0.1);
  color: @accent_color;
  border-color: @accent_color;
}
.dark-glass .glass-progress trough {
  background: rgba(255, 255, 255, 0.1);
}
.dark-glass .glass-progress progress {
  background: @accent_color;
}
.dark-glass .custom-titlebar {
  background: rgba(20, 22, 28, 0.7);
}
.dark-glass .custom-titlebar-label {
  color: #ccc;
}
.dark-glass menubar {
  background: rgba(40, 44, 52, 0.7);
}
.dark-glass .placeholder-page {
  background: rgba(40, 44, 52, 0.6);
  border-radius: 24px;
  margin: 40px;
}
.dark-glass .placeholder-icon {
  color: @accent_color;
  opacity: 0.7;
}
.dark-glass .placeholder-text {
  color: #888;
}
.dark-glass .custom-titlebar {
  background: rgba(50, 55, 65, 0.8);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 16px;
  margin: 8px;
}
.dark-glass .custom-titlebar-label {
  color: #ddd;
  font-weight: bold;
}
.dark-glass menubar {
  background: rgba(50, 55, 65, 0.7);
  border-bottom: 1px solid rgba(255, 255, 255, 0.1);
}
.dark-glass .side-navbar {
  background: rgba(30, 33, 40, 0.6);
  border-right: 1px solid rgba(255, 255, 255, 0.08);
}
.dark-glass .nav-button:hover {
  background: rgba(255, 255, 255, 0.1);
}
.dark-glass .nav-button-label {
  color: #aaa;
}
.dark-glass .dialog-box {
  background: rgba(40, 44, 52, 0.85);
  border: 1px solid rgba(255, 255, 255, 0.1);
  border-radius: 20px;
  box-shadow: 0 8px 32px rgba(0, 0, 0, 0.5);
}
.dark-glass .icon-label {
  font-family: lucide;
  color: inherit;
}
)";

std::string GlassTheme::load_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) return "";
    std::stringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

void GlassTheme::init() {
    // 尝试从可执行文件所在目录查找 resources
    char buf[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", buf, sizeof(buf)-1);
    std::string exe_dir;
    if (len != -1) {
        buf[len] = '\0';
        exe_dir = buf;
        size_t last_slash = exe_dir.rfind('/');
        if (last_slash != std::string::npos) {
            exe_dir = exe_dir.substr(0, last_slash);
        }
    } else {
        exe_dir = ".";
    }

    // 尝试几个可能的资源目录
    std::vector<std::string> candidates = {
        exe_dir + "/resources",
        exe_dir + "/../resources",
        "resources",
        "/usr/local/share/word_memorizer/resources"
    };

    bool loaded = false;
    for (const auto& dir : candidates) {
        std::string css_path = dir + "/style.css";
        std::string dark_path = dir + "/style-dark.css";
        base_css = load_file(css_path);
        dark_css = load_file(dark_path);
        if (!base_css.empty() && !dark_css.empty()) {
            std::cerr << "Loaded CSS from: " << dir << std::endl;
            loaded = true;
            break;
        }
    }

    if (!loaded) {
        // 使用内置 CSS
        base_css = BUILTIN_BASE_CSS;
        dark_css = BUILTIN_DARK_CSS;
        std::cerr << "Using built-in CSS" << std::endl;
    }

    css_provider = Gtk::CssProvider::create();
}

void GlassTheme::apply_theme(bool dark, const std::string& accent_color) {
    if (!css_provider) init();

    if (screen) {
        Gtk::StyleContext::remove_provider_for_screen(screen, css_provider);
    }

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

std::string GlassTheme::replace_accent(const std::string& css, const std::string& accent) {
    std::string result = css;
    size_t pos = 0;
    while ((pos = result.find("@accent_color", pos)) != std::string::npos) {
        result.replace(pos, 13, accent);
        pos += accent.length();
    }
    return result;
}
