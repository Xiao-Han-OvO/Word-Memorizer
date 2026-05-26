#include "MainWindow.h"
#include <fstream>
#include <iostream>

// 构造函数：初始化单词编辑器对话框
WordEditorDialog::WordEditorDialog(Gtk::Window& parent)
	: Dialog("单词编辑器", parent, true),
	  mainBox(Gtk::ORIENTATION_VERTICAL),
	  inputGrid(),
	  buttonBox(Gtk::ORIENTATION_HORIZONTAL) {
	
	set_default_size(500, 300);
	set_border_width(20);
	
	// 标题
	titleLabel.set_label("编辑单词");
	titleLabel.override_font(Pango::FontDescription("Sans Bold 16"));
	mainBox.pack_start(titleLabel, Gtk::PACK_SHRINK);
	
	// 输入网格
	inputGrid.set_row_spacing(10);
	inputGrid.set_column_spacing(15);
	inputGrid.set_border_width(20);
	
	// 单词输入
	wordLabel.set_label("单词:");
	wordLabel.set_halign(Gtk::ALIGN_START);
	wordEntry.set_placeholder_text("输入英文单词");
	wordEntry.set_size_request(200, -1);
	
	// 词性输入
	posLabel.set_label("词性:");
	posLabel.set_halign(Gtk::ALIGN_START);
	posEntry.set_placeholder_text("例如: n., v., adj.");
	posEntry.set_size_request(200, -1);
	
	// 释义输入
	meaningLabel.set_label("释义:");
	meaningLabel.set_halign(Gtk::ALIGN_START);
	meaningEntry.set_placeholder_text("输入中文释义");
	meaningEntry.set_size_request(200, -1);
	
	// 例句输入
	exampleLabel.set_label("例句:");
	exampleLabel.set_halign(Gtk::ALIGN_START);
	exampleEntry.set_placeholder_text("输入包含该单词的例句");
	exampleEntry.set_size_request(200, -1);
	
	// 布局网格
	inputGrid.attach(wordLabel, 0, 0, 1, 1);
	inputGrid.attach(wordEntry, 1, 0, 1, 1);
	inputGrid.attach(posLabel, 0, 1, 1, 1);
	inputGrid.attach(posEntry, 1, 1, 1, 1);
	inputGrid.attach(meaningLabel, 0, 2, 1, 1);
	inputGrid.attach(meaningEntry, 1, 2, 1, 1);
	inputGrid.attach(exampleLabel, 0, 3, 1, 1);
	inputGrid.attach(exampleEntry, 1, 3, 1, 1);
	
	mainBox.pack_start(inputGrid, Gtk::PACK_SHRINK);
	
	// 按钮
	buttonBox.set_spacing(10);
	buttonBox.set_halign(Gtk::ALIGN_CENTER);
	
	saveButton.set_label("保存到文件");
	loadButton.set_label("从文件导入");
	cancelButton.set_label("关闭");
	
	buttonBox.pack_start(saveButton, Gtk::PACK_SHRINK);
	buttonBox.pack_start(loadButton, Gtk::PACK_SHRINK);
	buttonBox.pack_start(cancelButton, Gtk::PACK_SHRINK);
	
	mainBox.pack_start(buttonBox, Gtk::PACK_SHRINK);
	
	get_content_area()->pack_start(mainBox);
	
	// 连接信号
	saveButton.signal_clicked().connect(sigc::mem_fun(*this, &WordEditorDialog::on_save_clicked));
	loadButton.signal_clicked().connect(sigc::mem_fun(*this, &WordEditorDialog::on_load_clicked));
	cancelButton.signal_clicked().connect(sigc::mem_fun(*this, &WordEditorDialog::on_cancel_clicked));
	
	show_all_children();
}

// 析构函数
WordEditorDialog::~WordEditorDialog() {}

// 获取单词
std::string WordEditorDialog::getWord() const {
	return wordEntry.get_text();
}

// 获取词性
std::string WordEditorDialog::getPos() const {
	return posEntry.get_text();
}

