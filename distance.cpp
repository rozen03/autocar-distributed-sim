#include "distance.h"


//la puta norma!
inline double norm(double x,double y, double z){
	return sqrt(pow(x,2)+ pow(y,2)+pow(z,2));
}

//I guess this works aprox
double distance(Line line1, Line line2) {
    	double x01 = line1.x0;
    	double x02 = line2.x0;
    	double y01 = line1.y0;
    	double y02 = line2.y0;
    	double z01 = line1.z0;
    	double z02 = line2.z0;
		double x11 = line1.x1;
		double x12 = line2.x1;
		double y11 = line1.y1;
		double y12 = line2.y1;
		double z11 = line1.z1;
		double z12 = line2.z1;
		double dir1_x=x11-x01;
		double dir1_y=y11-y01;
		double dir1_z=z11-z01;
		double dir2_x=x12-x02;
		double dir2_y=y12-y02;
		double dir2_z=z12-z02;
		double cross_x=dir1_x*dir2_x;
		double cross_y=dir1_y*dir2_y;
		double cross_z=dir1_z*dir2_z;
		//if we get cero, the two directions are parallel, somehow...
		double crossNorm= norm(cross_x,cross_y,cross_z);
		if(crossNorm==0.0){return -1.0;}
		//normalize product
		cross_x /= crossNorm;
		cross_y /= crossNorm;
		cross_z /= crossNorm;
		double diff_x= x01-x02;
		double diff_y= y01-y02;
		double diff_z= z01-z02;
		double prod= cross_x*diff_x + cross_y*diff_y+cross_z*diff_z;
		double dist = abs(prod);
		return dist;
}
