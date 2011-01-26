#ifndef ARENA_OBJ
#define ARENA_OBJ

#include "RobotArena.h"
#include "Vector2d.h"

class ArenaObject
{
  public:
    ArenaObject( RobotArena *arena=NULL ) : robotArena(arena) {}
    virtual void think() = 0;

    void setPosition( const Vector2d &pos ) { _position = pos; }
    void setVelocity( const Vector2d &vel ) { _velocity = vel; }

    RobotArena *arena() const { return robotArena; }
    Vector2d position() const { return _position; }
    Vector2d velocity() const { return _velocity; }
  protected:
    Vector2d _velocity, _position;
    RobotArena *robotArena;
};

#endif /* ARENA_OBJ */
