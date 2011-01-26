#include "Brain.h"
#include "RobotDna.h"
#include "RobotArena.h"
#include "Robot.h"

#include <iostream>
#include <cstdlib>
#include <allegro.h>
#include <cmath>

#define NUM_ROBOTS	5
#define max(a,b)	(a > b ? a : b)

unsigned score[NUM_ROBOTS];
Robot *robots[NUM_ROBOTS];
Robot *robots2[NUM_ROBOTS];

unsigned maxKills, bestScore;
float maxAccuracy, averageScore;


void pickTwoRobots( Robot **robot1, Robot **robot2 )
{
  unsigned totalScore = 0;
  unsigned chosen, firstIndex, accum=0;

  if( robot1 == NULL || robot2 == NULL )
    return;

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
    totalScore += score[i];

  chosen = (unsigned)rand() % totalScore;

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    if( chosen >= accum && chosen < accum + score[i] )
    {
      firstIndex = i;
      *robot1 = robots[i];
      break;
    }

    accum += score[i];
  }

  totalScore = 0;
  accum = 0;

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    if( i == firstIndex )
      continue;

    totalScore += score[i];
  }

  chosen = (unsigned)rand() % totalScore;

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    if( i == firstIndex )
      continue;

    if( chosen >= accum && chosen < accum + score[i] )
    {
      *robot2 = robots[i];
      break;
    }
    accum += score[i];
  }
}

unsigned numRobotsLeft()
{
  unsigned count = 0;

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    if( robots[i]->energy() >= robots[i]->requiredPower() )
      count++;
  }

  return count;
}

bool allRobotsDead()
{
  return numRobotsLeft() == 0 ? true : false;
}

void increaseScores()
{
  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    if( robots[i]->energy() > 0 )
      ;//score[i]++;
  }
}

void addKills()
{
  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    maxKills = max( robots[i]->killCount(), maxKills );

    score[i] += robots[i]->killCount() * 100;

    if( robots[i]->shotCount() > 0 )
    	maxAccuracy = fmax(maxAccuracy, robots[i]->hitCount() / (double)robots[i]->shotCount());

    if( robots[i]->shotCount() > 0 )
      score[i] += 50*pow( 3, 6 * (robots[i]->hitCount() / (double)robots[i]->shotCount()) );

    if( robots[i]->hitCount() == 0 && robots[i]->shotCount() > 0 )
      score[i] -= 0.1 * robots[i]->shotCount();
  }
}

void calcScoreStats()
{
  unsigned int total=0;

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
  {
    bestScore = max( bestScore, score[i] );
    total += score[i];
  }

  averageScore = (float)total / NUM_ROBOTS;
}

void resetScores()
{
  for( unsigned i=0; i < NUM_ROBOTS; i++ )
    score[i] = 300;
}

int main(int argc, char *argv[])
{
  unsigned generation=0;
  allegro_init();
 // install_keyboard();
  set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
  RobotArena arena = RobotArena( 640, 480 );
//  Robot *test = new Robot(NULL, &arena);
  //srand( time(NULL) );

  robots[0] = new Robot("dna/robo_dna5.dna", &arena );
  robots[1] = new Robot("dna/robo_dna6.dna", &arena );
  robots[2] = new Robot("dna/robo_dna7.dna", &arena );
  robots[3] = new Robot("dna/robo_dna8.dna", &arena );
  robots[4] = new Robot("dna/robo_dna9.dna", &arena );
//  robots[5] = new Robot("dna/robo_dna5.dna", &arena );
//  robots[6] = new Robot("dna/robo_dna6.dna", &arena );
//  robots[7] = new Robot("dna/robo_dna7.dna", &arena );
//  robots[8] = new Robot("dna/robo_dna8.dna", &arena );
//  robots[9] = new Robot("dna/robo_dna9.dna", &arena );

/*
  robots[11] = new Robot("dna/robo_dna1.dna", &arena );
  robots[12] = new Robot("dna/robo_dna2.dna", &arena );
  robots[13] = new Robot("dna/robo_dna3.dna", &arena );
  robots[14] = new Robot("dna/robo_dna4.dna", &arena );
  robots[15] = new Robot("dna/robo_dna5.dna", &arena );
  robots[16] = new Robot("dna/robo_dna6.dna", &arena );
  robots[17] = new Robot("dna/robo_dna7.dna", &arena );
  robots[18] = new Robot("dna/robo_dna8.dna", &arena );
  robots[19] = new Robot("dna/robo_dna9.dna", &arena );
  robots[10] = new Robot("dna/robo_dna0.dna", &arena );

  for( unsigned i=10; i < NUM_ROBOTS; i++ )
  {
    for( unsigned j=0; j < 1000; j++ )
      robots[i]->dna()->mutate();
  }
*/
  for( unsigned i=0; i < NUM_ROBOTS; i++ )
    arena.addRobot(*robots[i], (unsigned)rand() % arena.getWidth(), (unsigned)rand() % arena.getHeight() );

  RobotDna *childDna;
  Robot *robot1, *robot2;

  bestScore = averageScore = maxKills = maxAccuracy = 0;

  while(1)
  {
    bool bonusAwarded = false;

    resetScores();

    std::cout << "Generation: " << ++generation << std::endl;
    bestScore = averageScore = maxKills = maxAccuracy = 0;

    while( !allRobotsDead() )
    {
      bool oneLeft = false;

      arena.render();
      arena.doThink();

      if( numRobotsLeft() == 1 && bonusAwarded == false )
      {
	bonusAwarded = true;
        for( unsigned i=0; i < NUM_ROBOTS; i++ )
        {
          if( robots[i]->energy() >= robots[i]->requiredPower() )
          {
            score[i] += 100;
            oneLeft = true;
            break;
          }
        }
      }

      if( oneLeft )
        break;

      sleep(arena.deltaTime);
      increaseScores();
    }

    addKills();

    for( unsigned i=0; i < NUM_ROBOTS; i++ )
    {
      if( robots[i]->killCount() == 0 )
        score[i] -= 40;

      if( robots[i]->hitCount() == 0 )
        score[i] -= 150;
    }

    arena.clearAll();
    calcScoreStats();

    /* Pick two robots and create a single one until all robots are regenerated */

    for( unsigned i=0; i < NUM_ROBOTS; i++ )
    {
      pickTwoRobots( &robot1, &robot2 );

      robot1->dna()->crossover( *robot2->dna(), &childDna, NULL );

      if( bestScore == 0 )
      {
        for( unsigned j=0; j < 300; j++ )
          childDna->mutate();
      }
      else if( abs( bestScore - averageScore ) < 50 )
      {
        for( int j=0; j < 5; j++ )
          childDna->mutate();
      }

      robots2[i] = new Robot( *childDna, &arena );

      delete childDna;
      childDna = NULL;
    }

    for( unsigned i=0; i < NUM_ROBOTS; i++ )
    {
      delete robots[i];
      robots[i] = robots2[i];
      robots2[i] = NULL;
    }

  for( unsigned i=0; i < NUM_ROBOTS; i++ )
    arena.addRobot(*robots[i], (unsigned)rand() % arena.getWidth(), (unsigned)rand() % arena.getHeight() );

    std::cout << "Most kills: " << maxKills << " Best accuracy: " << maxAccuracy << std::endl;
    std::cout << "Best score: " << bestScore << " Average score: " << averageScore << std::endl;
  }

  return 0;
}
END_OF_MAIN()
