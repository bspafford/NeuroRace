// Fill out your copyright notice in the Description page of Project Settings.


#include "neuralNetwork.h"

neuralNetwork::neuralNetwork() {

}

neuralNetwork::~neuralNetwork() {

}

/*
network

inputs
	distance sensors?
	speed
	angle to track <- this might tell if its going the right way, if positive then correct, if negative then wrong
	maybe the point system to make sure its going the correct way?

1-2 hidden layers
5-20 neurons

outputs
	left, right
	forward, backwards / break (maybe no reverse)

*/

void neuralNetwork::InitializeNetwork(int inputCount, int hiddenCount, int outputCount) {
	weights.Empty();
	biases.Empty();

	// Input -> Hidden layer
	FLayersWeights hiddenWeights;
	hiddenWeights.neurons.SetNum(hiddenCount);
	for (int h = 0; h < hiddenCount; ++h) {
		for (int i = 0; i < inputCount; ++i) {
			hiddenWeights.neurons[h].weights.Add(RandomWeight());
		}
	}
	weights.Add(hiddenWeights);

	// Hidden -> Output layer
	FLayersWeights outputWeights;
	outputWeights.neurons.SetNum(outputCount);
	for (int o = 0; o < outputCount; ++o) {
		for (int h = 0; h < hiddenCount; ++h) {
			outputWeights.neurons[o].weights.Add(RandomWeight());
		}
	}
	weights.Add(outputWeights);

	// Optional: add random biases for each neuron
	FLayersBiases hiddenLayer;
	hiddenLayer.biases.Add(hiddenCount);
	hiddenLayer.biases.Add(RandomWeight());
	biases.Add(hiddenLayer);

	FLayersBiases outputLayer;
	outputLayer.biases.Add(outputCount);
	outputLayer.biases.Add(RandomWeight());
	biases.Add(outputLayer);
}

void neuralNetwork::InitializeNetwork(TArray<FLayersWeights> prevWeights, TArray<FLayersBiases> prevBiases) {
	this->weights = prevWeights;
	this->biases = prevBiases;
}

// inputs: first 5 are distance to edge, then speed, then angle
TArray<float> neuralNetwork::feedForward(const TArray<float>& inputs) {
	TArray<float> activations = inputs;

	for (int layer = 0; layer < weights.Num(); ++layer) {
		TArray<float> next_activations;
		for (int neuron = 0; neuron < weights[layer].neurons.Num(); ++neuron) {
			float sum = 0.f;
			for (int i = 0; i < activations.Num(); i++) {
				sum += weights[layer].neurons[neuron].weights[i] * activations[i];
			}

			sum += biases[layer].biases[neuron];
			next_activations.Add(ActivationFunction(sum));
		}

		activations = next_activations;
	}

	return activations;
}






/*
TArray<float> neuralNetwork::feedForward(const TArray<float>& inputs) {
	TArray<float> activations = inputs;

	for (int layer = 0; layer < weights.Num(); ++layer) {
		TArray<float> next_activations;
		for (int neuron = 0; neuron < weights[layer].neurons.Num(); ++neuron) {
			float sum = 0.f;
			for (int i = 0; i < activations.Num(); i++) {
				sum += weights[layer].neurons[neuron].weights[i] * activations[i];
			}

			sum += biases[layer].biases[neuron];
			next_activations.Add(ActivationFunction(sum));
		}

		activations = next_activations;
	}

	return activations;
}
*/


FNetworkData neuralNetwork::crossover(FNetworkData parentA, FNetworkData parentB) {
	FNetworkData child = parentA;

	// crossover 50/50 split
	for (int layer = 0; layer < parentA.weights.Num(); layer++) {
		for (int neuron = 0; neuron < parentA.weights[layer].neurons.Num(); neuron++) {
			// switch weights
			for (int i = 0; i < parentA.weights[layer].neurons[neuron].weights.Num(); i++) {
				float weightA = parentA.weights[layer].neurons[neuron].weights[i];
				float weightB = parentB.weights[layer].neurons[neuron].weights[i];
				child.weights[layer].neurons[neuron].weights[i] = FMath::RandBool() ? weightA : weightB;
			}
		}
	}

	for (int layer = 0; layer < parentA.biases.Num(); layer++) {
		for (int neuron = 0; neuron < parentA.biases[layer].biases.Num(); neuron++) {
			// switch biases
			float biasA = parentA.biases[layer].biases[neuron];
			float biasB = parentB.biases[layer].biases[neuron];
			child.biases[layer].biases[neuron] = FMath::RandBool() ? biasA : biasB;
		}
	}

	return child;
}

void neuralNetwork::Mutate(FNetworkData& child) {
	float MutationChance = 0.05f;
	float randomness = .5f;

	for (auto& layer : child.weights) {
		for (auto& neuron : layer.neurons) {
			for (auto& weight : neuron.weights) {
				if (FMath::FRand() < MutationChance) {
					weight += FMath::FRandRange(-randomness, randomness);
					weight = FMath::Clamp(weight, -1.f, 1.f);
				}
			}
		}
	}

	for (auto& layer : child.biases) {
		for (auto& bias : layer.biases) {
			if (FMath::FRand() < MutationChance) {
				bias += FMath::FRandRange(-randomness, randomness);
				bias = FMath::Clamp(bias, -1.f, 1.f);
			}
		}
	}
}

float neuralNetwork::ActivationFunction(float x) {
	return tanh(x);
}

float neuralNetwork::RandomWeight(float min, float max) {
	return FMath::FRandRange(min, max);
}

TArray<FLayersWeights> neuralNetwork::getWeights() {
	return weights;
}

TArray<FLayersBiases> neuralNetwork::getBiases() {
	return biases;
}

void neuralNetwork::setFitness(float newFitness) {
	fitness = newFitness;
}

float neuralNetwork::getFitness() {
	return fitness;
}