//-----------------------------------------------------------------------------
//! \file Configuration.h
//-----------------------------------------------------------------------------
// (C) Copyright by Ing.b�ro W.Meier, 2015
//-----------------------------------------------------------------------------
// Project                  : CANline CM32
// Module Reference         : Configuration
// Target Hardware          : CANline CM32 FrontPanel
// Target Operating System  : none
// Language/Compiler        : ARM C/C++ V5.06
// Author                   : Giovanni Caracciolo
// Created                  : 24. September 2015
//-----------------------------------------------------------------------------
// Description:
// -
//-----------------------------------------------------------------------------
// Revision History:
//
// 24. September 2015, Giovanni Caracciolo
//  - File created
//-----------------------------------------------------------------------------
#ifndef Configuration_h
#define Configuration_h

#include <stdint.h>
#include <stdbool.h>

//! \addtogroup Configuration
//! @{

#define CONFIGURATION_VERSION 6U
#define MAGIC_NUMBER          ((uint32_t)0x87654321U)
#define OFFSET_OF_FC(address) ((uint32_t)&address-(uint32_t)&settingsConfigurationFlash)
//! \def Driver_FLASHCELLERASED
//! Define the state of a 32 bit word of FLASH memory which is erased.
#define Driver_FLASHCELLERASED    0xffffffffU


// Hardware characteristics
#define App_SEGMENT_SIZE            32U                   // Number of sensor/relays on 1 segment
#define App_SEGMENTS_NR             4U                    // Number of segments
#define App_MAX_SENSORS_NR          (App_SEGMENTS_NR*App_SEGMENT_SIZE) // Maximum number of sensors
#define App_MAX_RELAYS_NR           (App_SEGMENTS_NR*App_SEGMENT_SIZE) // Maximum number of relays
#define App_MIN_ONBOARD_SENSORS_NR  4U                    // A/D inputs, 4 mainboard
#define App_MAX_ONBOARD_SENSORS_NR  10U                   // A/D inputs, 4 mainboard + 6 option board
#define App_MIN_ONBOARD_RELAYS_NR   5U                    // 5 onboard (mainboard) relays
#define App_MAX_ONBOARD_RELAYS_NR   8U                    // 8 onboard (mainboard) relays
#define App_LOCAL_SENSORS_NR        App_SEGMENT_SIZE      // Onboard sensors, 1st segment
#define App_LOCAL_RELAYS_NR         App_SEGMENT_SIZE      // Onboard relays, 1st segment
#define App_HW_RELAYTIMERS_NR       4U                    // Number of supported relay timers
#define App_ALARMLEVELS_NR          4U                    // Number of supported alarm levels


typedef enum
{
  C_SL_2,
  C_SL_4,
  C_SL_10,
  C_SL_MAX,
  C_SL_NR
} Configuration_SensorLimit_t;

//! Measurement modes
typedef enum
{
    GD_M_NORMAL = 0,    // Normal mode: upper value = alarm
    GD_M_OXYGEN = 1,    // Reverse mode: lower value = alarm
    GD_M_WINDOW = 2,    // Range mode: lower = main alarm, upper = pre alarm
    GD_M_NR
} GasDetection_Modes_t;

//! Value source
typedef enum
{
    GD_VS_NORMAL = 0,    // normal sensor value
    GD_VS_AVERAGE = 1,   // average value, time: ValueAverageTimeM
    GD_VS_20MA = 2,      // direct 20mA value, without range adaption, not used yet
    GD_VS_NR
} GasDetection_ValueSource_t;

//! Sensor states
typedef enum
{
    GD_SS_ALARM1 = 0, // 0..App_ALARMLEVELS_NR alarms
    GD_SS_ALARMLOWEST = GD_SS_ALARM1,
    GD_SS_ALARM2,
    GD_SS_ALARM3,
    GD_SS_ALARM4,
    GD_SS_ALARMHIGHEST = GD_SS_ALARM4,  // = App_ALARMLEVELS_NR-1
    GD_SS_FAULT,
    GD_SS_INACTIVATED_TEMPORARLY,
    GD_SS_INACTIVATED_PERMANENTLY,
    GD_SS_STARTUP,
    GD_SS_OUTOFRANGE,
    GD_SS_INACTIVATED  // GD_SS_INACTIVATED_TEMPORARLY or GD_SS_INACTIVATED_PERMANENTLY
} GasDetection_SensorState_t;

//! Fault alarm causes (bitwise)
typedef struct
{
    bool UnderRange : 1;
    bool OverRange : 1;
} GasDetection_FaultAlarmCauses_t;

typedef uint32_t Bit_128_t[App_SEGMENTS_NR];

//! Alarm properties
typedef struct
{
    GasDetection_ValueSource_t ValueSource; //  Normal sensor, average value, 20mA ...
    int16_t OnLevel;                        //  Alarm EIN, value hysteresis
    int16_t OffLevel;                       //  Alarm AUS, value hysteresis
    uint16_t OnDelay;                       //  Alarm Delay EIN, sec
    uint16_t OffDelay;                      //  Alarm Delay AUS, sec
    Bit_128_t Relays;                       //  Zugewiesene Relais (Bit 0..31, 32..63, 64..95, 96..127)
} GasDetection_Alarm_t;
typedef GasDetection_Alarm_t GasDetection_Alarms_t[App_ALARMLEVELS_NR]; // Alarm 1, 2, 3, 4 (Hauptalarm)


