#pragma once

#include <chrono>    // for high_resolution_clock


class timer {
public:
	timer() : t1(std::chrono::high_resolution_clock::now()) {}
	double elapsed() {
		return std::chrono::duration_cast<std::chrono::duration<double>>(
			std::chrono::high_resolution_clock::now() - t1)
			.count();
	}
	void restart() { t1 = std::chrono::high_resolution_clock::now(); }
	void start() { t1 = std::chrono::high_resolution_clock::now(); }
	void stop() { t2 = std::chrono::high_resolution_clock::now(); }
	double total() {
		stop();
		return std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1)
			.count();
	}
	~timer() {}

private:
	std::chrono::high_resolution_clock::time_point t1, t2;
};