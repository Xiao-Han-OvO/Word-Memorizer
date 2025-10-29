#include "SettingsManager.h"
#include <fstream>
#include <iostream>
#include <cstdlib> // for getenv

SettingsManager::SettingsManager() {
    // 确定配置文件路径
    const char* homeDir = std::getenv("HOME");
    if (homeDir) {
        configFile = std::string(homeDir) + "/.word_memorizer";
    } else {
        configFile = ".word_memorizer"; // 退回到当前目录
    }
    loadSettings();
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