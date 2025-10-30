#ifndef USAGEDIALOG_H
#define USAGEDIALOG_H

#include <gtkmm.h>

class UsageDialog : public Gtk::Dialog {
private:
    Gtk::Box mainBox;
    Gtk::ScrolledWindow scrolledWindow;
    Gtk::Label contentLabel;
    Gtk::Button closeButton;

public:
    UsageDialog(Gtk::Window& parent);
    virtual ~UsageDialog();
};

#endif // USAGEDIALOG_H