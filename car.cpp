#include <mpi.h>
#include <pthread.h>

#include "car.h"
#include "distance.h"


using MPI::COMM_WORLD; using MPI::ANY_SOURCE; using MPI::ANY_TAG;
using MPI::INT; using MPI::CHAR; using MPI::BOOL; using MPI::DOUBLE;
using MPI::Status;
#define TAG_NEW_BLOCK 10
using namespace std;
Car car;
int total_nodes,mpi_rank;

void broadcast(Point p){
    //Do not send to myself, start by the next node 
    int dest;
    for (int i =1; i <total_nodes; i++) {
        dest = (mpi_rank+i)%total_nodes;
        MPI_Send(&p, 3, MPI_DOUBLE, dest, TAG_NEW_BLOCK, MPI_COMM_WORLD);
    }
}
void* moverse(void *ptr){
    Point pos;
	while (true) {
		pos=car.move();
		broadcast(pos);
		sleep(1);
  	}
}
int runCar(){
	Point pos[total_nodes];
	car = Car(mpi_rank);
	pthread_t movicion;
	pthread_create(&movicion, NULL, moverse, NULL);
	MPI_Status status;
	Point bufferPoint;
	while (true) {
		MPI_Probe(MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);
		MPI_Recv(&bufferPoint, 3, MPI_DOUBLE,  status.MPI_SOURCE, TAG_NEW_BLOCK, MPI_COMM_WORLD, &status);
		Line l1(car.prev,car.pos);
		Line l2(pos[status.MPI_SOURCE],bufferPoint);
		double dist=distance(l1,l2);
        if ( dist <=10 && -1<dist){
            car.reSpeed();
            printf("[%d]COLISIONA con [%d] dist=%f\n",mpi_rank,status.MPI_SOURCE,dist);
        }
		pos[status.MPI_SOURCE]=bufferPoint;
		// printf("[%d]Estoy a %f de %d\n",mpi_rank,dist,status.MPI_SOURCE);
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
	MPI_Comm_size(MPI_COMM_WORLD, &total_nodes);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
	runCar();
	MPI_Finalize();
    return 0;
}
