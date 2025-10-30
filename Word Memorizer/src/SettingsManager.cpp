#include "SettingsManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib> // for getenv
#include <algorithm> // for std::transform
#include <gtkmm.h> // for GTK相关功能

SettingsManager::SettingsManager() {
    // 确定配置文件路径
    const char* homeDir = std::getenv("HOME");
    if (homeDir) {
        configFile = std::string(homeDir) + "/.word_memorizer";
    } else {
        configFile = ".word_memorizer"; // 退回到当前目录
    }
    
    // 加载设置
    loadSettings();
    
    // 初始化深色模式（如果设置文件中没有，则检测系统主题）
    std::string darkModeSetting = getSetting("dark_mode", "");
    if (darkModeSetting.empty()) {
        darkModeEnabled = detectSystemDarkMode();
    } else {
        darkModeEnabled = (darkModeSetting == "true");
    }
    
    // 初始化颜色主题
    colorTheme = getSetting("color_theme", "blue"); // 默认为蓝色主题
}

bool SettingsManager::isDarkModeEnabled() const {
    return darkModeEnabled;
}

void SettingsManager::setDarkModeEnabled(bool enabled) {
    darkModeEnabled = enabled;
    setSetting("dark_mode", enabled ? "true" : "false");
}

std::string SettingsManager::getColorTheme() const {
    return colorTheme;
}

void SettingsManager::setColorTheme(const std::string& theme) {
    colorTheme = theme;
    setSetting("color_theme", theme);
}

// 在 SettingsManager.cpp 中修改 detectSystemDarkMode 方法
bool SettingsManager::detectSystemDarkMode() {
    // 尝试多种方法检测系统主题
    const char* gtk_theme = std::getenv("GTK_THEME");
    if (gtk_theme) {
        std::string theme = gtk_theme;
        // 手动转换为小写，避免依赖algorithm
        for (char &c : theme) {
            c = std::tolower(c);
        }
        if (theme.find("dark") != std::string::npos) {
            return true;
        }
    }
    
    // 检查GTK设置 - 使用Gtk::Settings
    auto settings = Gtk::Settings::get_default();
    if (settings) {
        Glib::ustring theme_name;
        settings->get_property("gtk-theme-name", theme_name);
        
        // 手动转换为小写
        std::string theme_lower = theme_name;
        for (char &c : theme_lower) {
            c = std::tolower(c);
        }
        if (theme_lower.find("dark") != std::string::npos) {
            return true;
        }
    }
    
    return false;
}

void SettingsManager::loadSettings() {
    settings.clear();
    
    std::ifstream file(configFile);
    if (!file.is_open()) {
        // 如果文件不存在，使用默认设置
        setPosColor(DEFAULT_POS_COLOR);
        setCorrectColor(DEFAULT_CORRECT_COLOR);
        setErrorColor(DEFAULT_ERROR_COLOR);
        setAnswerColor(DEFAULT_ANSWER_COLOR);
        setColorTheme("blue"); // 默认蓝色主题
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            settings[key] = value;
        }
    }
    file.close();
}

void SettingsManager::saveSettings() {
    std::ofstream file(configFile);
    if (!file.is_open()) {
        std::cerr << "无法保存设置到文件: " << configFile << std::endl;
        return;
    }
    
    for (const auto& pair : settings) {
        file << pair.first << "=" << pair.second << std::endl;
    }
    file.close();
}

std::string SettingsManager::getSetting(const std::string& key, const std::string& defaultValue) {
    auto it = settings.find(key);
    if (it != settings.end()) {
        return it->second;
    }
    return defaultValue;
}

void SettingsManager::setSetting(const std::string& key, const std::string& value) {
    settings[key] = value;
    saveSettings();
}

// 颜色获取方法
std::string SettingsManager::getPosColor() {
    return getSetting("pos_color", DEFAULT_POS_COLOR);
}

std::string SettingsManager::getCorrectColor() {
    return getSetting("correct_color", DEFAULT_CORRECT_COLOR);
}

std::string SettingsManager::getErrorColor() {
    return getSetting("error_color", DEFAULT_ERROR_COLOR);
}

std::string SettingsManager::getAnswerColor() {
    return getSetting("answer_color", DEFAULT_ANSWER_COLOR);
}

void SettingsManager::setPosColor(const std::string& color) {
    setSetting("pos_color", color);
}

void SettingsManager::setCorrectColor(const std::string& color) {
    setSetting("correct_color", color);
}

void SettingsManager::setErrorColor(const std::string& color) {
    setSetting("error_color", color);
}

void SettingsManager::setAnswerColor(const std::string& color) {
    setSetting("answer_color", color);
}

// 主题颜色获取方法
std::string SettingsManager::getPrimaryColor() const {
    // 根据颜色主题返回对应的主色调
    if (colorTheme == "red") return "#e74c3c";
    else if (colorTheme == "orange") return "#e67e22";
    else if (colorTheme == "green") return "#2ecc71";
    else if (colorTheme == "cyan") return "#1abc9c";
    else if (colorTheme == "yellow") return "#f1c40f";
    else if (colorTheme == "brown") return "#a0522d";
    else if (colorTheme == "mono") return "#7f8c8d";
    else if (colorTheme == "purple") return "#9b59b6";
    else if (colorTheme == "pink") return "#e91e63";
    else return "#3498db"; // 默认蓝色
}

std::string SettingsManager::getSecondaryColor() const {
    // 根据颜色主题返回对应的辅助色调
    if (colorTheme == "red") return "#c0392b";
    else if (colorTheme == "orange") return "#d35400";
    else if (colorTheme == "green") return "#27ae60";
    else if (colorTheme == "cyan") return "#16a085";
    else if (colorTheme == "yellow") return "#f39c12";
    else if (colorTheme == "brown") return "#8b4513";
    else if (colorTheme == "mono") return "#34495e";
    else if (colorTheme == "purple") return "#8e44ad";
    else if (colorTheme == "pink") return "#ad1457";
    else return "#2980b9"; // 默认蓝色
}

std::string SettingsManager::getAccentColor() const {
    // 根据颜色主题返回对应的强调色调
    if (colorTheme == "red") return "#ff6b6b";
    else if (colorTheme == "orange") return "#ffa726";
    else if (colorTheme == "green") return "#4cd964";
    else if (colorTheme == "cyan") return "#4ecdc4";
    else if (colorTheme == "yellow") return "#ffeaa7";
    else if (colorTheme == "brown") return "#d2691e";
    else if (colorTheme == "mono") return "#95a5a6";
    else if (colorTheme == "purple") return "#a29bfe";
    else if (colorTheme == "pink") return "#fd79a8";
    else return "#74b9ff"; // 默认蓝色
}

std::string SettingsManager::getBackgroundColor() const {
    return darkModeEnabled ? "#1a202c" : "#ffffff";
}

std::string SettingsManager::getTextColor() const {
    return darkModeEnabled ? "#f7fafc" : "#1a202c";
}

std::string SettingsManager::getCardBackgroundColor() const {
    return darkModeEnabled ? "#2d3748" : "#ffffff";
}

std::string SettingsManager::getBorderColor() const {
    return darkModeEnabled ? "#4a5568" : "#e2e8f0";
}