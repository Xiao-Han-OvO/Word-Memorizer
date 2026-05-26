#include "MainWindow_ubuntu20.h"
#include <gtkmm/application.h>

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "com.example.wordmemorizer.ubuntu20");
    
    MainWindow window;
    
    return app->run(window);
}