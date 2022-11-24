#define MANUAL_MODE 0
#define AUTONOMOUS_MODE 1

void _init();
void inputTarget();
void checkCurrentState();
void calcInverseKinematics();
void move2target();
void driveMotor(int motorID);
void keyInput();
void switchMode();

int STATE = MANUAL_MODE;
double motorPos[3] = {0,0,0};

void setup(){
    _init();
}

void loop(){
    switch (STATE) {
    case MANUAL_MODE:
        int targetMotor = keyInput();
        if (switchMode(MANUAL_MODE))
            STATE = AUTONOMOUS_MODE;
        driveMotor(targetMotor);
    break;

    case AUTONOMOUS_MODE:
        inputTarget();
        if (switchMode(AUTONOMOUS_MODE))
            STATE = MANUAL_MODE;
        checkCurrentState();
        calcInverseKinematics();
        move2target();
    break;
}