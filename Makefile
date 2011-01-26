SRC	=Brain.cpp InputNeuron.cpp main.cpp Neuron.cpp Robot.cpp \
	RobotArena.cpp RobotDna.cpp Vector2d.cpp
LIBS	=-lalleg -lm -lXxf86vm -lXpm -lXcursor -lpthread

all: playground

playground: $(SRC)
	g++ -Wall -g $(SRC) -o $@ $(LIBS)

clean:
	rm -f playground
