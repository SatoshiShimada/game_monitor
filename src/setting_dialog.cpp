#include "setting_dialog.h"

SettingDialog::SettingDialog(QWidget *parent) : QDialog(parent)
{
	buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

	button = new QPushButton("Apply");
	font_size_label = new QLabel("font size of game state: ");
	font_size_slider = new QSlider(Qt::Horizontal);
	font_size_slider->setRange(1, 128);
	connect(font_size_slider, SIGNAL(valueChanged(int)), this, SLOT(fontSizeValueChanged(int)));
	display_size_label = new QLabel("display size: ");
	display_size_slider = new QSlider(Qt::Horizontal);
	display_size_slider->setRange(1, 300);
	connect(display_size_slider, SIGNAL(valueChanged(int)), this, SLOT(displaySizeValueChanged(int)));

	slider_layout = new QGridLayout;
	slider_layout->addWidget(font_size_label, 1, 1);
	slider_layout->addWidget(font_size_slider, 1, 2);
	slider_layout->addWidget(display_size_label, 2, 1);
	slider_layout->addWidget(display_size_slider, 2, 2);
	main_layout = new QVBoxLayout;
	main_layout->addLayout(slider_layout);
	main_layout->addWidget(button);
	main_layout->addWidget(buttonBox);
	setLayout(main_layout);

	setMinimumSize(640, 480);
}

void SettingDialog::setDefaultParameters(const int game_state_font_size, const int display_minimum_height)
{
	font_size_slider->setValue(game_state_font_size);
	display_size_slider->setValue(display_minimum_height);
}

void SettingDialog::fontSizeValueChanged(int value)
{
	emit fontSizeChanged(value);
}

void SettingDialog::displaySizeValueChanged(int value)
{
	emit displaySizeChanged(value);
}

