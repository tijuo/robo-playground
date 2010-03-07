#include "RobotDna.h"
#include "Brain.h"
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>

RobotDna::RobotDna()
{
    dnaData = NULL;
    dnaLength = 0;
}

RobotDna::RobotDna ( unsigned char *data, unsigned int len ) : dnaLength ( len )
{
    assert ( len >= 6 );

    dnaData = new unsigned char[dnaLength];
    memcpy ( dnaData, data, dnaLength );
}

RobotDna::RobotDna ( const RobotDna &dna )
{
    assert ( dna.dnaLength >= 6 );

    dnaLength = dna.dnaLength;

// std::cout << "Copying dna" << std::endl;

    dnaData = new unsigned char[dnaLength];
    memcpy ( dnaData, dna.dnaData, dnaLength );
}

RobotDna::RobotDna ( const char *filename )
{
    FILE *f;

    f = fopen ( filename, "r" );

    fseek ( f, 0, SEEK_END );

    dnaLength = ftell ( f );

    fseek ( f, 0, SEEK_SET );

    dnaData = new unsigned char[dnaLength];

    for ( unsigned i = 0; i < dnaLength; i++ )
        dnaData[i] = fgetc ( f );

    fclose ( f );
}

RobotDna::~RobotDna()
{
// std::cout << "Destroying dna: " << (unsigned int)dnaData << std::endl;

    if ( dnaData != NULL )
        delete [] dnaData;

    dnaData = NULL;

    dnaLength = 0;
}

RobotDna &RobotDna::operator= ( const RobotDna & rhs )
{
    this->dnaLength = rhs.dnaLength;
    this->dnaData = new unsigned char[this->dnaLength];

    memcpy ( this->dnaData, rhs.dnaData, this->dnaLength );

    return *this;
}

Brain *RobotDna::generateBrain ( unsigned int num_in, unsigned int num_out ) const
{
    unsigned char *weight;

    if ( dnaData == NULL )
        return NULL;

    weight = ( unsigned char * ) & dnaData[6];

    if ( ( NUM_HIDDEN == 0 ) && ( ( num_in * num_out + num_out ) + 6 != dnaLength ) )
        return NULL;
    else if ( ( NUM_HIDDEN > 0 ) && ( num_in*NUM_NEURONS + NUM_NEURONS * ( num_out + ( NUM_HIDDEN - 2 ) ) + NUM_HIDDEN * NUM_NEURONS + num_out + 6 != dnaLength ) )
        return NULL;

    Brain *brain = new Brain ( NUM_HIDDEN );

    for ( unsigned i = 0; i < num_in; i++ )
        brain->addInputNeuron ( 0.0 );

    for ( unsigned i = 0; i < NUM_HIDDEN; i++ )
    {
        for ( unsigned j = 0; j < NUM_NEURONS; j++ )
            brain->addHiddenNeuron ( 1 + i, ( *weight - WEIGHT_OFFSET ) / ( float ) WEIGHT_DENOMINATOR );
    }

    for ( unsigned i = 0; i < num_out; i++ )
        brain->addOutputNeuron ( ( *weight - WEIGHT_OFFSET ) / ( float ) WEIGHT_DENOMINATOR);

    if ( NUM_HIDDEN > 0 )
    {
        for ( unsigned i = 0; i < num_in; i++ )
        {
            for ( unsigned j = 0; j < NUM_NEURONS; j++, weight++ )
            {
                brain->addConnection ( ( *weight - WEIGHT_OFFSET ) / ( float ) WEIGHT_DENOMINATOR,
                                       0, i, 1, j );
            }
        }

      for ( unsigned k = 1; static_cast<signed> ( k ) < NUM_HIDDEN; k++ )
      {
          for ( unsigned i = 0; i < NUM_NEURONS; i++ )
          {
              for ( unsigned j = 0; j < NUM_NEURONS; j++, weight++ )
                  brain->addConnection ( ( *weight - WEIGHT_OFFSET ) / ( float ) WEIGHT_DENOMINATOR,
                                         k, i, k + 1, j );
          }

      }
    }

    for ( unsigned i = 0; i < NUM_NEURONS; i++ )
    {
        for ( unsigned j = 0; j < num_out; j++, weight++ )
            brain->addConnection ( ( *weight - WEIGHT_OFFSET ) / ( float ) WEIGHT_DENOMINATOR,
                                   NUM_HIDDEN, i, NUM_HIDDEN+1, j );
    }

    return brain;
}

