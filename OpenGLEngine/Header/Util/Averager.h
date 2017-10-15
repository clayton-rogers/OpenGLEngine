#pragma once

#include <array>

template <typename T, int SAMPLES>
class Averager {
	std::array<T, SAMPLES> samples;
	int nextLocation = 0;
	int num_samples = 0;

public:
	void push(T sample) {
		samples[nextLocation] = sample;
		nextLocation = (nextLocation + 1) % SAMPLES;
		if (num_samples < SAMPLES) { ++num_samples; }
	}

	T sample() {
		T sum = T(0.0);
		for (auto sample : samples) {
			sum += sample;
		}
		return (num_samples == 0) ? T(0) : sum / num_samples;
	}
};