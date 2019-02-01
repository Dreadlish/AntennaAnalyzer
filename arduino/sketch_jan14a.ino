#include "MAX2870.h"
#include "SoftwareI2C.h"
#include <SPI.h>

unsigned S1S2_SWITCH = 9;
unsigned S2_SWITCH = 8;
unsigned S1_SWITCH = 7;
unsigned GEN_SWITCH = 6;
unsigned LE_GEN = 5;
unsigned CE = 4;
unsigned LE_ATT = 3;
unsigned RF_OUT_EN = 2;

unsigned POWER_READ = A1;
unsigned MAG_VLT = A3;
unsigned PHS_VLT = A2;

char value_buffer[16];
char return_buffer[48];
char command = '\0';
uint8_t i = 0;

uint32_t diva = 5;
uint32_t r = 5;
uint32_t m = 2;
uint32_t n = 1164;
uint32_t frac = 0;
uint32_t bs = 80;
uint32_t cdiv = 50;
uint32_t fb = 0;

MAX2870Registers setupRegs(uint32_t diva, uint32_t r, uint32_t m, uint32_t n, uint32_t frac, uint32_t bs, uint32_t cdiv) {
  MAX2870Registers maxRegs;
  maxRegs.SetBS(bs);
  maxRegs.IntegerOrFractionalMode(0);
  maxRegs.LDPinFunction(1);
  maxRegs.MUXOutMode(2);
  maxRegs.FBModeSet(1);
  maxRegs.SetBDIV(1);
  maxRegs.RFOUTBEnable(0);
  maxRegs.RFOUTBPower(0);
  maxRegs.RFOUTAEnable(1);
  maxRegs.RFOUTAPower(3);
  maxRegs.VASShdn(0);
  maxRegs.VASRetune(1);
  maxRegs.SetCDMMode(0);
  maxRegs.LDSpeed(0);
  maxRegs.FracNNoise(2);
  maxRegs.RefDoubler(0);
  maxRegs.RefDiv2(0);
  maxRegs.DoubleBuffer(0);
  maxRegs.CPCurrent(12);
  maxRegs.LDF(0);
  maxRegs.LDP(0);
  maxRegs.PDP(1);
  maxRegs.Shutdown(0);
  maxRegs.CPThreeState(0);
  maxRegs.CounterReset(0);
  maxRegs.CPOutputClamp(0);
  maxRegs.CPLin(1);
  maxRegs.PhaseSet(1);
  maxRegs.SetRFDIV(diva);
  maxRegs.SetR(r);
  maxRegs.SetM(m);
  maxRegs.SetN(n);
  maxRegs.SetFrac(frac);
  maxRegs.SetCDIV(cdiv);
  return maxRegs;
}

uint16_t reczne_i2c_matko_bosko() {
#define SDA_1 8
#define SCL_1 9
  uint16_t input = 0;
  pinMode(SDA_1, OUTPUT);
  pinMode(SCL_1, OUTPUT);
  digitalWrite(SDA_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, HIGH); // 1
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, LOW); // 0
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, LOW); // 0
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, HIGH); // 1
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, HIGH); // 1
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, LOW); // 0
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, HIGH); // 1
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, HIGH); // R
  digitalWrite(SCL_1, LOW);
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, HIGH); // ACK
  digitalWrite(SCL_1, LOW);
  pinMode(SDA_1, INPUT);
  for(int i=0;i<8;i++) { // upper 8
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SCL_1, LOW);
  input = input << 1 & digitalRead(SDA_1);
  }
  pinMode(SDA_1, OUTPUT);
  digitalWrite(SDA_1, LOW); // ACK
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SCL_1, LOW);
  pinMode(SDA_1, INPUT);
  for(int i=0;i<8;i++) { // lower 8
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SCL_1, LOW);
  input = input << 1 & digitalRead(SDA_1);
  }
  pinMode(SDA_1, OUTPUT);
  digitalWrite(SDA_1, HIGH); // NAK
  digitalWrite(SCL_1, HIGH);
  digitalWrite(SDA_1, LOW); // STOP
  digitalWrite(SCL_1, LOW);
  return input;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LE_GEN, OUTPUT);
  pinMode(LE_ATT, OUTPUT);
  pinMode(RF_OUT_EN, OUTPUT);
  pinMode(CE, OUTPUT);
  pinMode(S1S2_SWITCH, OUTPUT);
  pinMode(S2_SWITCH, OUTPUT);
  pinMode(S1_SWITCH, OUTPUT);
  pinMode(GEN_SWITCH, OUTPUT);
    digitalWrite(LE_ATT, HIGH);
  digitalWrite(S1S2_SWITCH, LOW);
  digitalWrite(S2_SWITCH, LOW);
  digitalWrite(S1_SWITCH, LOW);
  digitalWrite(GEN_SWITCH, LOW);
  digitalWrite(RF_OUT_EN, LOW);
  digitalWrite(LE_GEN, HIGH);
  digitalWrite(CE, HIGH);
  SPI.begin();
  Serial.begin(9600);
//  dump_regs_to_serial(setupRegs(diva, r, m, n, frac, bs, cdiv));
  for (int i = 2; i > 0; i--) {
    put_all_gen_registers(setupRegs(diva, r, m, n, frac, bs, cdiv));
    delay(20); // delay recommended by manufacturer
  }
  spi_write_attenuator(127, LE_ATT);
  digitalWrite(RF_OUT_EN, HIGH);
//  Serial.write("hello\n");
  //dump_regs_to_serial();
}

void put_all_gen_registers(MAX2870Registers maxRegs) {
  for (int i = 5; i >= 0; i--) {
    spi_write_register(maxRegs.getRegister(i), LE_GEN);
  }
}

