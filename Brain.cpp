#include "Brain.h"
#include <iostream>

Brain::Brain(unsigned num_hidden_layers)
{
  if( num_hidden_layers >= 255 )
    num_hidden_layers = 254;

 // std::cout << "Creating " << num_hidden_layers << " hidden layers" << std::endl;

  numHiddenLayers = num_hidden_layers;

  for( unsigned i=0; i < num_hidden_layers+1; i++ )
    neuronLayers.push_back( std::vector<Neuron *>() );
}

Brain::Brain( const Brain &brain ) : numHiddenLayers(brain.numHiddenLayers), numNeurons(brain.numNeurons)
{
  std::cout << "Copying brain..." << std::endl;
  inputLayer = std::vector<InputNeuron *>();

  for( unsigned i=0; i < brain.inputLayer.size(); i++ )
    inputLayer.push_back( new InputNeuron(*brain.inputLayer[i]) );

  neuronConnections = std::vector<struct NeuronConnection>(brain.neuronConnections);
  neuronLayers = std::vector<NeuronLayer>();

  for(unsigned i=0; i < numHiddenLayers+1; i++)
  {
    for( unsigned j=0; j < brain.neuronLayers[i].size(); j++ )
      neuronLayers[i].push_back( new Neuron( *brain.neuronLayers[i][j] ) );
  }
}

Brain &Brain::operator=( const Brain &rhs )
{
  inputLayer = std::vector<InputNeuron *>();

  for( unsigned i=0; i < rhs.inputLayer.size(); i++ )
    inputLayer.push_back( new InputNeuron(*rhs.inputLayer[i]) );

  neuronConnections = std::vector<struct NeuronConnection>(rhs.neuronConnections);
  neuronLayers = std::vector<NeuronLayer>();

  for(unsigned i=0; i < numHiddenLayers+1; i++)
  {
    for( unsigned j=0; j < rhs.neuronLayers[i].size(); j++ )
      neuronLayers[i].push_back( new Neuron( *rhs.neuronLayers[i][j] ) );
  }

  return *this;
}

Brain::~Brain()
{
  NeuronLayer layer;

  while( inputLayer.size() )
  {
    delete inputLayer.back();
    inputLayer.pop_back();
  }

  while( neuronLayers.size() )
  {
    layer = neuronLayers.back();

    while( layer.size() )
    {
      delete layer.back();
      layer.pop_back();
    }

    neuronLayers.pop_back();
  }
}

void Brain::addConnection( float weight, unsigned char src_layer, unsigned char src_neuron,
   unsigned char dst_layer, unsigned char dst_neuron )
{
  struct NeuronConnection conn = { src_layer, src_neuron, dst_layer, dst_neuron, weight };

  if( conn.weight > 1.0 )
    conn.weight = 1.0;
  else if( conn.weight < -1.0 )
    conn.weight = -1.0;

  neuronConnections.push_back( conn );
}

void Brain::generateOutputs()
{
  float out;
  struct NeuronConnection *connection;

  for( unsigned i=0; i < numHiddenLayers+1; i++ )
  {
    for( unsigned j=0; j < neuronLayers[i].size(); j++ )
      neuronLayers[i][j]->clearSum();
  }

  for( unsigned i=0; i < neuronConnections.size(); i++ )
  {
    connection = &neuronConnections[i];

    if( connection->dstLayer % (numHiddenLayers + 2) == 0 )
      continue;

    out = getNeuronOutput(connection->srcLayer, connection->srcNeuron);

    neuronLayers[(connection->dstLayer % (numHiddenLayers+2)) - 1]
                   [connection->dstNeuron % 
                   neuronLayers[(connection->dstLayer % 
                   (numHiddenLayers+2)) - 1].size()]->
                   addWeightedInput(connection->weight, out);
  }

  for( unsigned i=0; i < numHiddenLayers+1; i++ )
  {
    for( unsigned j=0; j < neuronLayers[i].size(); j++ )
      neuronLayers[i][j]->calcOutput();
  }
}

void Brain::addInputNeuron(float input)
{
  inputLayer.push_back(new InputNeuron(input));
}

void Brain::addOutputNeuron( float threshold )
{
  addHiddenNeuron( numHiddenLayers+1, threshold );
}

void Brain::addHiddenNeuron(unsigned char layer, float threshold)
{
  Neuron *neuron = new Neuron();
  neuron->setThreshold(threshold);

  if( layer % (numHiddenLayers + 2) > 0 )
    neuronLayers[(layer % (numHiddenLayers + 2))-1].push_back(neuron);
}

void Brain::setInput(unsigned char in_neuron, float value)
{
  inputLayer[in_neuron % inputLayer.size()]->setInput(value);
}

float Brain::getOutput(unsigned char out_neuron)
{
  return getNeuronOutput(numHiddenLayers+1, out_neuron);
}

float Brain::getNeuronOutput(unsigned char layer, unsigned char neuron)
{
  if( layer % (numHiddenLayers + 2) == 0 )
    return inputLayer[neuron % inputLayer.size()]->getOutput();
  else
    return neuronLayers[(layer % (numHiddenLayers + 2)) - 1][neuron % 
                           neuronLayers[(layer % (numHiddenLayers + 2)) 
                                        - 1].size()]->getOutput();
}

void Brain::setNeuronThreshold(unsigned char layer, unsigned char neuron, float threshold)
{
  if(layer % (numHiddenLayers + 2) > 0 )
    neuronLayers[(layer % (numHiddenLayers + 2)) - 1][neuron %
                          neuronLayers[(layer % (numHiddenLayers + 2)) - 
                          1].size()]->setThreshold(threshold);
}

