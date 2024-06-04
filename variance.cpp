#include "variance.h"

float calculateVariance(float* values, int size) {
  float sum = 0;
  float mean = 0;
  float variance = 0;

  for (int i = 0; i < size; i++) {
    sum += values[i];
  }
  mean = sum / size;

  for (int i = 0; i < size; i++) {
    variance += pow(values[i] - mean, 2);
  }
  variance /= size;

  return variance;
}

