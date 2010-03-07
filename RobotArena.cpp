#include "Robot.h"
#include "RobotArena.h"
#include "Vector2d.h"
#include <allegro.h>
#include <iostream>

RobotArena::RobotArena( unsigned int w, unsigned int h ) : deltaTime(0.1), width( w ), height( h )
{
  if( w == 0 )
    width = DEFAULT_WIDTH;

  if( h == 0 )
    height = DEFAULT_HEIGHT;

  robotBmp = create_bitmap(ROBOT_WIDTH, ROBOT_HEIGHT);

  if( robotBmp == NULL )
    exit(1);

  arenaBmp = create_bitmap(width, height);

  if( arenaBmp == NULL )
  {
    destroy_bitmap(robotBmp);
    exit(1);
  }

  bulletBmp = create_bitmap(BULLET_WIDTH, BULLET_HEIGHT);

  if( bulletBmp == NULL )
  {
    destroy_bitmap(arenaBmp);
    destroy_bitmap(robotBmp);
    exit(1);
  }

  clear_to_color(robotBmp, makecol(255,255,255));
  clear_to_color(bulletBmp, makecol(255,255,255));

  for( int i=0; i <= 1; i++ )
  {
    for( int j=0; j <= 1; j++ )
    {
      putpixel(robotBmp, 0 + i*(10 - 2*0), 0 + j*(10-2*0), makecol(0,0,0));
      putpixel(robotBmp, 1 + i*(10 - 2*1), 0 + j*(10-2*0), makecol(0,0,0));
      putpixel(robotBmp, 2 + i*(10 - 2*2), 0 + j*(10-2*0), makecol(0,0,0));
      putpixel(robotBmp, 3 + i*(10 - 2*3), 0 + j*(10-2*0), makecol(0,0,0));
      putpixel(robotBmp, 0 + i*(10 - 2*0), 1 + j*(10-2*1), makecol(0,0,0));
      putpixel(robotBmp, 1 + i*(10 - 2*1), 1 + j*(10-2*1), makecol(0,0,0));
      putpixel(robotBmp, 0 + i*(10 - 2*0), 2 + j*(10-2*2), makecol(0,0,0));
      putpixel(robotBmp, 0 + i*(10 - 2*0), 3 + j*(10-2*3), makecol(0,0,0));
    }
  }

  putpixel(bulletBmp, 0, 0, makecol(0,0,0));
  putpixel(bulletBmp, BULLET_WIDTH-1, 0, makecol(0,0,0));
  putpixel(bulletBmp, 0, BULLET_HEIGHT-1, makecol(0,0,0));
  putpixel(bulletBmp, BULLET_WIDTH-1, BULLET_HEIGHT-1, makecol(0,0,0));
}

RobotArena::~RobotArena()
{
  destroy_bitmap(robotBmp);
  destroy_bitmap(arenaBmp);
  destroy_bitmap(bulletBmp);

  /*
  while( team1.size() )
  {
    delete team1.back();
    team1.pop_back();
  }

  while( team2.size() )
  {
    delete team2.back();
    team2.pop_back();
  }*/
}

void RobotArena::clearAll()
{
  robots.clear();
  deadRobots.clear();
  bullets.clear();

  clear_to_color( screen, makecol(0,0,0) );
}

void RobotArena::addRobot( Robot &robot, unsigned int x, unsigned int y )
{
  x = (x >= width ? width - 1 : x);
  y = (y >= height ? height - 1 : y);

  robot.setPosition(Vector2d(x,y));
  robots.push_back(&robot);
}

