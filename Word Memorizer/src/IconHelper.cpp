#include "IconHelper.h"
#include <gdkmm/pixbufloader.h>
#include <iostream>
#include <sstream>

// 常量定义
const std::string IconHelper::FOLDER_OPEN    = "folder-open";
const std::string IconHelper::EDIT           = "edit";
const std::string IconHelper::REFRESH_CW     = "refresh-cw";
const std::string IconHelper::SETTINGS       = "settings";
const std::string IconHelper::POWER          = "power";
const std::string IconHelper::X_CIRCLE       = "x-circle";
const std::string IconHelper::TRASH_2        = "trash-2";
const std::string IconHelper::FILE_EXPORT    = "file-export";
const std::string IconHelper::BUG            = "bug";
const std::string IconHelper::INFO           = "info";
const std::string IconHelper::CHECK          = "check";
const std::string IconHelper::ARROW_RIGHT    = "arrow-right";
const std::string IconHelper::EYE            = "eye";
const std::string IconHelper::SEARCH         = "search";
const std::string IconHelper::MOON           = "moon";
const std::string IconHelper::SUN            = "sun";
const std::string IconHelper::CHECK_CIRCLE   = "check-circle";
const std::string IconHelper::TIMES          = "times";
const std::string IconHelper::MINIMIZE_2     = "minimize-2";
const std::string IconHelper::ALERT_TRIANGLE = "alert-triangle";

std::unordered_map<std::string, std::string> IconHelper::svg_map;
bool IconHelper::dark_mode = false;

void IconHelper::set_dark_mode(bool dark) { dark_mode = dark; }

void IconHelper::init_icons() {
    svg_map["folder-open"] =
        "<path d=\"M22 19a2 2 0 0 1-2 2H4a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h5l2 3h9a2 2 0 0 1 2 2z\"/>";
    svg_map["edit"] =
        "<path d=\"M17 3a2.83 2.83 0 1 1 4 4L7.5 20.5 2 22l1.5-5.5Z\"/><path d=\"M15 5l4 4\"/>";
    svg_map["refresh-cw"] =
        "<path d=\"M3 12a9 9 0 0 1 9-9 9.75 9.75 0 0 1 6.74 2.74L21 8\"/><path d=\"M21 3v5h-5\"/><path d=\"M21 12a9 9 0 0 1-9 9 9.75 9.75 0 0 1-6.74-2.74L3 16\"/><path d=\"M3 21v-5h5\"/>";
    svg_map["settings"] =
        "<path d=\"M12.22 2h-.44a2 2 0 0 0-2 2v.18a2 2 0 0 1-1 1.73l-.43.25a2 2 0 0 1-2 0l-.15-.08a2 2 0 0 0-2.73.73l-.22.38a2 2 0 0 0 .73 2.73l.15.1a2 2 0 0 1 1 1.72v.51a2 2 0 0 1-1 1.74l-.15.09a2 2 0 0 0-.73 2.73l.22.38a2 2 0 0 0 2.73.73l.15-.08a2 2 0 0 1 2 0l.43.25a2 2 0 0 1 1 1.73V20a2 2 0 0 0 2 2h.44a2 2 0 0 0 2-2v-.18a2 2 0 0 1 1-1.73l.43-.25a2 2 0 0 1 2 0l.15.08a2 2 0 0 0 2.73-.73l.22-.38a2 2 0 0 0-.73-2.73l-.15-.1a2 2 0 0 1-1-1.72v-.51a2 2 0 0 1 1-1.74l.15-.09a2 2 0 0 0 .73-2.73l-.22-.38a2 2 0 0 0-2.73-.73l-.15.08a2 2 0 0 1-2 0l-.43-.25a2 2 0 0 1-1-1.73V4a2 2 0 0 0-2-2z\"/><circle cx=\"12\" cy=\"12\" r=\"3\"/>";
    svg_map["power"] =
        "<path d=\"M12 2v10\"/><path d=\"M18.4 6.6a9 9 0 1 1-12.8 0\"/>";
    svg_map["x-circle"] =
        "<circle cx=\"12\" cy=\"12\" r=\"10\"/><path d=\"M15 9l-6 6\"/><path d=\"M9 9l6 6\"/>";
    svg_map["trash-2"] =
        "<path d=\"M3 6h18\"/><path d=\"M19 6v14c0 1-1 2-2 2H7c-1 0-2-1-2-2V6\"/><path d=\"M8 6V4c0-1 1-2 2-2h4c1 0 2 1 2 2v2\"/><line x1=\"10\" y1=\"11\" x2=\"10\" y2=\"17\"/><line x1=\"14\" y1=\"11\" x2=\"14\" y2=\"17\"/>";
    svg_map["file-export"] =
        "<path d=\"M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z\"/><polyline points=\"14 2 14 8 20 8\"/><path d=\"M12 18v-6\"/><path d=\"M9 15l3-3 3 3\"/>";
    svg_map["bug"] =
        "<path d=\"M8 2l1.88 1.88\"/><path d=\"M14.12 3.88L16 2\"/><path d=\"M9 7.13v-1a3 3 0 1 1 6 0v1\"/><path d=\"M12 20a7 7 0 0 1-7-7V8a2 2 0 0 1 2-2h10a2 2 0 0 1 2 2v5a7 7 0 0 1-7 7z\"/><path d=\"M5 10H3\"/><path d=\"M21 10h-2\"/><path d=\"M12 20v2\"/><path d=\"M8 14h.01\"/><path d=\"M16 14h.01\"/>";
    svg_map["info"] =
        "<circle cx=\"12\" cy=\"12\" r=\"10\"/><path d=\"M12 16v-4\"/><path d=\"M12 8h.01\"/>";
    svg_map["check"] =
        "<polyline points=\"20 6 9 17 4 12\"/>";
    svg_map["arrow-right"] =
        "<path d=\"M5 12h14\"/><path d=\"M12 5l7 7-7 7\"/>";
    svg_map["eye"] =
        "<path d=\"M1 12s4-8 11-8 11 8 11 8-4 8-11 8-11-8-11-8z\"/><circle cx=\"12\" cy=\"12\" r=\"3\"/>";
    svg_map["search"] =
        "<circle cx=\"11\" cy=\"11\" r=\"8\"/><path d=\"M21 21l-4.35-4.35\"/>";
    svg_map["moon"] =
        "<path d=\"M21 12.79A9 9 0 1 1 11.21 3 7 7 0 0 0 21 12.79z\"/>";
    svg_map["sun"] =
        "<circle cx=\"12\" cy=\"12\" r=\"4\"/><path d=\"M12 2v2\"/><path d=\"M12 20v2\"/><path d=\"M4.93 4.93l1.41 1.41\"/><path d=\"M17.66 17.66l1.41 1.41\"/><path d=\"M2 12h2\"/><path d=\"M20 12h2\"/><path d=\"M6.34 17.66l-1.41 1.41\"/><path d=\"M19.07 4.93l-1.41 1.41\"/>";
    svg_map["check-circle"] =
        "<circle cx=\"12\" cy=\"12\" r=\"10\"/><path d=\"M16 10l-4 4-2-2\"/>";
    svg_map["times"] =
        "<path d=\"M18 6 6 18\"/><path d=\"M6 6l12 12\"/>";
    svg_map["minimize-2"] =
        "<polyline points=\"4 14 10 14 10 20\"/><polyline points=\"20 10 14 10 14 4\"/><line x1=\"14\" y1=\"10\" x2=\"21\" y2=\"3\"/><line x1=\"3\" y1=\"21\" x2=\"10\" y2=\"14\"/>";
    svg_map["alert-triangle"] =
        "<path d=\"M10.29 3.86L1.82 18a2 2 0 0 0 1.71 3h16.94a2 2 0 0 0 1.71-3L13.71 3.86a2 2 0 0 0-3.42 0z\"/><line x1=\"12\" y1=\"9\" x2=\"12\" y2=\"13\"/><line x1=\"12\" y1=\"17\" x2=\"12.01\" y2=\"17\"/>";
}

