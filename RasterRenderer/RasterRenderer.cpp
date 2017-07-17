#include "CanvasWidget.h"
#include "RasterRenderer.h"
#include "PhongShader.h"
#include "Scene.h"

#include <iostream>

RasterRenderer::RasterRenderer(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	int w, h;
	w = 1024;
	h = 768;

	//std::string inputfile = "../Resources/cube/cube.obj";
	//std::string inputfile = "../Resources/dragon/dragon.obj";
	std::string inputfile = "../Resources/conference/conference.obj";
	//std::string inputfile = "../Resources/CornellBox/CornellBox-Sphere.obj";
	//std::string inputfile = "../Resources/buddha/buddha.obj";
	Scene scene(inputfile);

	Camera cam;
	/*cam.lookAt(
		{ 1.0, 0.0, 0.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 }
	);*/
	cam.lookAt(
		{ 300.0, 400.0, -200.0 },
		{ -946.887512, 650.292542, 525.665161 },
		{ 0.0, 1.0, 0.0 }
	);
	/*cam.lookAt(
		{ 0.0, 0.8, 3.0 },
		{ 0.0, 0.8, 0.0 },
		{ 0.0, 0.8, 0.0 }
	);*/
	/*cam.lookAt(
		{ 0.0, 0.0, -1.0 },
		{ 0.0, 0.0, 0.0 },
		{ 0.0, 1.0, 0.0 }
	);*/
	cam.perspective(45.0, w * 1.0 / h, 0.1, 100000.0);

	PhongShader::Light light;
	light.pos = glm::dvec3(300.0, 320.0, -200.0);
	light.color = glm::dvec3(1.0, 1.0, 1.0);
	light.ambientStrength = 0.1;
	light.diffuseStrength = 0.7;
	light.specularStrength = 0.2;

	PhongShader renderer(w, h);
	renderer.setCamera(cam);
	renderer.setLight(light);
	renderer.setShadowMapDim(0);
	renderer.render(scene);

	CanvasWidget *canvasWidget = new CanvasWidget(this, renderer.frame);
	canvasWidget->setFixedSize(w, h);

	this->setCentralWidget(canvasWidget);
}
