#include "workaround.hpp"

#include <cstddef>

using namespace std;

void MatrixProcessor::Threshold(unsigned char* const data, const int width,
                                const int height, const int threshold) {
	for (int i = 0; i < width * height; ++i) {
		if (data[i] < threshold) data[i] = 0;
	}
}

unsigned char* MatrixProcessor::Averaging(unsigned char* const data, const int width, const int height,
								const int range) {
	unsigned char* dataCopy = new unsigned char[width * height];
	for (int k = 0; k < width * height - 1; ++k) {
		int sum = 0;
		int upperRow = k / height - range;
		if (upperRow < 0) upperRow = 0;
		int lowerRow = upperRow + 2 * range;
		if (lowerRow > height - 1) lowerRow = height - 1;
		int leftCol = k % width - range;
		if (leftCol < 0) leftCol = 0;
		int rightCol = leftCol + 2 * range;
		if (rightCol > width - 1) rightCol = width - 1;
		for (int i = upperRow; i < lowerRow; ++i) {
			for (int j = leftCol; i < rightCol; ++j) {
				sum += data[width * i + j];
			}
		}
		dataCopy[k] = sum / (range * range);
	}
	return dataCopy;
}