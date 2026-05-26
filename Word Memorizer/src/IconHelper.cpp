#include "IconHelper.h"
#include <fontconfig/fontconfig.h>

// 图标 Unicode 定义（Font Awesome 6 Free Solid）
const std::string IconHelper::ICON_OPEN_FILE   = "\uf07c"; // folder-open
const std::string IconHelper::ICON_EDIT        = "\uf044"; // edit
const std::string IconHelper::ICON_RESET       = "\uf021"; // sync
const std::string IconHelper::ICON_SETTINGS    = "\uf013"; // cog
const std::string IconHelper::ICON_EXIT        = "\uf011"; // power-off
const std::string IconHelper::ICON_WRONG_WORDS = "\uf057"; // times-circle
const std::string IconHelper::ICON_CLEAR       = "\uf2ed"; // trash-alt
const std::string IconHelper::ICON_EXPORT      = "\uf56e"; // file-export
const std::string IconHelper::ICON_DEBUG       = "\uf188"; // bug
const std::string IconHelper::ICON_USAGE       = "\uf05a"; // info-circle
const std::string IconHelper::ICON_ABOUT       = "\uf05a"; // info-circle
const std::string IconHelper::ICON_SUBMIT      = "\uf00c"; // check
const std::string IconHelper::ICON_NEXT        = "\uf061"; // arrow-right
const std::string IconHelper::ICON_SHOW_ANSWER = "\uf06e"; // eye
const std::string IconHelper::ICON_RESTART     = "\uf021"; // sync
const std::string IconHelper::ICON_REVIEW      = "\uf002"; // search
const std::string IconHelper::ICON_DARK_MODE   = "\uf186"; // moon
const std::string IconHelper::ICON_CORRECT     = "\uf058"; // check-circle
const std::string IconHelper::ICON_WRONG       = "\uf057"; // times-circle
const std::string IconHelper::ICON_CHECK       = "\uf00c"; // check
const std::string IconHelper::ICON_CLOSE       = "\uf00d"; // times
const std::string IconHelper::ICON_MINIMIZE    = "\uf2d1"; // window-minimize

const std::string IconHelper::ICON_SEARCH_ALERT = "\ue814"; // alert-triangle
void IconHelper::init_fonts() {
    // 注册 Font Awesome 字体
    const std::string font_path = "resources/fonts/fa-solid-900.ttf";
    if (FcConfigAppFontAddFile(nullptr, 
        reinterpret_cast<const FcChar8*>(font_path.c_str()))) {
        g_message("Font Awesome loaded successfully");
    } else {
        g_warning("Failed to load Font Awesome from %s", font_path.c_str());
    }
}

Gtk::Label* IconHelper::create_icon(const std::string& unicode, int size) {
    auto label = Gtk::manage(new Gtk::Label(unicode));
    label->override_font(Pango::FontDescription(
        "Font Awesome 6 Free Solid " + std::to_string(size)));
    return label;
}

Gtk::Button* IconHelper::create_icon_button(const std::string& unicode, 
                                            const std::string& text, 
                                            int icon_size) {
    auto button = Gtk::manage(new Gtk::Button());
    auto box = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL, 6));
    box->pack_start(*create_icon(unicode, icon_size), Gtk::PACK_SHRINK);
    if (!text.empty()) {
        box->pack_start(*Gtk::manage(new Gtk::Label(text)), Gtk::PACK_SHRINK);
    }
    button->add(*box);
    button->show_all();
    return button;
}
