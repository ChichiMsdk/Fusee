#include "neurone.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static inline float
Sigmoid(float output)
{
	return 1 / (1 + pow(M_E, (-1 * output)));
}

float
Aggregation(double* pInputs, double* pWeights, size_t count)
{
	double	total = 0;
	for (size_t i = 0; i < count; i++)
	{
		total += pInputs[i] * pWeights[i];
	}
	return Sigmoid(total);
}

void
FeedForward(Layer* pLayers, size_t count, double* pInputs)
{
	for (size_t i = 0; i < 4; i++)
	{
		pLayers[0].pInputs[i] = pInputs[i];
		pLayers[0].pOutputs[i] = pInputs[i];
		pLayers[1].pInputs[i] = pLayers[i].pOutputs[i];
	}
	for (size_t i = 1; i < count; i++)
	{
		for (size_t j = 0; j < pLayers[i].inputCount; j++)
		{
			pLayers[i].pOutputs[j] = Aggregation(pLayers[i].pInputs, pLayers[i].pWeights, pLayers[i].inputCount);
			if (i + 1 < count)
			{
				pLayers[i + 1].pInputs[j] = pLayers[i].pOutputs[j];
				/* pLayers[i + 1].inputCount++; */
			}
		}
	}
}

void
RandomWeights(Layer* pLayers, size_t count)
{
	for (size_t i = 1; i < count; i++)
	{
		for (size_t j = 0; j < pLayers[i].inputCount; j++)
		{
			pLayers[i].pWeights[j] = (double) rand() / RAND_MAX * 2 - 1;
		}
	}
}

void
feedforward(double input[], double hidden[], double output[],
                 double input_weights[][HIDDEN_SIZE], double output_weights[][OUTPUT_SIZE]) {
    
    // Hidden layer
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        hidden[i] = 0.0;
        for (int j = 0; j < INPUT_SIZE; j++) {
            hidden[i] += input[j] * input_weights[j][i];
        }
        hidden[i] = Sigmoid(hidden[i]);
    }

    // Output layer
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        output[i] = 0.0;
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            output[i] += hidden[j] * output_weights[j][i];
        }
        output[i] = Sigmoid(output[i]);
    }
}

void initialize_weights(double weights[][HIDDEN_SIZE], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            weights[i][j] = (double)rand() / RAND_MAX * 2 - 1; // Random values in [-1, 1]
        }
    }
}

Layer*
LayerCreate(IntArray perLayerCount, size_t layerCount, size_t inputCount, size_t outputCount)
{
	Layer* pLayers = malloc(sizeof(Layer) * layerCount);
	size_t i = 0;

	pLayers->inputCount = inputCount;
	pLayers->outputCount = 4;
	pLayers->pWeights = malloc(sizeof(double) * 4);
	pLayers->pInputs = malloc(sizeof(double) * 4);
	pLayers->pOutputs = malloc(sizeof(double) * 4);

	for (i = 1; i < layerCount - 1; i++)
	{
		pLayers[i].pWeights = malloc(sizeof(double) * pLayers[i - 1].outputCount);
		pLayers[i].pInputs = malloc(sizeof(double) * pLayers[i - 1].outputCount);
		pLayers[i].pOutputs = malloc(sizeof(double) * *perLayerCount.pArray);
		pLayers[i].inputCount = pLayers[i - 1].outputCount;
		pLayers[i].outputCount = *perLayerCount.pArray;
	}
	pLayers[i].pWeights = malloc(sizeof(double) * inputCount);
	pLayers[i].pInputs = malloc(sizeof(double) * outputCount);
	pLayers[i].pOutputs = malloc(sizeof(double) * outputCount);
	pLayers[i].inputCount = 4;
	pLayers[i].outputCount = outputCount;

	RandomWeights(pLayers, layerCount);
	return pLayers;
}