/*Color RobotDna::getColor() const
{

}
*/

unsigned int RobotDna::maxHP() const
{
    unsigned int max_hp;

    if ( dnaData == NULL )
        return 0;

    max_hp = static_cast<unsigned int> ( dnaData[3] | ( ( dnaData[4] & 0x01 ) << 7 ) );

    return max_hp;
}

unsigned int RobotDna::attack() const
{
    return ( dnaData == NULL ? 0 : static_cast<unsigned int> ( ( dnaData[4] >> 1 ) & 0x1F ) );
}

unsigned int RobotDna::defense() const
{
    unsigned int defense;

    if ( dnaData == NULL )
        return 0;

    defense = static_cast<unsigned int> ( dnaData[4] >> 6 ) | ( ( dnaData[5] & 0x7 ) << 2 );

    return defense;
}

unsigned int RobotDna::maxSpeed() const
{
    if ( dnaData == NULL )
        return 0;

    return static_cast<unsigned int> ( dnaData[5] >> 3 );
}

void RobotDna::serialize ( const char *filename ) const
{
    FILE *f;

    if ( dnaData == NULL )
        return;

    f = fopen ( filename, "w" );

    for ( unsigned i = 0; i < dnaLength; i++ )
        fputc ( dnaData[i], f );

    fclose ( f );
}

void RobotDna::crossover ( const RobotDna &parent, RobotDna **child1, RobotDna **child2 )
{
    unsigned int cut_bit, cut_byte;;
    unsigned char *_data1, *_data2;

    if ( dnaData == NULL || parent.dnaData == NULL )
    {
        ;

        if ( child1 != NULL )
            *child1 = NULL;

        if ( child2 != NULL )
            *child2 = NULL;

        return;
    }

    cut_bit = 1 + ( ( unsigned ) rand() % ( 8 * dnaLength - 2 ) );

    cut_byte = cut_bit / 8;
    cut_bit %= 8;

    _data1 = new unsigned char[dnaLength];
    _data2 = new unsigned char[dnaLength];

    memcpy ( _data1, dnaData, cut_byte );
    memcpy ( _data2, parent.dnaData, cut_byte );
    memcpy ( _data1 + cut_byte + 1, parent.dnaData + cut_byte + 1, dnaLength - cut_byte - 1 );
    memcpy ( _data2 + cut_byte + 1, dnaData + cut_byte + 1, dnaLength - cut_byte - 1 );

    _data1[cut_byte] = ( dnaData[cut_byte] & ( ( 1 << cut_bit ) - 1 ) ) |
                       ( parent.dnaData[cut_byte] &  ~ ( ( 1 << cut_bit ) - 1 ) );
    _data2[cut_byte] = ( parent.dnaData[cut_byte] & ( ( 1 << cut_bit ) - 1 ) ) |
                       ( dnaData[cut_byte] &  ~ ( ( 1 << cut_bit ) - 1 ) );

    if ( child1 != NULL )
        *child1 = new RobotDna ( _data1, dnaLength );

    if ( child2 != NULL )
        *child2 = new RobotDna ( _data2, dnaLength );

    delete _data1;

    delete _data2;
}

void RobotDna::mutate()
{
    unsigned int mutate_bit;

    if ( dnaData == NULL )
        return;

    mutate_bit = ( unsigned ) rand() % ( 8 * dnaLength );

    dnaData[mutate_bit/8] ^= ( 1 << ( mutate_bit % 8 ) );
}