static std::string build_svg(const std::string& path, int size, const std::string& color) {
    std::ostringstream ss;
    ss << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
       << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << size
       << "\" height=\"" << size << "\" viewBox=\"0 0 24 24\""
       << " fill=\"none\" stroke=\"" << color << "\" stroke-width=\"2\""
       << " stroke-linecap=\"round\" stroke-linejoin=\"round\">"
       << path << "</svg>";
    return ss.str();
}

Gtk::Image* IconHelper::create_icon(const std::string& name, int size, const std::string& color) {
    auto it = svg_map.find(name);
    if (it == svg_map.end()) {
        std::cerr << "Icon not found: " << name << std::endl;
        return Gtk::manage(new Gtk::Image());
    }

    std::string actual_color = color;
    if (dark_mode && actual_color == "#333") {
        actual_color = "#e0e0e0";
    }

    std::string raw_svg = build_svg(it->second, size, actual_color);
    auto loader = Gdk::PixbufLoader::create();
    try {
        loader->write(reinterpret_cast<const guint8*>(raw_svg.c_str()), raw_svg.size());
        loader->close();
        auto pixbuf = loader->get_pixbuf();
        if (pixbuf) {
            return Gtk::manage(new Gtk::Image(pixbuf));
        }
    } catch (...) {
        std::cerr << "Failed to render SVG icon: " << name << std::endl;
    }
    return Gtk::manage(new Gtk::Image());
}

Gtk::Button* IconHelper::create_icon_button(const std::string& name, const std::string& text, int icon_size) {
    auto button = Gtk::manage(new Gtk::Button());
    auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 6));
    box->pack_start(*create_icon(name, icon_size), Gtk::PACK_SHRINK);
    if (!text.empty()) {
        box->pack_start(*Gtk::manage(new Gtk::Label(text)), Gtk::PACK_SHRINK);
    }
    button->add(*box);
    button->show_all();
    return button;
}
