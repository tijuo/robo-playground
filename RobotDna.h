#ifndef ROBOT_DNA_H
#define ROBOT_DNA_H

#include "Brain.h"
#include <string.h>
/*
   Robots have five attributes and a brain
   maxHP - maximum number of hitpoints
   attack - the number of attack points (robot power)
   defense - the number of defense points (robot's resistance to attack)
   maxSpeed - maximum speed that a robot can travel

   Each of these statistics determine how much power that a robot will use up
   Higher statistics use more power. (power = energy / time)
*/

#define WEIGHT_DENOMINATOR	128
#define WEIGHT_OFFSET		128
#define MAX_NEG_INT		-256

struct DnaConnection
{
  unsigned char srcLayer, srcNeuron;
  unsigned char dstLayer, dstNeuron;
  unsigned char weight;
};

class RobotDna
{
  public:
    RobotDna();
    RobotDna( unsigned char *data, unsigned int len );
    RobotDna( const char *filename );
    RobotDna( const RobotDna &dna );
    ~RobotDna();

    RobotDna &operator=(const RobotDna &rhs);

    Brain *generateBrain( unsigned int num_in, unsigned int num_out ) const;
    //Color getColor() const;
    unsigned int maxHP() const;
    unsigned int attack() const;
    unsigned int defense() const;
    unsigned int maxSpeed() const;
    unsigned int length() const { return dnaLength; }

    void serialize( const char *filename ) const;

    RobotDna clone() const { return RobotDna(*this); }
    void crossover( const RobotDna &parent, RobotDna **child1, RobotDna **child2 );
    void mutate();
  protected:
    unsigned char *dnaData;
    unsigned int dnaLength;
};

#endif /* ROBOT_DNA_H */
