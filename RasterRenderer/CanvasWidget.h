#pragma once

#include "Canvas.h"

#include <QWidget>

class CanvasWidget : public QWidget {
	Q_OBJECT

private:
	Canvas<glm::dvec3> *frame;
	QImage *img;

public:
	CanvasWidget(QWidget *parent, Canvas<glm::dvec3> *frame_);
	~CanvasWidget() {}

	void paintEvent(QPaintEvent *);

	void frameToQImage();
};