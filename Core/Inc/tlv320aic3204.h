/*
 * tlv320aic3204.h
 *
 *  Created on: Jan 29, 2025
 *      Author: hayden
 */

#ifndef INC_TLV320AIC3204_H_
#define INC_TLV320AIC3204_H_
#include "main.h"

// define the relevant pins and handles

#define TLV_DEVICE_ADDR (uint16_t)(0x18 << 1)
#define TLV_I2C hi2c2
#define TLV_I2S hi2s1
#define TLV_NRST_GPIO_Port CODEC_NRST_GPIO_Port
#define TLV_NRST_Pin CODEC_NRST_Pin

//#define TLV_VERIFY_SETUP

// c-friendly scheme for passing a set of non-default register settings
// on initialization

typedef struct {
	uint8_t page;
	uint8_t address;
	uint8_t value;
}tlv_register_t;

#ifdef __cplusplus
extern "C" {
#endif



// does the main work of setting up registers. Should be called after initializing I2C but before starting the DMA loop
HAL_StatusTypeDef TLV_initCodec(tlv_register_t* settings, uint16_t size);

HAL_StatusTypeDef TLV_quickInit_monoGuitarPedal();

// REGISTER ADDRESSES=================================================
// these register & page definitions start on p 35 of the datasheet
#define TLV_softwareReset_pg 0x00
#define TLV_softwareReset_reg 0x01
// multiplexers
#define TLV_clockSetting1_pg 0x00
#define TLV_clockSetting1_reg 0x04
// pll P&R values
#define TLV_clockSetting2_pg 0x00
#define TLV_clockSetting2_reg 0x05
// pll J values
#define TLV_clockSetting3_pg 0x00
#define TLV_clockSetting3_reg 0x06
// pll D values (MSB)
#define TLV_clockSetting4_pg 0x00
#define TLV_clockSetting4_reg 0x07
// pll D values (LSB)
#define TLV_clockSetting5_pg 0x00
#define TLV_clockSetting5_reg 0x08
// NADC values
#define TLV_clockSetting6_pg 0x00
#define TLV_clockSetting6_reg 0x0B
// MADC values
#define TLV_clockSetting7_pg 0x00
#define TLV_clockSetting7_reg 0x0C

#define TLV_DACOSR1_pg 0x00
#define TLV_DACOSR1_reg 0x0D

#define TLV_DACOSR2_pg 0x00
#define TLV_DACOSR2_reg 0x0E

#define TLV_NADC_pg 0x00
#define TLV_NADC_reg 0x12

#define TLV_MADC_pg 0x00
#define TLV_MADC_reg 0x13

#define TLV_adcOversampling_pg 0x00
#define TLV_adcOversampling_reg 0x14

#define TLV_clockSetting10_pg 0x00
#define TLV_clockSetting10_reg 0x19

#define TLV_clockSetting11_pg 0x00
#define TLV_clockSetting11_reg 0x1A

#define TLV_audioIntSetting1_pg 0x00
#define TLV_audioIntSetting1_reg 0x1B

#define TLV_audioIntSetting2_pg 0x00
#define TLV_audioIntSetting2_reg 0x1C

#define TLV_audioIntSetting3_pg 0x00
#define TLV_audioIntSetting3_reg 0x1D

#define TLV_clockSetting12_pg 0x00
#define TLV_clockSetting12_reg 0x1E

#define TLV_audioIntSetting4_pg 0x00
#define TLV_audioIntSetting4_reg 0x1F

#define TLV_audioIntSetting5_pg 0x00
#define TLV_audioIntSetting5_reg 0x20

#define TLV_audioIntSetting6_pg 0x00
#define TLV_audioIntSetting6_reg 0x21

#define TLV_digitalIntMiscSetting_pg 0x00
#define TLV_digitalIntMiscSetting_reg 0x22

#define TLV_adcFlag_pg 0x00
#define TLV_adcFlag_reg 0x24

#define TLV_dacFlag1_pg 0x00
#define TLV_dacFlag1_reg 0x25

#define TLV_dacFlag2_pg 0x00
#define TLV_dacFlag2_reg 0x26

#define TLV_stickyFlag1_pg 0x00
#define TLV_stickyFlag1_reg 0x2A

#define TLV_intFlag1_pg 0x00
#define TLV_intFlag1_reg 0x2B

#define TLV_stickyFlag2_pg 0x00
#define TLV_stickyFlag2_reg 0x2C

#define TLV_stickyFlag3_pg 0x00
#define TLV_stickyFlag3_reg 0x2D

#define TLV_intFlag2_pg 0x00
#define TLV_intFlag2_reg 0x2E

#define TLV_intFlag3_pg 0x00
#define TLV_intFlag3_reg 0x2F

#define TLV_int1Control_pg 0x00
#define TLV_int1Control_reg 0x30

#define TLV_int2Control_pg 0x00
#define TLV_int2Control_reg 0x31

#define TLV_mfp5Control_pg 0x00
#define TLV_mfp5Control_reg 0x34

#define TLV_doutFuncControl_pg 0x00
#define TLV_doutFuncControl_reg 0x35

#define TLV_dinFuncControl_pg 0x00
#define TLV_dinFuncControl_reg 0x36

#define TLV_misoFuncControl_pg 0x00
#define TLV_misoFuncControl_reg 0x37

#define TLV_sclkFuncControl_pg 0x00
#define TLV_sclkFuncControl_reg 0x38

#define TLV_dacSignalProcessingBlock_pg 0x00
#define TLV_dacSignalProcessingBlock_reg 0x3C

#define TLV_adcSignalProcessingBlock_pg 0x00
#define TLV_adcSignalProcessingBlock_reg 0x3D

#define TLV_dacChannelSetup1_pg 0x00
#define TLV_dacChannelSetup1_reg 0x3F

#define TLV_dacChannelSetup2_pg 0x00
#define TLV_dacChannelSetup2_reg 0x40

#define TLV_leftDacDigitalVolume_pg 0x00
#define TLV_leftDacDigitalVolume_reg 0x41

#define TLV_rightDacDigitalVolume_pg 0x00
#define TLV_rightDacDigitalVolume_reg 0x42

#define TLV_headsetDetectionConfig_pg 0x00
#define TLV_headsetDetectionConfig_reg 0x43

#define TLV_drcControl1_pg 0x00
#define TLV_drcControl1_reg 0x44

#define TLV_drcControl2_pg 0x00
#define TLV_drcControl2_reg 0x45

#define TLV_drcControl3_pg 0x00
#define TLV_drcControl3_reg 0x46

#define TLV_beepGen1_pg 0x00
#define TLV_beepGen1_reg 0x47

#define TLV_beepGen2_pg 0x00
#define TLV_beepGen2_reg 0x48

#define TLV_beepGen3_pg 0x00
#define TLV_beepGen3_reg 0x49

#define TLV_beepGen4_pg 0x00
#define TLV_beepGen4_reg 0x4A

#define TLV_beepGen5_pg 0x00
#define TLV_beepGen5_reg 0x4B

#define TLV_beepGen6_pg 0x00
#define TLV_beepGen6_reg 0x4C

#define TLV_beepGen7_pg 0x00
#define TLV_beepGen7_reg 0x4D

#define TLV_beepGen8_pg 0x00
#define TLV_beepGen8_reg 0x4E

#define TLV_beepGen9_pg 0x00
#define TLV_beepGen9_reg 0x4F

#define TLV_adcChannelSetup_pg 0x00
#define TLV_adcChannelSetup_reg 0x51

#define TLV_adcFineGainAdjust_pg 0x00
#define TLV_adcFineGainAdjust_reg 0x52

#define TLV_leftAdcChannelVolume_pg 0x00
#define TLV_leftAdcChannelVolume_reg 0x53

#define TLV_rightAdcChannelVolume_pg 0x00
#define TLV_rightAdcChannelVolume_reg 0x54

#define TLV_adcPhaseAdjust_pg 0x00
#define TLV_adcPhaseAdjust_reg 0x55

#define TLV_leftChannelAcg1_pg 0x00
#define TLV_leftChannelAcg1_reg 0x56

#define TLV_leftChannelAcg2_pg 0x00
#define TLV_leftChannelAcg2_reg 0x57

#define TLV_leftChannelAcg3_pg 0x00
#define TLV_leftChannelAcg3_reg 0x58

#define TLV_leftChannelAcg4_pg 0x00
#define TLV_leftChannelAcg4_reg 0x59

#define TLV_leftChannelAcg5_pg 0x00
#define TLV_leftChannelAcg5_reg 0x5A

#define TLV_leftChannelAcg6_pg 0x00
#define TLV_leftChannelAcg6_reg 0x5B

#define TLV_leftChannelAcg7_pg 0x00
#define TLV_leftChannelAcg7_reg 0x5C

#define TLV_leftChannelAcg8_pg 0x00
#define TLV_leftChannelAcg8_reg 0x5D

#define TLV_rightChannelAcg1_pg 0x00
#define TLV_rightChannelAcg1_reg 0x5E

#define TLV_rightChannelAcg2_pg 0x00
#define TLV_rightChannelAcg2_reg 0x5F

#define TLV_rightChannelAcg3_pg 0x00
#define TLV_rightChannelAcg3_reg 0x60

#define TLV_rightChannelAcg4_pg 0x00
#define TLV_rightChannelAcg4_reg 0x61

#define TLV_rightChannelAcg5_pg 0x00
#define TLV_rightChannelAcg5_reg 0x62

#define TLV_rightChannelAcg6_pg 0x00
#define TLV_rightChannelAcg6_reg 0x63

#define TLV_rightChannelAcg7_pg 0x00
#define TLV_rightChannelAcg7_reg 0x64

#define TLV_rightChannelAcg8_pg 0x00
#define TLV_rightChannelAcg8_reg 0x65

#define TLV_dcMeasurement1_pg 0x00
#define TLV_dcMeasurement1_reg 0x66

#define TLV_dcMeasurement2_pg 0x00
#define TLV_dcMeasurement2_reg 0x67

#define TLV_leftDcOutput1_pg 0x00
#define TLV_leftDcOutput1_reg 0x68

#define TLV_leftDcOutput2_pg 0x00
#define TLV_leftDcOutput2_reg 0x69

#define TLV_leftDcOutput3_pg 0x00
#define TLV_leftDcOutput3_reg 0x6A

#define TLV_rightDcOutput1_pg 0x00
#define TLV_rightDcOutput1_reg 0x6B

#define TLV_rightDcOutput2_pg 0x00
#define TLV_rightDcOutput2_reg 0x6C

#define TLV_rightDcOutput3_pg 0x00
#define TLV_rightDcOutput3_reg 0x6D

// second page

#define TLV_powerConfig_pg 0x01
#define TLV_powerConfig_reg 0x01

#define TLV_ldoControl_pg 0x01
#define TLV_ldoControl_reg 0x02

#define TLV_playbackConfig1_pg 0x01
#define TLV_playbackConfig1_reg 0x03

#define TLV_playbackConfig2_pg 0x01
#define TLV_playbackConfig2_reg 0x04

#define TLV_outputDriverPowerControl_pg 0x01
#define TLV_outputDriverPowerControl_reg 0x09

#define TLV_commonModeControl_pg 0x01
#define TLV_commonModeControl_reg 0x0A

#define TLV_overCurrentProtectionConfig_pg 0x01
#define TLV_overCurrentProtectionConfig_reg 0x0B

#define TLV_hplRoutingSelection_pg 0x01
#define TLV_hplRoutingSelection_reg 0x0C

#define TLV_hprRoutingSelection_pg 0x01
#define TLV_hprRoutingSelection_reg 0x0D

#define TLV_lolRoutingSelection_pg 0x01
#define TLV_lolRoutingSelection_reg 0x0E

#define TLV_lorRoutingSelection_pg 0x01
#define TLV_lorRoutingSelection_reg 0x0F

#define TLV_hplDriverGainSetting_pg 0x01
#define TLV_hplDriverGainSetting_reg 0x10

#define TLV_hprDriverGainSetting_pg 0x01
#define TLV_hprDriverGainSetting_reg 0x11

#define TLV_lolDriverGainSetting_pg 0x01
#define TLV_lolDriverGainSetting_reg 0x12

#define TLV_lorDriverGainSetting_pg 0x01
#define TLV_lorDriverGainSetting_reg 0x13

#define TLV_headphoneDriverStartupControl_pg 0x01
#define TLV_headphoneDriverStartupControl_reg 0x14

#define TLV_in1LtoHPLVolume_pg 0x01
#define TLV_in1LtoHPLVolume_reg 0x16

#define TLV_in1RtoHPRVolume_pg 0x01
#define TLV_in1RtoHPRVolume_reg 0x17

#define TLV_mixerAmpLeftVolume_pg 0x01
#define TLV_mixerAmpLeftVolume_reg 0x18

#define TLV_mixerAmpRightVolume_pg 0x01
#define TLV_mixerAmpRightVolume_reg 0x19

#define TLV_micBiasConfig_pg 0x01
#define TLV_micBiasConfig_reg 0x33

#define TLV_leftMICPGAPosRouting_pg 0x01
#define TLV_leftMICPGAPosRouting_reg 0x34

#define TLV_leftMICPGANegRouting_pg 0x01
#define TLV_leftMICPGANegRouting_reg 0x36

#define TLV_rightMICPGAPosRouting_pg 0x01
#define TLV_rightMICPGAPosRouting_reg 0x37

#define TLV_rightMICPGANegRouting_pg 0x01
#define TLV_rightMICPGANegRouting_reg 0x39

#define TLV_floatingInputConfig_pg 0x01
#define TLV_floatingInputConfig_reg 0x3A

#define TLV_leftMICPGAVolumeControl_pg 0x01
#define TLV_leftMICPGAVolumeControl_reg 0x3B

#define TLV_rightMICPGAGain_pg 0x01
#define TLV_rightMICPGAGain_reg 0x3C

#define TLV_adcPowerTuneConfig_pg 0x01
#define TLV_adcPowerTuneConfig_reg 0x3D

#define TLV_adcAnalogVolumeControlFlag_pg 0x01
#define TLV_adcAnalogVolumeControlFlag_reg 0x3E

#define TLV_dacAnalogGainControlFlag_pg 0x01
#define TLV_dacAnalogGainControlFlag_reg 0x3F

#define TLV_analogQuickCharge_pg 0x01
#define TLV_analogQuickCharge_reg 0x47

//======================================

#ifdef __cplusplus
}
#endif

#endif /* INC_TLV320AIC3204_H_ */
