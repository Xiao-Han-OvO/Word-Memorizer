#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <gtkmm.h>

class AboutDialog : public Gtk::Dialog {
private:
    Gtk::Box mainBox;
    Gtk::Label titleLabel;
    Gtk::Label versionLabel;
    Gtk::Label descriptionLabel;
    Gtk::Label authorLabel;
    Gtk::Label licenseLabel;
    Gtk::Button closeButton;

public:
    AboutDialog(Gtk::Window& parent);
    virtual ~AboutDialog();
};

#endif // ABOUTDIALOG_H