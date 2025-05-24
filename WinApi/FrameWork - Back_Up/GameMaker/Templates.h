#pragma once

template<typename T>
void Safe_Delete(T& ptr) {
	if (ptr) {
		delete ptr;
		ptr = nullptr;
	}
}
