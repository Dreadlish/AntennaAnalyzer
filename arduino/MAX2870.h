#ifndef MAX2870_H
#define MAX2870_H

#include "Arduino.h"

class MAX2870Registers {
  public:
    MAX2870Registers();
    uint32_t getRegister(unsigned number);
    void IntegerOrFractionalMode(uint32_t isInteger);
    void LDPinFunction(uint32_t ld_mode);
    void MUXOutMode(uint32_t mode);
    void SetBS(uint32_t bs);
    void SetRFDIV(uint32_t rfdiv);
    void FBModeSet(uint32_t fb);
    void SetBDIV(uint32_t bdiv);
    void RFOUTBEnable(uint32_t en);
    void RFOUTBPower(uint32_t power);
    void RFOUTAEnable(uint32_t en);
    void RFOUTAPower(uint32_t power);
    void SelectVCO(uint32_t vco);
    void VASShdn(uint32_t vas_shdn);
    void VASRetune(uint32_t vas_retune);
    void SetCDMMode(uint32_t cdm_mode);
    void SetCDIV(uint32_t cdiv);
    void LDSpeed(uint32_t pfds);
    void FracNNoise(uint32_t noise_mode);
    void RefDoubler(uint32_t ref_doubler);
    void RefDiv2(uint32_t ref_div);
    void SetR(uint32_t r);
    void DoubleBuffer(uint32_t db);
    void CPCurrent(uint32_t current);
    void LDF(uint32_t ldf);
    void LDP(uint32_t ldp);
    void PDP(uint32_t pdp);
    void Shutdown(uint32_t shdn);
    void CPThreeState(uint32_t tri);
    void CounterReset(uint32_t cntreset);
    void CPOutputClamp(uint32_t cpoc);
    void CPLin(uint32_t cplin);
    void PhaseSet(uint32_t phase);
    void SetM(uint32_t m);
    void SetN(uint32_t n);
    void SetFrac(uint32_t frac);
  public:
    uint32_t registers[6];
};

#endif //MAX2870_H
