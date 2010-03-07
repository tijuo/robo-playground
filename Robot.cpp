#include "Robot.h"
#include "RobotDna.h"
#include "RobotArena.h"
#include "Brain.h"
#include "Vector2d.h"

#include <iostream>

Robot::Robot(const char *filename, RobotArena * const arena ) : robotArena(arena),
	maxEnergy(initialEnergy)
{
  this->dna = new RobotDna(filename);
  this->brain = this->dna->generateBrain( NUM_INPUTS, NUM_OUTPUTS );
  this->velocity = new Vector2d();
  this->position = new Vector2d();

  hp = maxHP = this->dna->maxHP();
  attackPwr = this->dna->attack();
  defensePwr = this->dna->defense();
  maxSpeed = this->dna->maxSpeed();

  shotCount = killCount = hitCount = 0;
 // turretRot = 0;
  powerRequired = 10 + /*maxHP / 10.0 + */(attackPwr + defensePwr) / 10.0 + 
                  maxSpeed / 20.0;
  //std::cout << "Power required: " << powerRequired << std::endl;
  energy = initialEnergy;
}

Robot::Robot(const RobotDna &dna, RobotArena * const arena ) : robotArena(arena),
	maxEnergy(initialEnergy)
{
//  std::cout << "Creating dna" << std::endl;
  this->dna = new RobotDna(dna);
  //std::cout << "Dna created" << std::endl;

  this->brain = this->dna->generateBrain( NUM_INPUTS, NUM_OUTPUTS );
  this->velocity = new Vector2d();
  this->position = new Vector2d();

  hp = maxHP = this->dna->maxHP();
  attackPwr = this->dna->attack();
  defensePwr = this->dna->defense();
  maxSpeed = this->dna->maxSpeed();
  shotCount = killCount = hitCount = 0;

//  turretRot = 0;
  powerRequired = 10 + /*maxHP / 5.0 */+ (attackPwr + defensePwr) / 10.0 + 
                  maxSpeed / 20.0;
  //std::cout << "Power required: " << powerRequired << std::endl;
  energy = initialEnergy;
}

Robot::Robot( const Robot &robot ) : robotArena(robot.robotArena), maxEnergy(initialEnergy)
{
  std::cout << "Copying robot" << std::endl;

  this->dna = new RobotDna(*robot.dna);
  this->brain = new Brain(*robot.brain);
  this->velocity = new Vector2d(*robot.velocity);
  this->position = new Vector2d(*robot.position);

  this->attackPwr = robot.attackPwr;
  this->defensePwr = robot.defensePwr;
  this->maxSpeed = robot.maxSpeed;
  this->hp = robot.hp;
  this->maxHP = robot.maxHP;
  this->powerRequired = robot.powerRequired;
  this->shotCount = robot.shotCount;
  this->killCount = robot.killCount;
  this->hitCount = robot.hitCount;
}

Robot::~Robot()
{
 // std::cout << "Removing robot" << std::endl;

  if( dna != NULL )
  {
    delete dna;
    delete brain;
    delete velocity;
    delete position;
  }

  dna = NULL;
  brain = NULL;
  velocity = NULL;
  position = NULL;
}

#include <cassert>

void Robot::think()
{
 // Robot *nearestFriend;
  Robot *nearestEnemy;

  if( energy < powerRequired )
  {
    energy = 0.0;
    velocity->setX(0.0);
    velocity->setY(0.0);
    return;
  }

  //std::cout << "Thinking..." << std::endl;

  assert( robotArena != NULL );

  //nearestFriend = robotArena->getNearestRobot( *this, team == TEAM1 ? TEAM1 : TEAM2 );
  nearestEnemy = robotArena->getNearestRobot( *this );

 // assert( nearestFriend != NULL );
 // assert( nearestEnemy != NULL );
  assert( brain != NULL );
  assert( velocity != NULL );

  brain->setInput( POS_X, position->getX() / robotArena->getWidth() );
 // std::cout << "Input1 set" << std::endl;
  brain->setInput( POS_Y, position->getY() / robotArena->getHeight() );
  brain->setInput( VEL_X, velocity->getX() / maxSpeed );
  brain->setInput( VEL_Y, velocity->getY() / maxSpeed );
  //brain->setInput( TURR_ROT, turretRot / 180.0 );
  //brain->setInput( ENERGY, energy / maxEnergy );
//  brain->setInput( ATK_PWR, (attackPwr + 1) / 32.0 );
//  brain->setInput( DEF_PWR, (defensePwr + 1) / 32.0 );
  //brain->setInput( HP, (float)hp / maxHP );

  //std::cout << "Inputs set" << std::endl;

  if( nearestEnemy != NULL )
  {
    Vector2d vel, pos;

    vel=nearestEnemy->getVelocity();
    pos=nearestEnemy->getPosition();

 //   brain->setInput( ENEMY_HP, nearestEnemy->getHP() / nearestEnemy->getMaxHP() );
    //brain->setInput( ENEMY_TURR_ROT, nearestEnemy->getTurretRot() / 180.0 );
    brain->setInput( ENEMY_DISTANCE, (pos - *position).magnitude() );
    brain->setInput( ENEMY_POS_X, pos.getX() / robotArena->getWidth());
    brain->setInput( ENEMY_POS_Y, pos.getY() / robotArena->getHeight());
    brain->setInput( ENEMY_VEL_X, vel.getX() / nearestEnemy->getMaxSpeed());
    brain->setInput( ENEMY_VEL_Y, vel.getY() / nearestEnemy->getMaxSpeed());
 //   brain->setInput( ENEMY_FIRING, nearestEnemy->isAttacking() == true ? 1.0 : -1.0 );
  }
/*
  if( nearestFriend != NULL )
  {
    Vector2d vel=nearestFriend->getVelocity(), pos=robotArena->getRobotPos(*nearestFriend);

    brain->setInput( FRIEND_HP, nearestFriend->getHP() / nearestFriend->getMaxHP());
    brain->setInput( FRIEND_TURR_ROT, nearestFriend->getTurretRot() / 180.0 );
    brain->setInput( FRIEND_POS_X, pos.getX() / robotArena->getWidth() );
    brain->setInput( FRIEND_POS_Y, pos.getY() / robotArena->getHeight() );
    brain->setInput( FRIEND_VEL_X, vel.getX() / nearestFriend->getMaxSpeed() );
    brain->setInput( FRIEND_VEL_Y, vel.getY() / nearestFriend->getMaxSpeed() );
    brain->setInput( FRIEND_FIRING, nearestFriend->isAttacking() == true ? 1.0 : 0.0 );
  }
*/
  brain->generateOutputs();

  velocity->setX( (brain->getOutput( SET_VEL_X ) - 0.5) * maxSpeed );
  velocity->setY( (brain->getOutput( SET_VEL_Y ) - 0.5) * maxSpeed );

  energy -= 0.3 * robotArena->deltaTime * (-0.025*(fabs(velocity->getX() + velocity->getY())) + powerRequired);

  if( brain->getOutput( ATTACK ) >= 0.5 )
    energy -= ATTACK_ENERGY;

  //std::cout << "Velocity: (" << velocity->getX() << "," << velocity->getY() << ")" << std::endl;

  //*position = *position + *velocity;

  //turretRot = (brain->getOutput( TURR_ROT ) * 360);
  //energy -= powerRequired;

  if( energy < 0.0 )
    energy = 0.0;
}