//! Gas detection properties
typedef struct
{
    bool Active;                            //  0:inactive / 1:active
    GasDetection_Modes_t Mode;              //  Normal, O2, ...
    int16_t MeasuringRange;                 //  Werteanzeige z.B. 4-stellig/2DP 20.00
    int16_t Offset;                         //  k�nstlicher Nullpunkt f�r negative Werte oder Differenzwerte
    uint8_t DecimalPoint;                   //  Wertanzeige Dezimal
    uint8_t ValueAverageTimeM;              //  Zeit f�r Mittelwertbildung in Minuten
    char MeasuringUnit[8];                  //  Einheitenstring
    char SensorName[16];                    //  Sensorstring
    char LocationName[24];                  //  Locationstring
    GasDetection_Alarms_t Alarms;           // Alarm 1, 2, 3, 4 (Hauptalarm)
    GasDetection_ValueSource_t FaultValueSource; //  Normal sensor, average value, 20mA, ...
    GasDetection_FaultAlarmCauses_t FaultAlarmCauses;  //  Fault under-/overrange, ...
    uint16_t FaultAlarmOnDelay;             //   Fault Delay EIN, value hysteresis
    uint16_t FaultAlarmOffDelay;            //   Fault Delay AUS, value hysteresis
    Bit_128_t FaultAlarmRelays;             //  Zugewiesene Relais (Bit 0..127), Relay 0 by default
} GasDetection_SensorProperties_t;
typedef GasDetection_SensorProperties_t GasDetection_SensorsProperties_t[App_MAX_SENSORS_NR];

//! Relay properties
typedef struct
{
    //! Relay 0:inactive / 1:active
    bool Active;
    //! Relay coil state. 0:de-energized (default) / 1:energized
    bool Energized;
    //! Relay is pulsating
    bool Pulsating;
    //! Manual oder auto reset. 0:auto (default) / 1:manual
    bool ManualReset;
    //! Manual override. 1:force immediate manual reset
    bool ImmediateReset;
    //! Relay state is mapped to buzzer
    bool BuzzerOn;
    //! Maximal relay on time (seconds)
    uint16_t MaxOnTime;
} RelayManager_RelayProperties_t;
//! Array of relays properties
typedef RelayManager_RelayProperties_t RelayManager_RelaysProperties_t[App_MAX_RELAYS_NR];

//! Relay timer properties
typedef struct
{
    int16_t Mode;                 // For future use (software requirement)
    //! Minutes after midnight at which the relays are on
    int16_t OnTime;               // Minutes since midnight
    //! Minutes after midnight at which the relays are off
    int16_t OffTime;              // Minutes since midnight
    //! Relays affected by timer
    Bit_128_t Relays;
} RelayManager_Timer_t;
//! Array of relay timers properties
typedef RelayManager_Timer_t RelayManager_Timers_t[App_HW_RELAYTIMERS_NR];

typedef struct
{
  char Date[12];      // Date of configuration, DD.MM.YYYYY
  char Name[8];       // Name of this device, displayed at startup
  uint8_t LEDMatrixType;
  uint64_t PasswordCode;  // Password code 8 bytes
  char AlarmText[App_ALARMLEVELS_NR][16];    // Text Alarm 1, 2, 3, 4 (Hauptalarm)
  char FaultText[16];       // Text bei Fehler
  char InactiveText[16];    // Text bei Inaktiv
  char ServiceText[16];     // Text bei Service Aufforderung
  int16_t TemperatureRangeLow; // Wird als Info benutzt
  int16_t TemperatureRangeHigh;// Wird als Info benutzt
  int32_t SerialNr; // max=2147483647 9 benutzbare freie Stellen, Kimessa Serien Nr = YYMMXXXX
  int16_t CPUType;    // Wird als Info benutzt
  uint16_t StartupTimeS;  // in Sekunden, Zeit bis Grenzwerte �berwacht werden
  uint16_t BeeperFrequency;
  uint16_t ServiceIntervalD;  // Tage
  uint16_t NrOfSensors;
  uint16_t NrOfRelays;
  bool SpecialStateOnRelay2;  // true: special state on relay 2, false: special state on relay 1 with fault
  GasDetection_SensorsProperties_t Sensors;
  RelayManager_RelaysProperties_t Relays;
  RelayManager_Timers_t RelayTimers;
} Configuration_Settings_t;

typedef struct
{
  volatile uint32_t MagicNumberBegin; // Magic number
  volatile uint32_t Version;          // Version of this data structure
  Configuration_Settings_t SettingsConfiguration;
  volatile uint32_t MagicNumberEnd;   // Magic number
  volatile uint32_t CRCval;
} Configuration_SettingsDescriptor_t;

//! Sensor inactivation
typedef struct
{
    bool Active;                          // according GasDetection_SensorProperties_t.Active
} GasDetection_UserSensorValues_t;
typedef GasDetection_UserSensorValues_t GasDetection_UserSensorsValues_t[App_MAX_SENSORS_NR];

typedef struct
{
  uint16_t ServiceIntervalD;           // Configuration_Settings_t.ServiceInterval
  int32_t SerialNr;                    // Configuration_Settings_t.SerialNr
  GasDetection_UserSensorsValues_t Sensors;
} Configuration_User_t;

//! @}
#endif
// End of Configuration.h
//-----------------------------------------------------------------------------