void RobotArena::updateRobotPos( Robot &robot, const Vector2d &v )
{
  float x, y;

  Vector2d pos = robot.getPosition() + v * deltaTime;

  x = pos.getX();
  y = pos.getY();
  pos.setX( x < ROBOT_WIDTH / 2 ? ROBOT_WIDTH / 2 : (x > width - 1 - ROBOT_WIDTH / 2? width - 1 - ROBOT_WIDTH / 2: x) );
  pos.setY( y < ROBOT_WIDTH / 2 ? ROBOT_WIDTH / 2 : (y > height - 1 - ROBOT_WIDTH / 2? height - 1 - ROBOT_WIDTH / 2: y) );
  robot.setPosition(pos);

  if( robot.getEnergy() > 0 && (pos.getX() == ROBOT_WIDTH / 2 || 
      pos.getX() == width - 1 - ROBOT_WIDTH / 2
      || pos.getY() == ROBOT_WIDTH / 2 || pos.getY() == height - 1 - ROBOT_WIDTH / 2) )
  {
    robot.setEnergy(robot.getEnergy()-1);
  }
}

Robot *RobotArena::getNearestRobot( const Robot &robot )
{
  Robot *nearest = NULL;
  float nearestDist, dist;
  Robot *_tRobot;

  for( unsigned i=0; i < robots.size(); i++ )
  {
    _tRobot = robots[i];

    dist = (_tRobot->getPosition() - robot.getPosition()).magnitude();

    if( (dist < nearestDist || nearest == NULL) && _tRobot != &robot )
    {
      nearestDist = dist;
      nearest = _tRobot;
    }
  }

  return nearest;
}

