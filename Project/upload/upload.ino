#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define L1 35
#define L2 35
#define BUFFER_SIZE 10
#define TX_Enable_pin            12  // 송신 활성화 핀
#define RX_Enable_pin            13  // 수신 활성화 핀
#define DX_ID                    0x07   // 서보모터 아이디
#define POTMODE                  0
#define INVERSEKINEMATICMODE     1
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
void cartsia2cylinder(double x, double y, double _z);
double findTheta2(double r, double z);
double findTheta1(double r, double z, double th2);
void findInverseKinematic(double x, double y, double z);

unsigned char ref[2];
int potVal[3];
double cartsianCord[3];
double r, pi, z;
double motorDeg[3];
char buf[BUFFER_SIZE];

void setup() {
    Serial.begin(57600);  // 통신 속도
    pinMode(TX_Enable_pin, OUTPUT); //TX Enable
    pinMode(RX_Enable_pin, OUTPUT); //RX Enable
    pinMode(8, OUTPUT);
    pinMode(9, INPUT);
    unsigned char cw_angle_limit[2];
    unsigned char ccw_angle_limit[2];

    cw_angle_limit[0] = 0x00;
    cw_angle_limit[1] = 0x00;
    ccw_angle_limit[0] = 0xFF;
    ccw_angle_limit[1] = 0xFF;  

    dx_set_control_mode(254, cw_angle_limit, ccw_angle_limit);
    digitalWrite(8, HIGH);
    delay(1);
}

void loop() {
    switch (readModeSW()) {
    case POTMODE:
        ref[0] = 0xFD;
        ref[1] = 0x07;
        dx_tx_packet_for_position_control(254, ref);
        delay(1000);
        ref[0] = 0x00;
        ref[1] = 0x00;
        dx_tx_packet_for_position_control(254, ref);
        delay(1000);
        break;

    case INVERSEKINEMATICMODE:
        Serial.println("Input");
        for (int i = 0; i < 3;  i++){
            if (Serial.available())
                cartsianCord[i] = Serial.parseInt();
        }
        findInverseKinematic(cartsianCord[0], cartsianCord[1], cartsianCord[2]);
        Serial.print(motorDeg[0]);
        Serial.print("\t");
        Serial.print(motorDeg[1]);
        Serial.print("\t");
        Serial.println(motorDeg[2]);
    }
}

void potRead(){
    potVal[POT_BOTTOM] = analogRead(A0);
    potVal[POT_MIDDLE] = analogRead(A1);
    potVal[POT_TOP]    = analogRead(A2);
    Serial.print(potVal[POT_BOTTOM]);
    Serial.print("\t");
    Serial.print(potVal[POT_MIDDLE]);
    Serial.print("\t");
    Serial.println(potVal[POT_TOP]);
}

int readModeSW(){
    bool modeSW = digitalRead(9);
    if(modeSW == 0){
        return POTMODE;
    }
    else if(modeSW == 1){
        return INVERSEKINEMATICMODE;
    }
    else
        return ERROR;
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
