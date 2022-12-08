#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define L1 35
#define L2 35
#define BUFFER_SIZE 10
#define TX_Enable_pin            12  // 송신 활성화 핀
#define RX_Enable_pin            13  // 수신 활성화 핀
#define POTMODE                  0
#define INVERSEKINEMATICMODE     1
#define MOTOR_TEST_MODE          2
#define MOTOR_INIT_MODE          3
#define POT_BOTTOM_ID            0
#define POT_MIDDLE_ID            1
#define POT_TOP_ID               100
#define POT_BOTTOM               0
#define POT_MIDDLE               1
#define POT_TOP                  2
#define ERROR                    -1
typedef enum _com_mode {
    RX_MODE,
    TX_MODE
} com_mode;

// 통신 모드(송/수신)에 따른 버퍼칩 설정
void set_com_mode(com_mode mode);
// 통신 프로토콜에 체크섬 삽입
void dx_insert_checksum_byte(unsigned char *packet);
// id 설정을 위한 통신 패킷 조립
void dx_set_id(unsigned char id);
// 제어 모드 설정
// 정방향과 역방향 최대 각도에 따라 바퀴모드나 관절모드로 설정됨
// 바퀴모드: 모두 0으로 설정
// 관절모드: 0~360(0xfff)로 설정
void dx_set_control_mode(unsigned char id, unsigned char cw_angle_limit[2], unsigned char ccw_angle_limit[2]);
// 위치제어용 패킷 조립
void dx_tx_packet_for_position_control(unsigned char id, unsigned char goal_pos[2]);
int readModeSW();
void potRead();
void cartsia2cylinder(float x, float y, float _z);
float findTheta2(float r, float z);
float findTheta1(float r, float z, float th2);
void findInverseKinematic(float x, float y, float z);
bool toggleSW();
unsigned char topPacket(int REF);
unsigned char bottomPacket(int REF);

unsigned char ref_0[2];
unsigned char ref_1[2];
unsigned char ref_2[2];
int potVal[3];
float cartsianCord[3];
float r, pi, z;
int motorDeg[3];
char buf[BUFFER_SIZE];
int stateMainFSM;

