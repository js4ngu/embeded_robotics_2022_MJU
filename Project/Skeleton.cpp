#include <stdio.h>
#include <math.h>
#define L1 10
#define L2 10

double* cartsia2cylinder(double x, double y, double z);
double findTheta2(double r, double z);
double findTheta1(double r, double z, double th2);
double cartsianCord[3];
double* cylinderCord; 

int main(){
    scanf("%lf %lf %lf", &cartsianCord[0], &cartsianCord[1], &cartsianCord[2]);
    cylinderCord = cartsia2cylinder(cartsianCord[0], cartsianCord[1], cartsianCord[2]);
    printf("TransCord");
    for (int i = 0; i < 3; i++){
        printf("%lf\t", cylinderCord[i]);
    }
    double theta2 = findTheta2(cylinderCord[0],cylinderCord[2]);
    double theta1 = findTheta1(cylinderCord[0],cylinderCord[2],theta2);
    printf("\nTheta : %lf \t %lf\n",theta2, theta1);
    return 0;
}

double* cartsia2cylinder(double x, double y, double z){
    double r, pi;
    double cord[3];
    pi = atan2(y,x) * 57.2958;
    r = sqrt(x*x + y*y);
    cord[0] = r;
    cord[1] = pi;
    cord[2] = z;
    return cord;
}

double findTheta2(double r, double z){
    double innerSqrt = ((L1 + L2)*(L1 + L2) - (r*r + z*z))/
                        ((r*r + z*z) - (L1 - L2)*(L1 - L2));
    return 2*atan(sqrt(innerSqrt))* 57.2958;
}

double findTheta1(double r, double z, double th2){
    double firstTerm = atan2(z,r)*57.2958;
    double secondTerm = atan2(L2*sin(th2),(L1 + L2*cos(th2)))* 57.2958;
    return firstTerm - secondTerm;
}