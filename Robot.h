#ifndef ROBOT_H
#define ROBOT_H

#include "ArenaObject.h"
#include "Brain.h"
#include "RobotArena.h"
#include "RobotDna.h"
#include "Vector2d.h"

#define ATTACK_ENERGY	2.0

enum InputNames { POS_X, POS_Y, ENEMY_DISTANCE,VEL_X, VEL_Y,/*, TURR_ROT,*/ 
                  /*ENERGY, ATK_PWR, DEF_PWR, *//*HP,*//* ENEMY_HP, */
                  /*ENEMY_TURR_ROT, */ENEMY_POS_X, ENEMY_POS_Y, 
		  ENEMY_VEL_X, ENEMY_VEL_Y, /*ENEMY_FIRING*//*,
		  FRIEND_HP, FRIEND_TURR_ROT, FRIEND_POS_X, 
                  FRIEND_POS_Y, FRIEND_VEL_X, FRIEND_VEL_Y, 
                  FRIEND_FIRING*/ };

enum OutputNames { SET_VEL_X, SET_VEL_Y,/*, SET_TURR_ROT, */ATTACK };

const float initialEnergy = 2000.0;

class Robot : public ArenaObject
{
  public:
    Robot() : _dna(NULL), brain(NULL), _maxEnergy(initialEnergy) { }
    Robot(const char *filename, RobotArena * const arena );
    Robot( const Robot &robot );
    Robot(const RobotDna &dna, RobotArena * const arena );

    Robot& operator=(const Robot &rhs) 
    { 
      this->_dna = new RobotDna(*rhs._dna);
      this->brain = new Brain(*rhs.brain);
      this->_velocity = rhs._velocity;
      this->_position = rhs._position;

      this->_attackPwr = rhs._attackPwr;
      this->_defensePwr = rhs._defensePwr;
      this->_maxSpeed = rhs._maxSpeed;
      this->_hp = rhs._hp;
      this->_maxHP = rhs._maxHP;
      this->_powerRequired = rhs._powerRequired;
      this->robotArena = rhs.robotArena;
      this->_maxEnergy = rhs._maxEnergy;

      this->_killCount = rhs._killCount;
      this->_shotCount = rhs._shotCount;
      this->_hitCount = rhs._hitCount;

      return *this;
    }

    ~Robot();

    void doDamage( unsigned int damage ) 
      { _hp -= (damage >= _hp ? _hp : damage); }
    void setEnergy( float val )
      { _energy = (val < 0 ? 0 : (val > _maxEnergy ? _maxEnergy : val )); }
    void setVelocity( const Vector2d & ) {}
    void think();

    RobotDna *dna() const { return _dna; }
//    Brain getBrain() const { return *brain; }
    unsigned int attackPower() const { return _attackPwr; }
    unsigned int defensePower() const { return _defensePwr; }
    float energy() const { return _energy; }
    unsigned int killCount() const { return _killCount; }
    unsigned int hitCount() const { return _hitCount; }
    unsigned int hp() const { return _hp; }
    unsigned int maxHP() const { return _maxHP; }
    unsigned int maxSpeed() const { return _maxSpeed; }
    float requiredPower() const { return _powerRequired; }
    unsigned int shotCount() const { return _shotCount; }

    Vector2d robotDistance( const Robot &other ) const
      { return other._position - _position; }

    void increaseKillCount( ) { _killCount++; }
    void increaseHitCount() { _hitCount++; }
    void increaseShotCount() { _shotCount++; }

   // float getTurretRot() const { return turretRot; }
   // unsigned int getId() const { return id; }
    //void setId( unsigned _id ) { id = _id; }

    bool isAttacking() const
      { return brain->getOutput( ATTACK ) >= 0.5 ? true : false; }

    float getOutput(enum OutputNames output) const
      { return brain->getOutput(output); }

  protected:
    RobotDna *_dna;
    Brain *brain;
    unsigned int _attackPwr, _defensePwr;
    unsigned int _maxSpeed;
    unsigned int _hp, _maxHP;
    unsigned int _killCount, _hitCount, _shotCount;
    float _energy, _powerRequired;
    // Color color;
    //unsigned int id;
    float _maxEnergy;
    friend class RobotArena;
};

#endif /* ROBOT_H */
