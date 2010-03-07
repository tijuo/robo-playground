#ifndef ROBOT_H
#define ROBOT_H

#include "RobotDna.h"
#include "Brain.h"
#include "RobotArena.h"
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

class Robot
{
  public:
    Robot() : dna(NULL), robotArena(NULL), maxEnergy(initialEnergy) { }
    Robot(const char *filename, RobotArena * const arena );
    Robot( const Robot &robot );
    Robot(const RobotDna &dna, RobotArena * const arena );

    Robot& operator=(const Robot &rhs) 
    { 
      this->dna = new RobotDna(*rhs.dna);
      this->brain = new Brain(*rhs.brain);
      this->velocity = new Vector2d(*rhs.velocity);
      this->position = new Vector2d(*rhs.position);

      this->attackPwr = rhs.attackPwr;
      this->defensePwr = rhs.defensePwr;
      this->maxSpeed = rhs.maxSpeed;
      this->hp = rhs.hp;
      this->maxHP = rhs.maxHP;
      this->powerRequired = rhs.powerRequired;
      this->robotArena = rhs.robotArena;
      this->maxEnergy = rhs.maxEnergy;

      return *this;
    }

    ~Robot();

    void think();
    void setEnergy( float val ) { energy = val; }

    void setPosition( const Vector2d &pos ) { *position = pos; }
    Vector2d getPosition() const { return *position; }
    Vector2d getVelocity() const { return *velocity; }
    RobotDna *getDna() const { return dna; }
    Brain getBrain() const { return *brain; }
    unsigned int getAttackPower() const { return attackPwr; }
    unsigned int getDefensePower() const { return defensePwr; }
    unsigned int getHP() const { return hp; }
    unsigned int getMaxHP() const { return maxHP; }
    unsigned int getMaxSpeed() const { return maxSpeed; }
    unsigned int getKillCount() const { return killCount; }
    float getRequiredPower() const { return powerRequired; }
    float getEnergy() const { return energy; }
    void increaseKillCount( ) { killCount++; }
    void increaseHitCount() { hitCount++; }
    void increaseShotCount() { shotCount++; }
    unsigned int getShotCount() const { return shotCount; }
    void doDamage( unsigned int damage ) { hp -= (damage >= hp ? hp : damage); }
    unsigned int getHitCount() const { return hitCount; }
   // float getTurretRot() const { return turretRot; }
   // unsigned int getId() const { return id; }
    //void setId( unsigned _id ) { id = _id; }

    bool isAttacking() const 
    {
      return brain->getOutput( ATTACK ) >= 0.5 ? true : false; 
    }

    float getOutput(enum OutputNames output) const { return brain->getOutput(output); }

  private:
    RobotDna *dna;
    Brain *brain;
    Vector2d *velocity, *position;
    unsigned int attackPwr, defensePwr;
    unsigned int maxSpeed;
    unsigned int hp, maxHP;
    unsigned int killCount, hitCount, shotCount;
    float energy, powerRequired;
    RobotArena *robotArena;
    // Color color;
    //unsigned int id;
    float maxEnergy;
    friend class RobotArena;
};

#endif /* ROBOT_H */