void RobotArena::doThink()
{
  struct Bullet *bullet;
  size_t coll_index;
  double t0, t1, k, coll_time;
  Vector2d a, b;
  Robot *coll_robot;

  for( unsigned i=0; i < bullets.size(); i++ )
  {
    coll_robot = NULL;

    for( unsigned j=0; j < robots.size(); j++ )
    {
      if( robots[j] == bullets[i].shooter )
        continue;

      a = robots[j]->getPosition() - bullets[i].position;
      b = robots[j]->getVelocity() - bullets[i].velocity;
/*
      std::cout << "Robot Position: " << robots[j]->getPosition().str();
      std::cout << " Bullet Position: " << bullets[i].position.str() << std::endl;
      std::cout << "Robot Velocity: " << robots[j]->getVelocity().str();
      std::cout << " Bullet Velocity: " << bullets[i].velocity.str() << std::endl;
*/
      k = (a*b)*(a*b) - 4.0 * b*b*(a*a - (BULLET_WIDTH+ROBOT_WIDTH)*(BULLET_WIDTH+ROBOT_WIDTH)/4.0);

      if( (b*b) == 0.0 )
      {
        if( (a-b).magnitude() <= (ROBOT_WIDTH+BULLET_WIDTH) / 2.0 )
          t0 = t1 = 0.0;
        else
          continue;
      }
      else if( k < 0.0 )
        continue;
      else
      {
        t0 = (-(a * b) + sqrt( (a*b)*(a*b) - 4 * b*b*(a*a - (BULLET_WIDTH+ROBOT_WIDTH)*(BULLET_WIDTH+ROBOT_WIDTH)/4) )) / (b*b*b*b);
        t1 = (-(a * b) - sqrt( (a*b)*(a*b) - 4 * b*b*(a*a - (BULLET_WIDTH+ROBOT_WIDTH)*(BULLET_WIDTH+ROBOT_WIDTH)/4) )) / (b*b*b*b);
      }

//      std::cout << "k: " << k << std::endl;

      if( !((t0 < 0 && t1 < 0) || (t0 >= deltaTime && t1 >= deltaTime) ) )
      {
/*
        std::cout << "Collision! t0: " << t0 << "t1: " << t1 << " k: " << k << std::endl;
        std::cout << "a: " << robots[j]->getPosition().str() << " b: " << robots[j]->getVelocity().str() << std::endl;
        std::cout << "a: " << bullets[i].position.str() << " b: " << bullets[i].velocity.str() << std::endl;*/
        t0 = (t0 < 0.0 ? 0.0 : t0);
        t1 = (t1 < 0.0 ? 0.0 : t1);

        if( coll_robot == NULL || (t0 < t1 ? t0 : t1) < coll_time )
        {
          coll_index = j;
          coll_robot = robots[j];
          coll_time = (t0 < t1 ? t0 : t1);
        }
      }
    }

    if( coll_robot != NULL )
    {
   //   std::cout << "Collision" << std::endl;
      bullets[i].shooter->increaseHitCount();
/*
      if( coll_robot->getHP() <= 4.5 * bullets[i].shooter->getAttackPower() / (1 + coll_robot->getDefensePower() ) )
      {
        coll_robot->doDamage( coll_robot->getHP() );
      }
      else
      {
        coll_robot->doDamage( 4.5 * bullets[i].shooter->getAttackPower() / (1 + coll_robot->getDefensePower() ) );
      }
*/
      coll_robot->doDamage(coll_robot->getHP());
      if( coll_robot->getHP() == 0 )
      {
        bullets[i].shooter->increaseKillCount();
        coll_robot->setEnergy(0.0);  // XXX: Remove the bot from the arena
        deadRobots.push_back( coll_robot );
        robots.erase( robots.begin() + coll_index, robots.begin() + coll_index + 1);
      }

      //bullet = &bullets[i];
      bullets.erase( bullets.begin() + i, bullets.begin() + i + 1 );
    }
  }

  for( unsigned i=0; i < robots.size(); i++ )
  {
    robots[i]->think();

    if( robots[i]->getEnergy() < robots[i]->getRequiredPower() )
    {
      robots[i]->setEnergy(0);
      robots.erase( robots.begin() + i, robots.begin() + i + 1 );
      deadRobots.push_back( robots[i] );
      i--;
    }
  }

  for( unsigned i=0; i < robots.size(); i++ )
    updateRobotPos( *robots[i], robots[i]->getVelocity() );

  for( unsigned i=0; i < bullets.size(); i++ )
  {
    bullets[i].position = bullets[i].position + bullets[i].velocity * deltaTime;

    if( bullets[i].position.getX() < 0 || bullets[i].position.getY() < 0 )
    {
      bullet = &bullets[i];
      //bullets[i] = bullets[len-1];
      bullets.erase(bullets.end(), bullets.end());
   //   delete bullet;
    }
  }

/* If a robot is attacking, spawn a bullet */

  for( unsigned i=0; i < robots.size(); i++ )
  {
    if( robots[i]->isAttacking() && robots[i]->getVelocity().magnitude() > 0.0 )
    {
      bullet = new struct Bullet;
      bullet->velocity = robots[i]->getVelocity().normalize() * BULLET_SPEED;
      bullet->position = robots[i]->getPosition() + robots[i]->getVelocity().normalize() * ((ROBOT_WIDTH/2.0)+(BULLET_WIDTH));
      bullet->shooter = robots[i];
      bullet->shooter->increaseShotCount();
      bullets.push_back(*bullet);
      delete bullet;
    } 
  }
}
/*
void RobotArena::drawRobot( unsigned x, unsigned y )
{

}
*/
void RobotArena::render()
{
  Vector2d v;

  clear_bitmap(arenaBmp);

  for( unsigned i=0; i < robots.size(); i++ )
  {
    v = robots[i]->getPosition();

    blit(robotBmp, arenaBmp, 0, 0, v.getX()-ROBOT_WIDTH/2, v.getY()-ROBOT_HEIGHT/2, ROBOT_WIDTH, ROBOT_HEIGHT);
  }

  for( unsigned i=0; i < bullets.size(); i++ )
  {
    struct Bullet* b;
    b = &bullets[i];
    blit(bulletBmp, arenaBmp, 0, 0, b->position.getX(), b->position.getY(), BULLET_WIDTH, BULLET_HEIGHT);
  }

  blit(arenaBmp, screen, 0, 0, 0, 0, width, height);
}
