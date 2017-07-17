#include "Camera.h"

void Camera::lookAt(glm::dvec3 eye, glm::dvec3 center, glm::dvec3 up) {
	eye_ = eye;
	center_ = center;

	glm::dvec3 x, y, z;
	y = glm::normalize(up);
	z = glm::normalize(eye - center);
	x = glm::cross(y, z);

	glm::dmat4 R(
		x[0], x[1], x[2], 0.0,
		y[0], y[1], y[2], 0.0,
		z[0], z[1], z[2], 0.0,
		0.0, 0.0, 0.0, 1.0
	);
	glm::dmat4 T(
		1.0, 0.0, 0.0, -eye[0],
		0.0, 1.0, 0.0, -eye[1],
		0.0, 0.0, 1.0, -eye[2],
		0.0, 0.0, 0.0, 1.0
	);
	viewMat_ = glm::transpose(R) * glm::transpose(T);
}

void Camera::perspective(double fovy, double aspect, double zNear, double zFar) {
	double f = 1.0 / tan(fovy * 0.5);
	projMat_ = glm::transpose(glm::dmat4(
		f / aspect,		0.0,	0.0,								0.0,
		0.0,			f,		0.0,								0.0,
		0.0,			0.0,	(zFar + zNear) / (zNear - zFar),	(2 * zFar * zNear) / (zNear - zFar),
		0.0,			0.0,	-1.0,								0.0
	));
}
