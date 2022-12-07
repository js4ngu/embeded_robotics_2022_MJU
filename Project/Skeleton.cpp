#include <stdio.h>
#include <math.h>
#define L1 10
#define L2 10
#define POTMODE 0
#define INVERSEKINEMATIC 1
#define POT_X 0
#define POT_Y 1

void cartsia2cylinder(double x, double y, double _z);
double findTheta2(double r, double z);
double findTheta1(double r, double z, double th2);
void findInverseKinematic(double x, double y, double z);
void potRead();
void potVal2motorDeg();
bool modeSW();
void FSM(bool mode);
void InverseKinematicMode();
void potMode();

double cartsianCord[3];
double r, pi, z;
double motorDeg[3];
int potVal[2];
bool SW;

int main(){
    FSM(modeSW());
}

void FSM(bool mode){
    if (mode == INVERSEKINEMATIC) {
        InverseKinematicMode();
    }
    else if (mode == POTMODE) {
        potMode();
    }
}

void InverseKinematicMode(){
    scanf("%lf %lf %lf", &cartsianCord[0], &cartsianCord[1], &cartsianCord[2]);
    findInverseKinematic(cartsianCord[0], cartsianCord[1], cartsianCord[2]);
    for (size_t i = 0; i < 3; i++)
        printf("%lf\t", motorDeg[i]);
}

void potMode(){
    potRead();
    potVal2motorDeg();
}

bool modeSW(){
    if (digiatlRead(6) == 1)
        return POTMODE;
    else
        return INVERSEKINEMATIC;
}

void potRead(){
    SW = digitalRead(A5);
    potVal[POT_X] = analogRead(A4);
    potVal[POT_Y] = analogRead(A4);
}

void potVal2motorDeg(){
    for (int i = 0; i < 3; i++) {
        motorDeg[i] = map(potVal[i], 0, 1024, 0, 360);
    }
    for (size_t i = 0; i < 3; i++)
        printf("%lf\t", motorDeg[i]);
}

void findInverseKinematic(double x, double y, double z){
    cartsia2cylinder(x, y, z);
    double theta2 = findTheta2(r,z);
    double theta1 = findTheta1(r,z,theta2);
    motorDeg[0] = pi;
    motorDeg[1] = theta1;
    motorDeg[2] = theta2;
}

void cartsia2cylinder(double x, double y, double _z){
    double cord[3];
    pi = atan2(y,x) * 57.2958;
    r = sqrt(x*x + y*y);
    z = _z;
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