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

//! \def NR_OF_SENSORS
//! Maximum number of connected gas sensors.
#define NR_OF_SENSORS  32u

//! \def NR_OF_ALARMS
//! Number of possible alarm levels.
#define NR_OF_ALARMS    2u

//! \def NR_OF_RELAYS
//! Maximum number of relays.
#define NR_OF_RELAYS  2u
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

//! Number of average values
//static const uint16_t averageValueNr = 15U;
#define averageValueNr 15U

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

typedef enum
{
    INH_NONE, // inhibit not active
    INH_TEMP, // inhibit 4h
    INH_PERM  // inhibit permanent
} InhModes_t;

//! \enum SensorType_t
//! List of possible SensorTypes.
typedef enum
{
    SNS_EMPTY,         //!< no sensor attached
    SNS_ELCHEM,        //!< El-Chemical sensor
    SNS_NDIR,          //!< NDIR Std Sensor
    SNS_ZIRKON,        //!< Zirkonium O2 Sensor
    SNS_PELLISTOR,     //!< Pellistor Sensor
    SNS_SEMICONDUCTOR, //!< Semiconductor Sensor
    SNS_EA_CO2,        //!< Electro Acustic CO2 Sensor
    SNS_HUMIDITY,      //!< Humidity Sensor
    SNS_UNDEF          //!< undefined sensor
} SensorType_t;

//! \enum SensorBoard_t
//! Sensor Board
typedef enum
{
    SB_NO_BOARD, //!< no valid channel active
    SB_CH1,      //!< Channel1 active
    SB_CH2,      //!< Channel2 active
    SB_BOTH      //!< Both Channels active
  } SensorBoard_t;

//! \enum AlarmModes_t
//! Alarm modes.
typedef enum
{
    AT_M_NORMAL, // normal mode: upper value = alarm
    AT_M_OXYGEN, // reverse mode: lower value = alarm
    AT_M_WINDOW  // range mode: lower = main alarm, upper = pre-alarm
} AlarmModes_t;

//! \enum FaultNr_t
//! Fault number types.
typedef enum
{
    FNR_NOFAULT,      //!< no fault
    FNR_ADOF,         //!< AD overflow
    FNR_ADUF,         //!< AD underflow
    FNR_CALIBRATION,  //!< missing calibration
    FNR_POWER24V,     //!< 24V power too low
    FNR_POWER5V,      //!< 5V power too low
    FNR_NTC,          //!< temperature sensor failure
    FNR_EEPROM,       //!< EEPROM failure
    FNR_CPUTEMP,      //!< CPU temperature outside sensor specific limit
    FNR_SYSTEM,       //!< watchdog CRC or RAM failure
    FNR_OFFLINE,      //!< sensor offline (no CAN responses)
    FNR_BLOCKED,      //!< sensor blocked (no CAN counter changes)
    FNR_OBJ_LOSS      //!< object missing
} FaultNr_t;

//! \addtogroup Sensors
//! @{

typedef int16_t Sensors_Value20mA_t;  // 20mA values are stored in �A
typedef int16_t Sensors_Value_t;

//! \struct Sensor_t
//! Sensor variables structure.
typedef struct
{
  uint8_t On;                          //!< on off decrement status counter of sensor
  uint8_t Objects;                     //!< Check byte 1bit per TPDO object
  InhModes_t Inhibit;                  //!< inhibit status
  uint8_t CommVersionMain;             //!< main communication version number
  uint8_t CommVersionSub;              //!< sub communication version number
  SensorType_t  SensorType;            //!< definition of sensor type
  SensorBoard_t SensorBoardType;       //!< channel definition of sensor
  AlarmModes_t AlarmMode;              //!< alarm mode
  bool Failed;                         //!< set, when on off decrement counter hits one
  bool Detected;                       //!< set, when sensor response on CAN is received
  bool SetAdjRequest;                  //!< set, when factory setup command needs to be sent
  char FWversion[8];                   //!< firmware version
  char GasType[6];                     //!< gas type string (i.e. CO or NO2)
  char GasUnit[6];                     //!< gas unit string (i.e. PPM)
  int16_t GasEndValue;                 //!< integer end value of concentration range
  int16_t StartupConcentration;        //!< integer value of defined startup concentration
  uint8_t DecimalPoint;                //!< decimal point definition of concentration
  uint8_t MaxDecimalPoint;             //!< maximum decimal point to be displayed on OLED
  uint8_t Faults;                      //!< channel fault
  uint8_t SysFaults;                   //!< system fault
  FaultNr_t FaultNr;                   //!< first detected fault number (priority weighed)
  uint8_t AlarmByte;                   //!< bit pattern of alarms
  uint8_t StoredAlarmByte;             //!< bit pattern of alarms that have occured
  uint16_t InhTimer;                   //!< timer to handle 4h inhibit
  int16_t Temperature;                 //!< sensor temperature
  int16_t GasResult;                   //!< integer gas concentration (raw unprocessed value)
  int16_t GasBusResult;                //!< integer gas concentration after processing defined rules
  int16_t GasResultuA;                 //!< integer gas result in uA
  int16_t GasResultPermille;           //!< relative gas concentration in per-mill of end value
  int16_t InputValue;                  //!< sensor input value during setup mode
  int16_t InputAvVal;                  //!< sensor input average value during setup mode
  uint16_t CanCounter;                 //!< response counter
  int16_t AlarmValues[NR_OF_ALARMS];   //!< average gas concentration integer part
  int8_t AlarmAvgMin[NR_OF_ALARMS];    //!< average time in minutes (-1 = alarm not used)
  uint16_t AlarmOnVal[NR_OF_ALARMS];   //!< alarm on level gas concentration integer part
  uint16_t AlarmOffVal[NR_OF_ALARMS];  //!< alarm off level gas concentration integer part
  uint16_t AlarmOnDelay[NR_OF_ALARMS]; //!< alarm on delay time in seconds
  uint16_t AlarmOffDelay[NR_OF_ALARMS];//!< alarm off delay time in seconds
  uint16_t Power24V;                   //!< Voltage 2400 == 24.00V
  int16_t AlarmProfile;                //!< alarm profile (alarm definition) of sensor
  bool AlarmProfileWarning;            //!< CO profile different than in other sensors
} Sensor_t;
//! @}  close group Sensor_public_typedefs

