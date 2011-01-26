#include "Robot.h"
#include "RobotDna.h"
#include "RobotArena.h"
#include "Brain.h"
#include "Vector2d.h"

#include <iostream>

Robot::Robot(const char *filename, RobotArena * const arena ) : _dna(NULL),
  brain(NULL), _maxEnergy(initialEnergy), robotArena(arena)
{
  if( filename != NULL )
  {
    this->_dna = new RobotDna(filename);
    this->brain = this->_dna->generateBrain( NUM_INPUTS, NUM_OUTPUTS );
  }

  _hp = _maxHP = this->_dna->maxHP();
  _attackPwr = this->_dna->attack();
  _defensePwr = this->_dna->defense();
  _maxSpeed = this->_dna->maxSpeed();

  _shotCount = _killCount = _hitCount = 0;
 // turretRot = 0;
  _powerRequired = 10 + /*maxHP / 10.0 + */(_attackPwr + _defensePwr) / 10.0 + 
                  _maxSpeed / 20.0;
  //std::cout << "Power required: " << powerRequired << std::endl;
  _energy = initialEnergy;
}

Robot::Robot(const RobotDna &dna, RobotArena * const arena ) : 
  _maxEnergy(initialEnergy), robotArena(arena)
{
//  std::cout << "Creating dna" << std::endl;
  this->_dna = new RobotDna(dna);
  //std::cout << "Dna created" << std::endl;

  this->brain = this->_dna->generateBrain( NUM_INPUTS, NUM_OUTPUTS );

  _hp = _maxHP = this->_dna->maxHP();
  _attackPwr = this->_dna->attack();
  _defensePwr = this->_dna->defense();
  _maxSpeed = this->_dna->maxSpeed();
  _shotCount = _killCount = _hitCount = 0;

//  turretRot = 0;
  _powerRequired = 10 + /*maxHP / 5.0 */+ (_attackPwr + _defensePwr) / 10.0 + 
                  _maxSpeed / 20.0;
  //std::cout << "Power required: " << powerRequired << std::endl;
  _energy = initialEnergy;
}

Robot::Robot( const Robot &robot ) : _dna(NULL), brain(NULL), 
  _maxEnergy(initialEnergy), robotArena(robot.robotArena)
{
  std::cout << "Copying robot" << std::endl;

  if( this->_dna )
    this->_dna = new RobotDna(*robot._dna);

  if( this->brain )
    this->brain = new Brain(*robot.brain);

  this->_velocity = robot._velocity;
  this->_position = robot._position;

  this->_attackPwr = robot._attackPwr;
  this->_defensePwr = robot._defensePwr;
  this->_maxSpeed = robot._maxSpeed;
  this->_hp = robot._hp;
  this->_maxHP = robot._maxHP;
  this->_powerRequired = robot._powerRequired;
  this->_shotCount = robot._shotCount;
  this->_killCount = robot._killCount;
  this->_hitCount = robot._hitCount;
}

Robot::~Robot()
{
 // std::cout << "Removing robot" << std::endl;

  if( _dna )
  {
    delete _dna;
    delete brain;
  }

  _dna = NULL;
  brain = NULL;
}

#include <cassert>

void Robot::think()
{
 // Robot *nearestFriend;
  Robot *nearestEnemy;

  if( !brain )
    return;

  if( _energy < _powerRequired )
  {
    _energy = 0.0;
    _velocity.setX(0.0);
    _velocity.setY(0.0);
    return;
  }

  //std::cout << "Thinking..." << std::endl;

  assert( robotArena != NULL );

  //nearestFriend = robotArena->getNearestRobot( *this, team == TEAM1 ? TEAM1 : TEAM2 );
  nearestEnemy = robotArena->getNearestRobot( *this );

 // assert( nearestFriend != NULL );
 // assert( nearestEnemy != NULL );
  assert( brain != NULL );

  brain->setInput( POS_X, _position.x() / robotArena->getWidth() );
 // std::cout << "Input1 set" << std::endl;
  brain->setInput( POS_Y, _position.y() / robotArena->getHeight() );
  brain->setInput( VEL_X, _velocity.x() / _maxSpeed );
  brain->setInput( VEL_Y, _velocity.y() / _maxSpeed );
  //brain->setInput( TURR_ROT, turretRot / 180.0 );
  //brain->setInput( ENERGY, energy / maxEnergy );
//  brain->setInput( ATK_PWR, (attackPwr + 1) / 32.0 );
//  brain->setInput( DEF_PWR, (defensePwr + 1) / 32.0 );
  //brain->setInput( HP, (float)hp / maxHP );

  //std::cout << "Inputs set" << std::endl;

  if( nearestEnemy != NULL )
  {
    Vector2d vel, pos;

    vel=nearestEnemy->velocity();
    pos=nearestEnemy->position();

 //   brain->setInput( ENEMY_HP, nearestEnemy->getHP() / nearestEnemy->getMaxHP() );
    //brain->setInput( ENEMY_TURR_ROT, nearestEnemy->getTurretRot() / 180.0 );
    brain->setInput( ENEMY_DISTANCE, (pos - _position).magnitude() );
    brain->setInput( ENEMY_POS_X, pos.x() / robotArena->getWidth());
    brain->setInput( ENEMY_POS_Y, pos.y() / robotArena->getHeight());
    brain->setInput( ENEMY_VEL_X, vel.x() / nearestEnemy->maxSpeed());
    brain->setInput( ENEMY_VEL_Y, vel.y() / nearestEnemy->maxSpeed());
 //   brain->setInput( ENEMY_FIRING, nearestEnemy->isAttacking() == true ? 1.0 : -1.0 );
  }
/*
  if( nearestFriend != NULL )
  {
    Vector2d vel=nearestFriend->getVelocity(), pos=robotArena->getRobotPos(*nearestFriend);

    brain->setInput( FRIEND_HP, nearestFriend->getHP() / nearestFriend->getMaxHP());
    brain->setInput( FRIEND_TURR_ROT, nearestFriend->getTurretRot() / 180.0 );
    brain->setInput( FRIEND_POS_X, pos.x() / robotArena->getWidth() );
    brain->setInput( FRIEND_POS_Y, pos.y() / robotArena->getHeight() );
    brain->setInput( FRIEND_VEL_X, vel.x() / nearestFriend->getMaxSpeed() );
    brain->setInput( FRIEND_VEL_Y, vel.y() / nearestFriend->getMaxSpeed() );
    brain->setInput( FRIEND_FIRING, nearestFriend->isAttacking() == true ? 1.0 : 0.0 );
  }
*/
  brain->generateOutputs();

  _velocity.setX( (brain->getOutput( SET_VEL_X ) - 0.5) * _maxSpeed );
  _velocity.setY( (brain->getOutput( SET_VEL_Y ) - 0.5) * _maxSpeed );

  _energy -= 0.3 * robotArena->deltaTime * (-0.025*(fabs(_velocity.x() + _velocity.y())) + _powerRequired);

  if( brain->getOutput( ATTACK ) >= 0.5 )
    _energy -= ATTACK_ENERGY;

  //std::cout << "Velocity: (" << velocity->x() << "," << velocity->y() << ")" << std::endl;

  //*position = *position + *velocity;

  //turretRot = (brain->getOutput( TURR_ROT ) * 360);
  //energy -= powerRequired;

  if( _energy < 0.0 )
    _energy = 0.0;
}
