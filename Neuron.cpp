#include "Neuron.h"
#include <iostream>

float Neuron::calcOutput()
{
  output = 1.0 / (1.0+exp( -sum * SIGMOID_CONST + threshold));
  return output;
}

float Neuron::getOutput() const
{ 
  //std::cout << output << std::endl;
  return output;
}

float Neuron::getThreshold() const
{
  return threshold;
}

void Neuron::addWeightedInput( float weight, float input )
{
  //std::cout << "Weight: " << weight << " input: " << input << std::endl;
  sum += weight * input;
}
