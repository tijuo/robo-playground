#ifndef NEURON_H
#define NEURON_H

#include <cmath>

#define SIGMOID_CONST	1.0

class Neuron
{
  public:
    Neuron() : sum(0.0), output(0.0), threshold(0.0) { }
    virtual ~Neuron() { }

    virtual float calcOutput();
    virtual float getOutput() const;
    void clearSum() { sum = 0.0; }
    float getThreshold() const;
    virtual void addWeightedInput( float weight, float input );
    virtual void setThreshold( float t ) { threshold = t; }

  protected:
    float sum, output, threshold;
};

#endif /* NEURON_H */
