#include "CanvasWidget.h"
#include "RasterRenderer.h"
#include "Renderer.h"
#include "Scene.h"

#include <iostream>

RasterRenderer::RasterRenderer(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	int w, h;
	w = 1024;
	h = 768;

	//std::string inputfile = "../Resources/cube/cube.obj";
	std::string inputfile = "../Resources/dragon/dragon.obj";
	Scene scene(inputfile);

	Camera cam;
	cam.lookAt(
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 }
	);
	cam.perspective(
		45.0,
		w * 1.0 / h,
		0.1,
		100.0
	);

	Renderer renderer(w, h);
	renderer.setCamera(cam);
	renderer.render(scene);

	CanvasWidget *canvasWidget = new CanvasWidget(this, renderer.frame);
	canvasWidget->setFixedSize(w, h);

	this->setCentralWidget(canvasWidget);
}