void setup() {
    Serial.begin(57600);  // 통신 속도
    pinMode(TX_Enable_pin, OUTPUT); //TX Enable
    pinMode(RX_Enable_pin, OUTPUT); //RX Enable
    pinMode(2, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, INPUT);
    pinMode(10, INPUT);

    unsigned char cw_angle_limit[2];
    unsigned char ccw_angle_limit[2];

    cw_angle_limit[0] = 0x00;
    cw_angle_limit[1] = 0x00;
    ccw_angle_limit[0] = 0xFF;
    ccw_angle_limit[1] = 0xFF;  

    dx_set_control_mode(254, cw_angle_limit, ccw_angle_limit);
    digitalWrite(2, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(8, HIGH);
    delay(1);
}

void loop() {
    switch (readModeSW()) {
    case POTMODE:
        potRead();
        break;

    case INVERSEKINEMATICMODE:
        char inCom;
        static int state = 0;
        switch (state)
        {
        case 0:
            for (int i = 0; i < 3;  i++) {
                Serial.print("INPUT (since 3Sec) : \n");
                delay(3000);
                if (Serial.available()){
                    cartsianCord[i] = Serial.parseInt();
                }
            }
            while (1) {
                char inCOme;
                Serial.print("Press 'a' to start\n");
                if (Serial.available()){
                    inCOme = Serial.read();
                }
                if (inCOme == 'a') {
                    break;
                }
            }
            state = 1;
            break;
        
        case 1:
            findInverseKinematic(cartsianCord[0], cartsianCord[1], cartsianCord[2]);
            ref_0[0] = bottomPacket(motorDeg[0]);
            ref_0[1] = topPacket(motorDeg[0]);
            dx_tx_packet_for_position_control(POT_BOTTOM_ID, ref_0);
            delay(1000);
            ref_1[0] = bottomPacket(motorDeg[1]);
            ref_1[1] = topPacket(motorDeg[1]);
            dx_tx_packet_for_position_control(POT_MIDDLE_ID, ref_1);
            delay(1000);
            ref_2[0] = bottomPacket(motorDeg[2]);
            ref_2[1] = topPacket(motorDeg[2]);
            dx_tx_packet_for_position_control(POT_TOP_ID, ref_2);
            delay(1000);
            state = 2;
            break;
        case 2:
            while (1) {
                char inCOme;
                Serial.print("Press 'a' to restart\n");
                if (Serial.available()){
                    inCOme = Serial.read();
                }
                if (inCOme == 'a') {
                    state = 0;
                    break;
                }
            }
        }


    case MOTOR_TEST_MODE:
        ref_0[0] = 0x00;
        ref_0[1] = 0x10;
        dx_tx_packet_for_position_control(POT_BOTTOM_ID, ref_0);
        delay(1000);
        dx_tx_packet_for_position_control(POT_MIDDLE_ID, ref_0);
        delay(1000);
        dx_tx_packet_for_position_control(POT_TOP_ID, ref_0);
        delay(1000);
        ref_0[0] = 0x00;
        ref_0[1] = 0x00;
        dx_tx_packet_for_position_control(POT_BOTTOM_ID, ref_0);
        delay(1000);
        dx_tx_packet_for_position_control(POT_MIDDLE_ID, ref_0);
        delay(1000);
        dx_tx_packet_for_position_control(POT_TOP_ID, ref_0);
        delay(1000);
        break;

    case MOTOR_INIT_MODE:
        ref_0[0] = 0x00;
        ref_0[1] = 0x08;
        dx_tx_packet_for_position_control(POT_BOTTOM_ID, ref_0);
        delay(1000);
        dx_tx_packet_for_position_control(POT_MIDDLE_ID, ref_0);
        delay(1000);
        dx_tx_packet_for_position_control(POT_TOP_ID, ref_0);
        delay(1000);
        while(1);
    }
}

int readModeSW(){
    bool modeSW = digitalRead(9);
    if(modeSW == 0){
        digitalWrite(5, HIGH);
        return POTMODE;
    }
    else if(modeSW == 1){
        digitalWrite(5, LOW);
        return INVERSEKINEMATICMODE;
    }
    else
        return ERROR;
}

bool toggleSW(){
    static bool prev_btn;
    static bool current_btn;
    current_btn = digitalRead(10);
    if ( (prev_btn ^ current_btn) == 1){
        prev_btn = current_btn;
        Serial.println("SW");
        return 1;
    }
    else{
        prev_btn = current_btn;
        return 0;
    }
}

void potRead(){
    static int state = 0;
    switch (state)
    {
    case 0:
        while(1){
            potVal[POT_BOTTOM] = analogRead(A2)*4;
            Serial.print("POT_BOTTOM : ");
            Serial.println(potVal[POT_BOTTOM]);
            if (toggleSW() == 1) {
                state = 1;
                break;
            }
        }
        break;
    case 1:
        while(1){
            potVal[POT_MIDDLE] = analogRead(A2)*4;
            Serial.print("POT_MIDDLE : ");
            Serial.println(potVal[POT_MIDDLE]);
            if (toggleSW() == 1) {
                state = 2;
                break;
            }
        }
        break;
    case 2:
        while(1){
            potVal[POT_TOP] = analogRead(A2)*4;
            Serial.print("POT_TOP : ");
            Serial.println(potVal[POT_TOP]);
            if (toggleSW() == 1) {
                state = 3;
                break;
            }
        }
        break;
    case 3:
        Serial.print(potVal[POT_BOTTOM]);
        Serial.print("\t");
        Serial.print(potVal[POT_MIDDLE]);
        Serial.print("\t");
        Serial.println(potVal[POT_TOP]);
        state = 4;
        break;

    case 4:
        motorDeg[POT_BOTTOM] = potVal[POT_BOTTOM];
        motorDeg[POT_MIDDLE] = potVal[POT_MIDDLE];
        motorDeg[POT_TOP]    = potVal[POT_TOP];

        ref_0[0] = bottomPacket(motorDeg[0]);
        ref_0[1] = topPacket(motorDeg[0]);
        dx_tx_packet_for_position_control(POT_BOTTOM_ID, ref_0);
        delay(1000);
        ref_1[0] = bottomPacket(motorDeg[1]);
        ref_1[1] = topPacket(motorDeg[1]);
        dx_tx_packet_for_position_control(POT_MIDDLE_ID, ref_1);
        delay(1000);
        ref_2[0] = bottomPacket(motorDeg[2]);
        ref_2[1] = topPacket(motorDeg[2]);
        dx_tx_packet_for_position_control(POT_TOP_ID, ref_2);
        delay(1000);

        while (1) {
            if (toggleSW() == 1) {
                state = 0;
                break;
            }
        }    
        break;
    }
}

void set_com_mode(com_mode mode) {

  if (mode == RX_MODE)
  {
    // 비활성화 먼저 수행하여 동시에 활성화 되는 순간을 방지
    digitalWrite(TX_Enable_pin, LOW); // TX disable
    digitalWrite(RX_Enable_pin, HIGH); // RX Enable
  }
  else
  {
    // 비활성화 먼저 수행하여 동시에 활성화 되는 순간을 방지
    digitalWrite(RX_Enable_pin, LOW); // RX disable
    digitalWrite(TX_Enable_pin, HIGH); // TX Enable
  }
}
void dx_insert_checksum_byte(unsigned char *packet) {

  unsigned char i;
  unsigned char checksum_pt;
  unsigned char checksum;
  unsigned char packet_length;

  packet_length = packet[3];  // 3번 바이트에 패킷 길이가 저장되어 있음
  checksum_pt = packet_length + 3;

  checksum = 0x00;
  for (i = 2; i < checksum_pt; i++) {
    checksum += packet[i];
  }
  packet[checksum_pt] = ~checksum;

}
void dx_set_id(unsigned char id) {
  unsigned char packet[8];
  unsigned char i;

  packet[0] = 0xFF;
  packet[1] = 0xFF;
  packet[2] = 0xFE;
  packet[3] = 0x04;
  packet[4] = 0x03;
  packet[5] = 0x03;
  packet[6] = id;

  dx_insert_checksum_byte(packet);

  set_com_mode(TX_MODE); // 송신 모드로 설정
  for (i = 0; i < 8; i++) {
    Serial.write(packet[i]);
  }

  // 송신완료시까지 블록
  while ( !(UCSR0A & (1 << TXC0)) ) {
    __asm__("nop\n\t"); // no operation
  }

  set_com_mode(RX_MODE); // 수신 모드로 설정

}
void dx_set_control_mode(unsigned char id,
                         unsigned char cw_angle_limit[2],
                         unsigned char ccw_angle_limit[2]) {

  unsigned char packet[11];
  unsigned char i;

  packet[0] = 0xFF;
  packet[1] = 0xFF;
  packet[2] = id;
  packet[3] = 0x07;
  packet[4] = 0x03;
  packet[5] = 0x06;
  packet[6] = cw_angle_limit[0];
  packet[7] = cw_angle_limit[1];
  packet[8] = ccw_angle_limit[0];
  packet[9] = ccw_angle_limit[1];
  dx_insert_checksum_byte(packet);

  set_com_mode(TX_MODE); // 송신 모드로 설정
  for (i = 0; i < 11; i++) {
    Serial.write(packet[i]);
  }

  // 송신완료시까지 블록
  while ( !(UCSR0A & (1 << TXC0)) ) {
    __asm__("nop\n\t"); // no operation
  }

  set_com_mode(RX_MODE); // 수신 모드로 설정

}
void dx_tx_packet_for_position_control(unsigned char id, unsigned char goal_pos[2]) {
  
  unsigned char packet[11];
  unsigned char i;

  packet[0] = 0xFF;
  packet[1] = 0xFF;
  packet[2] = id;
  packet[3] = 0x07;
  packet[4] = 0x03;
  packet[5] = 0x1E;
  packet[6] = goal_pos[0];
  packet[7] = goal_pos[1];
  packet[8] = 0x00;
  packet[9] = 0x00;
  dx_insert_checksum_byte(packet);

  set_com_mode(TX_MODE); // 송신 모드로 설정
  for (i = 0; i < 11; i++) {
    Serial.write(packet[i]);
  }

  // 송신완료시까지 블록
  while ( !(UCSR0A & (1 << TXC0)) ) {
    __asm__("nop\n\t"); // no operation
  }
  
  set_com_mode(RX_MODE); // 수신 모드로 설정
}

void findInverseKinematic(float x, float y, float z){
    cartsia2cylinder(x, y, z);
    float theta2 = findTheta2(r,z);
    float theta1 = findTheta1(r,z,theta2);
    motorDeg[0] = int(pi*11);
    motorDeg[1] = int(theta1*11);
    motorDeg[2] = int(theta2*11);
}

void cartsia2cylinder(float x, float y, float _z){
    float cord[3];
    pi = atan2(y,x) * 57.2958;
    r = sqrt(x*x + y*y);
    z = _z;
}

float findTheta2(float r, float z){
    float innerSqrt = ((L1 + L2)*(L1 + L2) - (r*r + z*z))/
                        ((r*r + z*z) - (L1 - L2)*(L1 - L2));
    return 2*atan(sqrt(innerSqrt))* 57.2958;
}

float findTheta1(float r, float z, float th2){
    float firstTerm = atan2(z,r)*57.2958;
    float secondTerm = atan2(L2*sin(th2),(L1 + L2*cos(th2)))* 57.2958;
    return firstTerm - secondTerm;
}

unsigned char bottomPacket(int REF){
    return REF % (16*16);
}

unsigned char topPacket(int REF){
  return (REF/(16*16))%(16*16);
}
