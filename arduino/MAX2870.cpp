#include "MAX2870.h"

MAX2870Registers::MAX2870Registers() {
  for(int i=0;i<6;i++) {
    registers[i] = i;
    if(i == 4) {
      registers[i] |= 0b011000 << 26;
    }
  }
}

void MAX2870Registers::IntegerOrFractionalMode(uint32_t isInteger) {
  registers[5] = (isInteger << 24) | (registers[5] & ~(1 << 24));
  registers[0] = (isInteger << 31) | (registers[0] & ~(1 << 31));
}

void MAX2870Registers::LDPinFunction(uint32_t ld_mode) {
  registers[5] = (ld_mode << 22) | (registers[5] & ~(3 << 22));
}

void MAX2870Registers::MUXOutMode(uint32_t mode) {
  unsigned reg5bit = (mode >> 3) & 1;
  registers[5] = (reg5bit << 18) | (registers[5] & ~(1 << 18));
  registers[2] = ((mode & ~(8)) << 26) | (registers[2] & ~(7 << 26));
}

void MAX2870Registers::SetBS(uint32_t bs) {
  unsigned long bs_msbs = (bs & 0x300) >> 8;
  registers[4] = (bs_msbs & 3) << 24 | (registers[4] & ~(3 << 24));
  registers[4] = ((bs & 0xFF) << 12) | (registers[4] & ~(0xFF << 12));
}

void MAX2870Registers::SetRFDIV(uint32_t rfdiv) {
  registers[4] = (rfdiv & 7) << 20 | (registers[4] & ~(7 << 20));
}

void MAX2870Registers::FBModeSet(uint32_t fb) {
  registers[4] = ((fb & 1) << 23) | (registers[4] & ~(1 << 23));
}

void MAX2870Registers::SetBDIV(uint32_t bdiv) {
  registers[4] = ((bdiv & 1) << 9) | (registers[4] & ~(1 << 9));
}

void MAX2870Registers::RFOUTBEnable(uint32_t en) {
  registers[4] = ((en & 1) << 8) | (registers[4] & ~(1 << 8));
}

void MAX2870Registers::RFOUTBPower(uint32_t power) {
  registers[4] = ((power & 3) << 6) | (registers[4] & ~(3 << 6));
}

void MAX2870Registers::RFOUTAEnable(uint32_t en) {
  registers[4] = ((en & 1) << 5) | (registers[4] & ~(1 << 4));
}

void MAX2870Registers::RFOUTAPower(uint32_t power) {
  registers[4] = ((power & 3) << 3) | (registers[4] & ~(3 << 3));
}

void MAX2870Registers::SelectVCO(uint32_t vco) {
  registers[3] = (vco << 26) | (registers[3] & ~(63 << 26));
}

void MAX2870Registers::VASShdn(uint32_t vas_shdn) {
  registers[3] = (vas_shdn << 25) | (registers[3] & ~(1 << 25));
}

void MAX2870Registers::VASRetune(uint32_t vas_retune) {
  registers[3] = (vas_retune << 24) | (registers[3] & ~(1 << 24));
}

void MAX2870Registers::SetCDMMode(uint32_t cdm_mode) {
  registers[3] = (cdm_mode << 15) | (registers[3] & ~(3 << 15));
}

void MAX2870Registers::SetCDIV(uint32_t cdiv) {
  registers[3] = (cdiv << 3) | (registers[3] & ~(4095 << 3));
}


void MAX2870Registers::LDSpeed(uint32_t pfds) {
  registers[2] = (pfds << 31) | (registers[2] & ~(1 << 31));
}

void MAX2870Registers::FracNNoise(uint32_t noise_mode) {
  registers[2] = (noise_mode << 29) | (registers[2] & ~(3 << 29));
}

void MAX2870Registers::RefDoubler(uint32_t ref_doubler) {
  registers[2] = (ref_doubler << 25) | (registers[2] & ~(1 << 25));
}

void MAX2870Registers::RefDiv2(uint32_t ref_div) {
  registers[2] = (ref_div << 24) | (registers[2] & ~(1 << 24));
}

void MAX2870Registers::SetR(uint32_t r) {
  registers[2] = (r << 14) | (registers[2] & ~(1023 << 14));
}

void MAX2870Registers::DoubleBuffer(uint32_t db) {
  registers[2] = (db << 13) | (registers[2] & ~(1 << 13));
}

void MAX2870Registers::CPCurrent(uint32_t current) {
  registers[2] = (current << 9) | (registers[2] & ~(15 << 9));
}

void MAX2870Registers::LDF(uint32_t ldf) {
  registers[2] = (ldf << 8) | (registers[2] & ~(1 << 8));
}

void MAX2870Registers::LDP(uint32_t ldp) {
  registers[2] = (ldp << 7) | (registers[2] & ~(1 << 7));
}

void MAX2870Registers::PDP(uint32_t pdp) {
  registers[2] = (pdp << 6) | (registers[2] & ~(1 << 6));
}

void MAX2870Registers::Shutdown(uint32_t shdn) {
  registers[2] = (shdn << 5) | (registers[2] & ~(1 << 5));
}

void MAX2870Registers::CPThreeState(uint32_t tri) {
  registers[2] = (tri << 4) | (registers[2] & ~(1 << 4));
}

void MAX2870Registers::CounterReset(uint32_t cntreset) {
  registers[2] = (cntreset << 3) | (registers[2] & ~(1 << 3));
}

void MAX2870Registers::CPOutputClamp(uint32_t cpoc) {
  registers[1] = (cpoc << 31) | (registers[1] & ~(1 << 31));
}

void MAX2870Registers::CPLin(uint32_t cplin) {
  registers[1] = (cplin << 29) | (registers[1] & ~(3 << 29));
}

void MAX2870Registers::PhaseSet(uint32_t phase) {
  registers[1] = (phase << 15) | (registers[1] & ~(4095 << 15));
}

void MAX2870Registers::SetM(uint32_t m) {
  registers[1] = (m << 3) | (registers[1] & ~(4095 << 3));
}

void MAX2870Registers::SetN(uint32_t n) {
  registers[0] = (n << 15) | (registers[0] & ~(65535 << 15));
}

void MAX2870Registers::SetFrac(uint32_t frac) {
  registers[0] = (frac << 3) | (registers[0] & ~(4095 << 3));
}

uint32_t MAX2870Registers::getRegister(unsigned regnum) {
  return registers[regnum];
}
