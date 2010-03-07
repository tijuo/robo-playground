#ifndef ROBOT_ARENA
#define ROBOT_ARENA

#include "Vector2d.h"
#include <allegro.h>
#include <cstdlib>
#include <vector>

#define DEFAULT_WIDTH	320
#define DEFAULT_HEIGHT	200

#define ROBOT_WIDTH	11
#define ROBOT_HEIGHT	11

#define BULLET_SPEED	100.0
#define BULLET_WIDTH	3
#define BULLET_HEIGHT	3

class Robot;

struct Bullet
{
  Vector2d position, velocity;
  Robot *shooter;
};

class RobotArena
{
  public:
    RobotArena( unsigned int w=DEFAULT_WIDTH, unsigned int h=DEFAULT_HEIGHT );
    ~RobotArena();

    void addRobot( Robot &robot, unsigned int x, unsigned int y );
    void clearAll();
    Robot *getNearestRobot( const Robot &robot );
    unsigned int getWidth() const { return width; }
    unsigned int getHeight() const { return height; }
    void updateRobotPos( Robot &robot, const Vector2d &v );
    void doThink();
    void render();
    const float deltaTime;

  private:
    std::vector<Robot *> robots;
    std::vector<Robot *> deadRobots;
    std::vector<struct Bullet> bullets;
    unsigned int width, height;
    BITMAP *arenaBmp;
    BITMAP *robotBmp;
    BITMAP *bulletBmp;
};

#endif /* ROBOT_ARENA */
