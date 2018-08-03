#include "car.h"
#include "distance.h"

using MPI::COMM_WORLD; using MPI::ANY_SOURCE; using MPI::ANY_TAG;
using MPI::INT; using MPI::CHAR; using MPI::BOOL; using MPI::DOUBLE;
using MPI::Status;
#define TAG_NEW_POSITION 10
using namespace std;
Car car;
int total_nodes,mpi_rank;

//TODO: Measure broadcast time
//TODO: See what happens when we decide randomly wich cars to send the nodes
//TODO: Maybe i'm going crazy and this is too much, help!(?)
void broadcast(Point p){
    //Do not send to myself, start by the next node
    int dest;
    for (int i = 1; i < total_nodes; i++) {
        dest = (mpi_rank+i)%total_nodes;
        MPI_Send(&p, 3, MPI_DOUBLE, dest, TAG_NEW_POSITION, MPI_COMM_WORLD);
    }
}
//Thread function that moves the FUCKING CAR :D
void* moveCar(void *ptr){
    Point pos;
	while (true) {
		pos = car.move();
		broadcast(pos);
		sleep(1);
  	}
}
int runCar(){
    //Reserves the position of every node/car in the map
	Point pos[total_nodes];
	car = Car(mpi_rank);

    //Start moving in parallel so stoping is not an option
	pthread_t movicion;//Change this var name properly
	pthread_create(&movicion, NULL, moveCar, NULL);

	MPI_Status status;
	Point bufferPoint;
	while (true) {
        //TODO: search any useful tag other than Position
		MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		MPI_Recv(&bufferPoint, 3, MPI_DOUBLE, status.MPI_SOURCE,
             TAG_NEW_POSITION, MPI_COMM_WORLD, &status);
        double dist = distance(car.pos, bufferPoint);
		Line l1(car.prev, car.pos);
		Line l2(pos[status.MPI_SOURCE], bufferPoint);
        double col;
        if ( dist <= 10 ){
        //TODO:Log collision without losing performance
            car.choques++;
            printf("[%d] COLISIONA! con [%d] dist=%f - %d\n",
                mpi_rank, status.MPI_SOURCE, dist,car.choques);
        }else if ((col  = colide(l1, l2)) && col <= 10 && -1 < col ){
            car.newSpeed();
            printf("[%d] COLISIONARA con [%d] dist=%f\n",
            mpi_rank, status.MPI_SOURCE, col);
        }
		pos[status.MPI_SOURCE]=bufferPoint;
	}
	return 0;
}
int main(int argc, char **argv){
	srand (time(NULL));
	/*Init MPI with threading support
	**TODO:Test thread support for sending and recieving concurrenctly
	*/
  	int provided;
	int initStatus= MPI_Init_thread( &argc, &argv, MPI_THREAD_SERIALIZED, &provided );
	if (initStatus != MPI_SUCCESS){
		fprintf(stderr, "MPI Error al inicializar.\n");
		MPI_Abort(MPI_COMM_WORLD, initStatus);
	}
    //Load MPI node values
	MPI_Comm_size(MPI_COMM_WORLD, &total_nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	runCar();
	MPI_Finalize();
    return 0;
}