// 获取释义
std::string WordEditorDialog::getMeaning() const {
	return meaningEntry.get_text();
}

// 获取例句
std::string WordEditorDialog::getExample() const {
	return exampleEntry.get_text();
}

// 清空输入字段
void WordEditorDialog::clearFields() {
	wordEntry.set_text("");
	posEntry.set_text("");
	meaningEntry.set_text("");
	exampleEntry.set_text("");
}

// 保存按钮点击事件
void WordEditorDialog::on_save_clicked() {
	std::string word = getWord();
	std::string pos = getPos();
	std::string meaning = getMeaning();
	std::string example = getExample();
	
	// 验证输入
	if (word.empty() || pos.empty() || meaning.empty()) {
		Gtk::MessageDialog errorDialog(*this, "请填写单词、词性和释义", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
		errorDialog.run();
		return;
	}
	
	// 选择保存文件
	Gtk::FileChooserDialog dialog("保存单词到文件", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	
	dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
	dialog.add_button("保存", Gtk::RESPONSE_OK);
	
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("文本文件");
	filter_text->add_pattern("*.txt");
	dialog.add_filter(filter_text);
	
	// 设置默认文件名
	dialog.set_current_name("my_words.txt");
	
	int result = dialog.run();
	
	if (result == Gtk::RESPONSE_OK) {
		std::string filename = dialog.get_filename();
		
		// 确保文件扩展名
		if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".txt") {
			filename += ".txt";
		}
		
		// 保存单词到文件
		std::ofstream file;
		file.open(filename, std::ios_base::app); // 追加模式
		
		if (file.is_open()) {
			file << word << "|" << pos << "|" << meaning << "|" << example << std::endl;
			file.close();
			
			Gtk::MessageDialog successDialog(*this, "单词已成功保存到文件", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
			successDialog.run();
			
			clearFields();
		} else {
			Gtk::MessageDialog errorDialog(*this, "无法保存文件", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
			errorDialog.run();
		}
	}
}

// 加载按钮点击事件
void WordEditorDialog::on_load_clicked() {
	// 选择要导入的文件
	Gtk::FileChooserDialog dialog("导入单词文件", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	
	dialog.add_button("取消", Gtk::RESPONSE_CANCEL);
	dialog.add_button("打开", Gtk::RESPONSE_OK);
	
	auto filter_text = Gtk::FileFilter::create();
	filter_text->set_name("文本文件");
	filter_text->add_pattern("*.txt");
	dialog.add_filter(filter_text);
	
	int result = dialog.run();
	
	if (result == Gtk::RESPONSE_OK) {
		std::string filename = dialog.get_filename();
		
		// 清空当前输入字段
		clearFields();
		
		// 读取文件内容并显示在对话框中供编辑
		std::ifstream file(filename);
		if (file.is_open()) {
			std::string line;
			bool firstLine = true;
			
			while (std::getline(file, line)) {
				if (line.empty()) continue;
				
				std::stringstream ss(line);
				std::string word, pos, meaning, example;
				
				if (std::getline(ss, word, '|') && 
					std::getline(ss, pos, '|') &&
					std::getline(ss, meaning, '|')) {
					
					std::getline(ss, example);
					
					// 如果是第一行，填充到编辑框中
					if (firstLine) {
						wordEntry.set_text(word);
						posEntry.set_text(pos);
						meaningEntry.set_text(meaning);
						exampleEntry.set_text(example);
						firstLine = false;
					}
				}
			}
			
			file.close();
			
			if (firstLine) {
				Gtk::MessageDialog infoDialog(*this, "文件为空或格式不正确", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, true);
				infoDialog.run();
			}
		} else {
			Gtk::MessageDialog errorDialog(*this, "无法打开文件", false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_OK, true);
			errorDialog.run();
		}
	}
}

// 取消按钮点击事件
void WordEditorDialog::on_cancel_clicked() {
	response(Gtk::RESPONSE_CANCEL);
}