#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <string>
#include <map>

class SettingsManager {
private:
    std::string configFile;
    std::map<std::string, std::string> settings;
    
    // 默认颜色
    const std::string DEFAULT_POS_COLOR = "blue";
    const std::string DEFAULT_CORRECT_COLOR = "green";
    const std::string DEFAULT_ERROR_COLOR = "red";
    const std::string DEFAULT_ANSWER_COLOR = "blue";
    
public:
    SettingsManager();
    void loadSettings();
    void saveSettings();
    std::string getSetting(const std::string& key, const std::string& defaultValue = "");
    void setSetting(const std::string& key, const std::string& value);
    
    // 颜色相关的便捷方法
    std::string getPosColor();
    std::string getCorrectColor();
    std::string getErrorColor();
    std::string getAnswerColor();
    void setPosColor(const std::string& color);
    void setCorrectColor(const std::string& color);
    void setErrorColor(const std::string& color);
    void setAnswerColor(const std::string& color);
};

#endif