//! \typedef RTC_Time_t
//! Time base type, equivalent to time_t from time.h - UNIX time stamp in
//! seconds since 1-Jan-70
typedef uint32_t RTC_Time_t;


//! \struct RTC_SetTimeDate_t
//! Time and date base type, equivalent to \p struct \p tm from time.h
typedef struct
{
    uint8_t Sec;   //!< seconds 0 to 60 (60 allowed for the occasional leap second)
    uint8_t Min;   //!< minutes , 0 to 59
    uint8_t Hour;  //!< hours since midnight, 0 to 23
    uint8_t Day;   //!< day of the month, 1 to 31
    uint8_t Month; //!< months since January, 0 to 11
    uint8_t Year;  //!< years since 2000
} RTC_SetTimeDate_t;

typedef enum
{
    OS_STARTUP,
    OS_ACTIVE,
    OS_GO_INACTIVE_TEMPORARLY,
    OS_GO_INACTIVE_PERMANENTLY,
    OS_INACTIVE_TEMPORARLY,
    OS_INACTIVE_PERMANENTLY,
    OS_OFF
  } OperatingStates_t;
//! Dynamic sensor parameters

//! Alarm states
typedef enum
{
    AS_OFF,
    AS_ONDELAYED,
    AS_ON,
    AS_OFFDELAYED
  } AlarmStates_t;
//! Dynamic alarm parameters

typedef struct
{
    uint16_t TimerS : 14;   //lint !e46
    AlarmStates_t State : 2;
} __attribute__((packed))AlarmParameters_t;

typedef struct
{
    int32_t SingleSum;
    Sensors_Value_t Values[averageValueNr];
    uint8_t ValueIndex;
    uint8_t TimerS;
} __attribute__((packed))MovingAverage_t;

typedef  struct
{
    uint8_t OperatingTimerSM; // seconds or minutes
    bool OutOfRange : 1;
    OperatingStates_t OperatingState : 3;
    AlarmParameters_t Fault;
    AlarmParameters_t Alarm[App_ALARMLEVELS_NR];    // Alarm 1, 2, 3, 4 (main alarm)
    Sensors_Value_t SensorValue[GD_VS_NR];
    MovingAverage_t MovingAverage;
} __attribute__((packed))SensorStates_t;

//! \struct RTC_TimeDate_t
//! Time and date base type, equivalent to \p struct \p tm from time.h
typedef struct
{
    uint8_t Sec;   //!< seconds 0 to 60 (60 allowed for the occasional leap second)
    uint8_t Min;   //!< minutes , 0 to 59
    uint8_t Hour;  //!< hours since midnight, 0 to 23
    uint8_t MDay;  //!< day of the month, 1 to 31
    uint8_t Mon;   //!< months since January, 0 to 11
    uint8_t Year;  //!< years since 1900
    uint8_t WDay;  //!< days since Sunday, 0 to 6
    uint16_t YDay; //!< days since January 1, 0 to 365
} RTC_TimeDate_t;



//! @}  close group RTC_public_typedefs

//! Fault alarm causes (bitwise)
typedef struct
{
    bool UnderRange : 1;
    bool OverRange : 1;
} GasDetection_FaultAlarmCauses_t;

//! Relay test states
typedef enum
{
    //! Relay coil off
    RM_TC_OFF = false,
    //! Relay coil on
    RM_TC_ON = true,
    //! No relay coil test
    RM_TC_NOTEST
  } RelayManager_TestCoil;

//! Dynamic relay properties
typedef struct
{
    //! Relay on timer (seconds)
    uint16_t MaxOnTimer : 12;  //lint !e46
    //! Test state
    RelayManager_TestCoil TestCoil : 2;
    //! Wait for manual (user) reset
    bool WaitForManualReset : 1;
    //! Relay has reached maximal on time
    bool MaxOnTimedOut : 1;
} relay_t;

typedef uint32_t Bit_128_t[App_SEGMENTS_NR];

typedef enum
{
    ES_FREE,
    ES_INHIBIT_BUSY,
    ES_FAULT_BUSY,
    ES_AL3_BUSY,
    ES_AL4_BUSY,
    ES_INHIBIT_CLOSED,
    ES_FAULT_CLOSED,
    ES_AL3_CLOSED,
    ES_AL4_CLOSED,
    ES_LAST
} EntryStatus_t;


typedef struct
{
    EntryStatus_t Status;
    uint8_t SensorIndex;
    int32_t MaxLevel;
    uint32_t StartTime;
    uint32_t MaxTime;
    uint32_t EndTime;
    uint16_t PlotValuesLength;
    Sensors_Value20mA_t PlotValues[60]; // Plot buffer for 4 Hours
} HistoryEntry_t;

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
