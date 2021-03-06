#pragma once

#include <glm/glm.hpp>

template<typename T>
class Canvas {
public:
	T *map;
	int w, h;

public:
	Canvas() : map(nullptr), w(0), h(0) {}
	Canvas(int w_, int h_);
	~Canvas() { /*if (map) delete[] map;*/ }

	// not a safe method!!
	T &at(int i, int j);

	void setAll(const T &val);
	bool isValid(double i, double j);
};

template<typename T>
inline Canvas<T>::Canvas(int w_, int h_) :
	w(w_), h(h_) {
	map = new T[w * h];
	for (int i = 0; i < w * h; ++i)
		map[i] = T(0);
}

template<typename T>
inline T &Canvas<T>::at(int i, int j) {
	return map[i * h + j];
}

template<typename T>
inline void Canvas<T>::setAll(const T &val) {
	for (int i = 0; i < w * h; ++i)
		map[i] = val;
}

template<typename T>
inline bool Canvas<T>::isValid(double i, double j) {
	return 0 <= i && i < w && 0 <= j && j < h;
}
