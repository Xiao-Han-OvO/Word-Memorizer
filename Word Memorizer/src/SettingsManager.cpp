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
    return getThemeBackgroundColor();
}

std::string SettingsManager::getTextColor() const {
    return darkModeEnabled ? "#f7fafc" : "#1a202c";
}

std::string SettingsManager::getCardBackgroundColor() const {
    return getThemeCardBackgroundColor();
}

std::string SettingsManager::getBorderColor() const {
    return getThemeBorderColor();
}

// 在 SettingsManager.cpp 中添加新的方法实现

std::string SettingsManager::getThemeBackgroundColor() const {
    // 根据颜色主题和明暗模式返回对应的背景色
    if (darkModeEnabled) {
        if (colorTheme == "red") return "#161111";
        else if (colorTheme == "orange") return "#1a140f";
        else if (colorTheme == "green") return "#0f160f";
        else if (colorTheme == "cyan") return "#0f1616";
        else if (colorTheme == "yellow") return "#16160f";
        else if (colorTheme == "brown") return "#161310";
        else if (colorTheme == "mono") return "#0a0a0a";
        else if (colorTheme == "purple") return "#161016";
        else if (colorTheme == "pink") return "#160f13";
        else return "#0f141a"; // 默认蓝色深色背景
    } else {
        if (colorTheme == "red") return "#fae6e6";
        else if (colorTheme == "orange") return "#fff4e6";
        else if (colorTheme == "green") return "#e6f7e6";
        else if (colorTheme == "cyan") return "#e6f7f7";
        else if (colorTheme == "yellow") return "#fffde6";
        else if (colorTheme == "brown") return "#f7f3e6";
        else if (colorTheme == "mono") return "#f5f5f5";
        else if (colorTheme == "purple") return "#f3e6f7";
        else if (colorTheme == "pink") return "#fce6f5";
        else return "#f0f8ff"; // 默认蓝色浅色背景
    }
}

std::string SettingsManager::getThemeCardBackgroundColor() const {
    // 卡片背景色比主背景色稍深/稍浅
    if (darkModeEnabled) {
        if (colorTheme == "red") return "#1e1919";
        else if (colorTheme == "orange") return "#221c15";
        else if (colorTheme == "green") return "#151e15";
        else if (colorTheme == "cyan") return "#151e1e";
        else if (colorTheme == "yellow") return "#1e1e15";
        else if (colorTheme == "brown") return "#1e1b16";
        else if (colorTheme == "mono") return "#121212";
        else if (colorTheme == "purple") return "#1e151e";
        else if (colorTheme == "pink") return "#1e1519";
        else return "#151e25"; // 默认蓝色深色卡片背景
    } else {
        if (colorTheme == "red") return "#ffffff";
        else if (colorTheme == "orange") return "#ffffff";
        else if (colorTheme == "green") return "#ffffff";
        else if (colorTheme == "cyan") return "#ffffff";
        else if (colorTheme == "yellow") return "#ffffff";
        else if (colorTheme == "brown") return "#ffffff";
        else if (colorTheme == "mono") return "#ffffff";
        else if (colorTheme == "purple") return "#ffffff";
        else if (colorTheme == "pink") return "#ffffff";
        else return "#ffffff"; // 默认浅色卡片背景保持白色
    }
}

std::string SettingsManager::getThemeBorderColor() const {
    // 边框颜色
    if (darkModeEnabled) {
        if (colorTheme == "red") return "#3a2e2e";
        else if (colorTheme == "orange") return "#3a3225";
        else if (colorTheme == "green") return "#253a25";
        else if (colorTheme == "cyan") return "#253a3a";
        else if (colorTheme == "yellow") return "#3a3a25";
        else if (colorTheme == "brown") return "#3a3525";
        else if (colorTheme == "mono") return "#2a2a2a";
        else if (colorTheme == "purple") return "#3a253a";
        else if (colorTheme == "pink") return "#3a2532";
        else return "#25303a"; // 默认蓝色深色边框
    } else {
        if (colorTheme == "red") return "#e6d1d1";
        else if (colorTheme == "orange") return "#e6d9c6";
        else if (colorTheme == "green") return "#c6e6c6";
        else if (colorTheme == "cyan") return "#c6e6e6";
        else if (colorTheme == "yellow") return "#e6e6c6";
        else if (colorTheme == "brown") return "#e6e0c6";
        else if (colorTheme == "mono") return "#d5d5d5";
        else if (colorTheme == "purple") return "#e0c6e6";
        else if (colorTheme == "pink") return "#e6c6da";
        else return "#c6d9e6"; // 默认蓝色浅色边框
    }
}