#ifndef CAR_H
#define CAR_H
#include <cstdlib>
#include <unistd.h>
#include <math.h>
#include <array>
#define MAP_SIZE 2000
using namespace std;
typedef  array<double,3> Point;

class Car{
public:
	Point pos;
	Point prev;
	Point speed;
 	double time;
    double getRand(int off,int limit){
    	return (rand()*(off+1)) % limit;
    }
	int rank;
	Car(){}
	Car(int mpi_rank){
		rank=mpi_rank;
		rePos();
		prev=pos;
		reSpeed();
		this->time=0;
	}
	void rePos() {
		pos={getRand(rank,50),getRand(rank,50),0};
	}
	void reSpeed() {
		speed={getRand(rank,15),getRand(rank,15),1};
	}
	Point move(){
		prev = pos;
		this->time += 1;
		pos[0] += speed[0];
		pos[1] += speed[1];
		if((pos[0]>MAP_SIZE) || (pos[1]>MAP_SIZE)){
			rePos();
			prev = pos;
		}
		pos[2] = this->time;
		return pos;
	}
};
#endif  //CAR_H
