#ifndef INPUT_NEURON
#define INPUT_NEURON

#include "Neuron.h"

class InputNeuron : public Neuron
{
  public:
    InputNeuron( float _input ) : Neuron() { setInput(_input); }
    InputNeuron( const InputNeuron &n ) : Neuron(n) { input = n.input; }
    ~InputNeuron() { }

    void addWeightedInput( float weight, float input ) { }
    float calcOutput() const { return input; }
    void setInput( float input );
    float getInput() const { return input; }

  protected:
    float input;
};

#endif /* INPUT_NEURON */
