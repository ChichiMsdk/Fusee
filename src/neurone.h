#ifndef NEURONE_H
#define NEURONE_H

#include <stdio.h>

typedef struct Layer
{
	double*		pInputs;
	double*		pWeights;
	size_t		inputCount;

	double*		pOutputs;
	size_t		outputCount;
} Layer;

typedef struct IntArray
{
	int		count;
	int*	pArray;
} IntArray;

#define INPUT_SIZE  5
#define HIDDEN_SIZE 4
#define OUTPUT_SIZE 4

void feedforward(double input[], double hidden[], double output[], double input_weights[][HIDDEN_SIZE], double output_weights[][OUTPUT_SIZE]);

void initialize_weights(double weights[][HIDDEN_SIZE], int rows, int cols);

void FeedForward(
		Layer*								pLayers,
		size_t								count,
		double*								pInputs);

void RandomWeights(
		Layer*								pLayers,
		size_t								count);

Layer* LayerCreate(
		IntArray							perLayerCount,
		size_t								layerCount,
		size_t								inputCount,
		size_t								outputCount);

#endif