void fillWithZeros(char* ret_buffer) {
  int i;
  for(i=0;i<16;i++) {
    if(ret_buffer[i] == '\0') break;
  }
  for(;i<16;i++) {
    Serial.write("0");
  }
}

void dump_regs_to_serial(MAX2870Registers maxRegs)
{
  Serial.write("5: ");
  fillWithZeros(itoa((maxRegs.getRegister(5) >> 16), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(5) >> 16), return_buffer, 2));
  fillWithZeros(itoa((maxRegs.getRegister(5)), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(5)), return_buffer, 2));
  Serial.write("\n");
  Serial.write("4: ");
  fillWithZeros(itoa((maxRegs.getRegister(4) >> 16), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(4) >> 16), return_buffer, 2));
  Serial.write("\n");
  fillWithZeros(itoa((maxRegs.getRegister(4)), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(4)), return_buffer, 2));
  Serial.write("\n");
  Serial.write("3: ");
  fillWithZeros(itoa((maxRegs.getRegister(3) >> 16), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(3) >> 16), return_buffer, 2));
  fillWithZeros(itoa((maxRegs.getRegister(3)), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(3)), return_buffer, 2));
  Serial.write("\n");
  Serial.write("2: ");
  fillWithZeros(itoa((maxRegs.getRegister(2) >> 16), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(2) >> 16), return_buffer, 2));
  fillWithZeros(itoa((maxRegs.getRegister(2)), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(2)), return_buffer, 2));
  Serial.write("\n");
  Serial.write("1: ");
  fillWithZeros(itoa((maxRegs.getRegister(1) >> 16), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(1) >> 16), return_buffer, 2));
  fillWithZeros(itoa((maxRegs.getRegister(1)), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(1)), return_buffer, 2));
  Serial.write("\n");
  Serial.write("0: ");
  fillWithZeros(itoa((maxRegs.getRegister(0) >> 16), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(0) >> 16), return_buffer, 2));
  fillWithZeros(itoa((maxRegs.getRegister(0)), return_buffer, 2));
  Serial.write(itoa((maxRegs.getRegister(0)), return_buffer, 2));
  Serial.write("\n");
}

void spi_write_attenuator(unsigned attenuation, unsigned le) {
  digitalWrite(le, LOW);
  SPI.transfer(attenuation);
  digitalWrite(le, HIGH);
}

void spi_write_register(uint32_t single_register, unsigned le) {
  uint8_t single_8byte = single_register >> 24 & 255;
  digitalWrite(le, LOW);
  SPI.transfer(single_8byte);
  single_8byte = (single_register >> 16) & 255;
  SPI.transfer(single_8byte);
  single_8byte = (single_register >> 8) & 255;
  SPI.transfer(single_8byte);
  single_8byte = (single_register) & 255;
  SPI.transfer(single_8byte);
  digitalWrite(le, HIGH);
}

void loop() {
  char c;
  uint32_t val;
  uint32_t temp;
  if(Serial.available()) {
    c = Serial.read();
    if(c == '\n') {
      i=0;
      val = atoi(value_buffer);
//      Serial.write("command: ");
//      Serial.write(command);
//      Serial.write(" param: ");
//      Serial.write(itoa(val, return_buffer, 2));
//      Serial.write("\n");
      if(command == 'd') {
        diva = val;
      } else if(command == 'n') {
        n = val;
      } else if(command == 'c') {
        cdiv = val;
      } else if(command == 'b') {
        bs = val;
      } else if(command == 'm') {
        m = val;
      } else if(command == 'a') {
        frac = val;
      } else if(command == 'r') {
        r = val;
      } else if(command == 'f') {
        fb = val;
      } else if(command == 'w') {
//        Serial.write("Writing registers\n");
        //dump_regs_to_serial();
        put_all_gen_registers(setupRegs(diva, r, m, n, frac, bs, cdiv));
      } else if(command == 't') {
        spi_write_attenuator(val, LE_ATT);
      } else if(command == 'p') {
//        Serial.write("\nPOWER_READ ");
//        Serial.write(itoa(analogRead(POWER_READ), return_buffer, 16));
        Serial.write("M ");
        Serial.write(itoa(analogRead(MAG_VLT), return_buffer, 16));
        Serial.write(" P ");
        Serial.write(itoa(analogRead(PHS_VLT), return_buffer, 16));
        Serial.write("\n");
        Serial.flush();
      } else if(command == 'g') {
        digitalWrite(GEN_SWITCH, val);
      } else if(command == 's') {
        char s1s2val = val & 1;
        char s1val = (val >> 1) & 1;
        char s2val = (val >> 2) & 1;
        char genval = (val >> 3) & 1;
        if(s1s2val == 1) {
          digitalWrite(S1S2_SWITCH, HIGH);
        } else digitalWrite(S1S2_SWITCH, LOW);
        if(s1val == 1) {
          digitalWrite(S1_SWITCH, HIGH);
        } else digitalWrite(S1_SWITCH, LOW);
        if(s2val == 1) {
          digitalWrite(S2_SWITCH, HIGH);
        } else digitalWrite(S2_SWITCH, LOW);
        if(genval == 1) {
          digitalWrite(GEN_SWITCH, HIGH);
        } else digitalWrite(GEN_SWITCH, LOW);
      }
      command = '\0';
    }
    if(!(command >= 0x61 && command <= 0x7a) && !(c == '\r' || c == '\n')) {
      command = c;
    } else {
      value_buffer[i++] = c;
      value_buffer[i] = '\0';
    }
  }
}
