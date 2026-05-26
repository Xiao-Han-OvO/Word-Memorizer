#include "MainWindow.h"
#include "GlassTheme.h"
#include "IconHelper.h"
#include <gtkmm/application.h>
#include <iostream>

int main(int argc, char *argv[]) {
    // 初始化字体
    IconHelper::init_fonts();
    
    // 初始化主题
    GlassTheme::init();
    GlassTheme::apply_theme(false, "#3498db"); // 默认浅色蓝色主题
    
    auto app = Gtk::Application::create(argc, argv, "com.example.wordmemorizer");
    MainWindow window;
    
    // 设置屏幕以全局应用样式
    auto screen = window.get_screen();
    GlassTheme::set_screen(screen);
    GlassTheme::apply_theme(false, "#3498db");
    
    return app->run(window);
}
