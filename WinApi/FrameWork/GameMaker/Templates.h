#pragma once

template<typename T>
void Safe_Delete(T& ptr) {
	if (ptr) {
		delete ptr;
		ptr = nullptr;
	}
}

template <typename T>
constexpr const T& Clamp(const T& v, const T& lo, const T& hi) {
	return (v < lo) ? lo : (hi < v) ? hi : v;
}
