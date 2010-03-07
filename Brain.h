#ifndef BRAIN_H
#define BRAIN_H

#include "Neuron.h"
#include "InputNeuron.h"
#include <vector>

#define NUM_INPUTS	9
#define NUM_HIDDEN	0
#define NUM_NEURONS	9
#define NUM_OUTPUTS	3

struct NeuronConnection
{
  unsigned char srcLayer, srcNeuron;
  unsigned char dstLayer, dstNeuron;
  float weight;
};

/* Assumes that there is at least one hidden layer */

class Brain
{
  public:
    explicit Brain(unsigned num_hidden);
    Brain( const Brain &brain );
    ~Brain();

    Brain &operator=( const Brain &rhs );

    void generateOutputs();
    void addInputNeuron(float input);
    void addOutputNeuron(float threshold/*, enum NeuronFunc funcType*/);
    void addHiddenNeuron(unsigned char layer, float threshold/*, 
                       enum NeuronFunc funcType*/);

    void addConnection( float weight, unsigned char src_layer,
                        unsigned char src_neuron, unsigned char dst_layer,
                        unsigned char dst_neuron );

    void setInput(unsigned char in_neuron, float value);
    float getOutput(unsigned char out_neuron);

    float getNeuronOutput(unsigned char layer, unsigned char neuron);
    unsigned getLayers() const { return numHiddenLayers; }
    void setNeuronThreshold(unsigned char layer, unsigned char neuron, float threshold);
  private:
    typedef std::vector<Neuron *> NeuronLayer;
    std::vector<NeuronLayer> neuronLayers; // Layers 1 through N-1
    std::vector<InputNeuron *> inputLayer; // Layer 0
    unsigned int numHiddenLayers;
    std::vector<unsigned int> numNeurons;
    std::vector<struct NeuronConnection> neuronConnections;
};

#endif /* BRAIN_H */
