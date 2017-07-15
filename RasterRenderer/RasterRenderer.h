#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_RasterRenderer.h"

class RasterRenderer : public QMainWindow {
	Q_OBJECT

public:
	RasterRenderer(QWidget *parent = Q_NULLPTR);

private:
	Ui::RasterRendererClass ui;
};
