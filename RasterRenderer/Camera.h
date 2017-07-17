#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	glm::dvec3 eye_;
	glm::dvec3 center_;
	double zNear_;

	glm::dmat4 viewMat_;
	glm::dmat4 projMat_;

public:
	Camera() {}
	~Camera() {}


	void lookAt(glm::dvec3 eye, glm::dvec3 center, glm::dvec3 up);
	void perspective(double fovy, double aspect, double zNear, double zFar);

	glm::dvec3 eye() const { return eye_; }
	glm::dvec3 center() const { return center_; }
	double zNear() const { return zNear_; }
	glm::dmat4 viewMat() const { return viewMat_; }
	glm::dmat4 projMat() const { return projMat_; }
};