#include "InputNeuron.h"

void InputNeuron::setInput( float input )
{
  if( input > 1.0 )
    this->input = 1.0;
  else if( input < -1.0 )
    this->input = -1.0;
  else
    this->input = input;

  output = this->input;
}
