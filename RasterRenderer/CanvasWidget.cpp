#include "CanvasWidget.h"

#include <QPainter>

#include <iostream>

CanvasWidget::CanvasWidget(QWidget *parent, Canvas<glm::dvec3> *frame_) :
	QWidget(parent), frame(frame_) {
	img = new QImage(frame->w, frame->h, QImage::Format_RGB32);
}

void CanvasWidget::paintEvent(QPaintEvent *) {
	frameToQImage();
	QPainter p(this);
	p.drawImage(QRect(0, 0, frame->w, frame->h), *img);
	saveCanvas("result.png");
}

void CanvasWidget::frameToQImage() {
	for (int i = 0; i < frame->w; ++i) {
		for (int j = 0; j < frame->h; ++j) {
			glm::dvec3 &c = frame->at(i, j);
			img->setPixel(i, frame->h - j - 1, qRgb(c.r * 255, c.g * 255, c.b * 255));
		}
	}
}

void CanvasWidget::saveCanvas(std::string filename) {
	img->save(QString(filename.c_str()));
}
