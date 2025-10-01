// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "neuralNetwork.generated.h"

USTRUCT(BlueprintType)
struct FNeuronWeights {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> weights;
};

USTRUCT(BlueprintType)
struct FLayersBiases{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> biases;
};

USTRUCT(BlueprintType)
struct FLayersWeights {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNeuronWeights> neurons;
};

USTRUCT(BlueprintType)
struct FNetworkData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float fitness = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLayersWeights> weights;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLayersBiases> biases;
};

class AIDRIVING_API neuralNetwork {
public:
	neuralNetwork();
	~neuralNetwork();

	// neural network
	// give inputs get outputs
	UFUNCTION(BlueprintCallable)
	TArray<float> feedForward(const TArray<float>& inputs);
	// setup random weights and biases for network
	UFUNCTION(BlueprintCallable)
	void InitializeNetwork(int inputCount, int hiddenCount, int outputCount);
	// copy the network from previous objects
	UFUNCTION(BlueprintCallable)
	void InitializeNetwork(TArray<FLayersWeights> prevWeights, TArray<FLayersBiases> prevBiases);

	FNetworkData crossover(FNetworkData parentA, FNetworkData parentB);
	void Mutate(FNetworkData& child);

	UFUNCTION()
	TArray<FLayersWeights> getWeights();
	UFUNCTION()
	TArray<FLayersBiases> getBiases();

	//UFUNCTION()
	//TArray<TArray<TArray<float>>> weightStructToList(TArray<
	void setFitness(float newFitness);
	float getFitness();

	// weights[layer][neuron][input]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLayersWeights> weights;
	// biases[layer][neuron]
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FLayersBiases> biases;
	float fitness = 0.f;

private:
	UFUNCTION()
	float ActivationFunction(float x);
	UFUNCTION()
	float RandomWeight(float min = -1.0f, float max = 1.0f);

	
};
