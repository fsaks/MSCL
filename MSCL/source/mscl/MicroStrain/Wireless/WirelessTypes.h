/*******************************************************************************
Copyright(c) 2015-2016 LORD Corporation. All rights reserved.

MIT Licensed. See the included LICENSE.txt for a copy of the full MIT License.
*******************************************************************************/

#pragma once

#include "mscl/Types.h"

namespace mscl
{
    //API Class: WirelessTypes
    //    Contains enumerations/types for various wireless information
    class WirelessTypes
    {
    public:
        //=====================================================================================================
        //API Enums: BeaconSource
        //    Represents the source of a BaseStation's Beacon.
        //
        //    beacon_none                - 0 - No Beacon
        //    beacon_internalTimer       - 1 - Internal Timer
        //    beacon_internalPPS         - 2 - Internal PPS
        //    beacon_externalPPS         - 3 - External PPS
        //=====================================================================================================
        enum BeaconSource
        {
            beacon_none                = 0,
            beacon_internalTimer       = 1,
            beacon_internalPPS         = 2,
            beacon_externalPPS         = 3
        };

        //=====================================================================================================
        //API Enums: MicroControllerType
        //    Represents the types of microcontrollers possible on the Wireless Devices.
        //
        //    microcontroller_18F452             - 31 - 18F452, 20MHz
        //    microcontroller_18F4620            - 32 - 18F4620, 20MHz
        //    microcontroller_18F46K20           - 33 - 18F46K20, 40MHz
        //    microcontroller_18F67K90           - 34 - 18F67K90, 40MHz
        //    microcontroller_EFM32WG990F256     - 35 - EFM32WG990F256, 48MHz
        //=====================================================================================================
        enum MicroControllerType
        {
            microcontroller_18F452             = 31,
            microcontroller_18F4620            = 32,
            microcontroller_18F46K20           = 33,
            microcontroller_18F67K90           = 34,
            microcontroller_EFM32WG990F256     = 35,
        };

        //=====================================================================================================
        //API Enums: DataCollectionMethod
        //    Represents the types of data collection methods that can be performed on the Node.
        //
        //    collectionMethod_logOnly           - 1 - Data is logged to the Node's internal memory to be downloaded later
        //    collectionMethod_transmitOnly      - 2 - Data is transmitted wirelessly over the air
        //    collectionMethod_logAndTransmit    - 3 - Data is both logged to the Node and transmitted over the air.
        //=====================================================================================================
        enum DataCollectionMethod
        {
            collectionMethod_logOnly           = 1,
            collectionMethod_transmitOnly      = 2,
            collectionMethod_logAndTransmit    = 3
        };

        //=====================================================================================================
        //Enums: DataType
        //    dataType_first                     - 1 - The smallest value in the list
        //    dataType_2byteUInt_shifted         - 1 - 2-byte unsigned integer (bit-shifted)
        //    dataType_4ByteFloat                - 2 - 4-byte float
        //    dataType_2byteUInt_12bitRes        - 3 - 2-byte unsigned integer (12-bit resolution)
        //    dataType_4byteUInt                 - 4 - 4-byte unsigned integer
        //    dataType_2byteUInt_16bitRes        - 7 - 2-byte unsigned integer (16-bit resolution)
        //    dataType_last                      - 7 - The largest value in the list
        //=====================================================================================================
        enum DataType
        {
            dataType_first                 = 1,

            dataType_2byteUInt_shifted     = 1,
            dataType_4ByteFloat            = 2,
            dataType_2byteUInt_12bitRes    = 3,
            dataType_4byteUInt             = 4,
            dataType_2byteUInt_16bitRes    = 7,

            dataType_last                  = 7
        };

        //=====================================================================================================
        //API Enums: DataFormat
        //    Represents the types of data formats that data can be sampled in.
        //
        //    dataFormat_2byte_uint         - 1 - Data is in a 2-byte unsigned integer format
        //    dataFormat_4byte_float        - 2 - Data is in a 4-byte float format
        //=====================================================================================================
        enum DataFormat
        {
            dataFormat_2byte_uint         = 1,
            dataFormat_4byte_float        = 2
        };

        //=====================================================================================================
        //API Enums: SyncSamplingMode
        //    Represents the types of synchronized sampling modes the Node can be in.
        //
        //    syncMode_continuous   - 0x7400 - Data will be transmitted in standard non-burst synchronized sampling mode.
        //    syncMode_burst        - 0xF600 - Data will be transmitted in a burst synchronized sampling mode.
        //=====================================================================================================
        enum SyncSamplingMode
        {
            syncMode_continuous   = 0x7400,
            syncMode_burst        = 0xF600
        };

        //=====================================================================================================
        //API Enums: SamplingMode
        //    Represents the types of sampling modes the Node can be in.
        //
        //    samplingMode_sync             - 1 - Synchronized Sampling mode
        //    samplingMode_syncBurst        - 2 - Synchronized Sampling, Burst mode
        //    samplingMode_nonSync          - 3 - Asynchronous Sampling mode (LDC)
        //    samplingMode_armedDatalog     - 4 - Armed Datalogging Sampling mode
        //    samplingMode_syncEvent        - 5 - Synchronized Sampling mode with Event Triggering
        //    samplingMode_nonSyncEvent     - 6 - Asynchronous Sampling mode (LDC) with Event Triggering
        //=====================================================================================================
        enum SamplingMode
        {
            samplingMode_sync             = 1,
            samplingMode_syncBurst        = 2,
            samplingMode_nonSync          = 3,
            samplingMode_armedDatalog     = 4,
            samplingMode_syncEvent        = 5,
            samplingMode_nonSyncEvent     = 6
        };

        //=====================================================================================================
        //API Enums: DefaultMode
        //    Represents the types of default modes the Node can have.
        //    This is the mode the Node starts in, as well as the mode the node goes into after an inactivity timeout.
        //
        //    defaultMode_idle       - 0 - Default of Idle Mode.
        //    default_ldc            - 1 - Default of Low Duty Cycle sampling.
        //    default_datalog        - 4 - Default of Armed Datalogging (but with no timestamp) sampling.
        //    default_sleep          - 5 - Default of a low-power Sleep Mode.
        //    default_sync           - 6 - Default of Sync Sampling (must hear a beacon to start sampling).
        //=====================================================================================================
        enum DefaultMode
        {
            defaultMode_idle           = 0,
            defaultMode_ldc            = 1,
            defaultMode_datalog        = 4,
            defaultMode_sleep          = 5,
            defaultMode_sync           = 6
        };

        //=====================================================================================================
        //API Enums: Frequency
        //    Represents the frequency (channel) that can be used for Wireless Devices.
        //
        //    freq_unknown - 0 - Unknown Frequency
        //    freq_11      - 11 - Frequency 11 (2.405 Ghz)
        //    freq_12      - 12 - Frequency 12 (2.410 Ghz)
        //    freq_13      - 13 - Frequency 13 (2.415 Ghz)
        //    freq_14      - 14 - Frequency 14 (2.420 Ghz)
        //    freq_15      - 15 - Frequency 15 (2.425 Ghz)
        //    freq_16      - 16 - Frequency 16 (2.430 Ghz)
        //    freq_17      - 17 - Frequency 17 (2.435 Ghz)
        //    freq_18      - 18 - Frequency 18 (2.440 Ghz)
        //    freq_19      - 19 - Frequency 19 (2.445 Ghz)
        //    freq_20      - 20 - Frequency 20 (2.450 Ghz)
        //    freq_21      - 21 - Frequency 21 (2.455 Ghz)
        //    freq_22      - 22 - Frequency 22 (2.460 Ghz)
        //    freq_23      - 23 - Frequency 23 (2.465 Ghz)
        //    freq_24      - 24 - Frequency 24 (2.470 Ghz)
        //    freq_25      - 25 - Frequency 25 (2.475 Ghz)
        //    freq_26      - 26 - Frequency 26 (2.480 Ghz)
        //=====================================================================================================
        enum Frequency
        {
            freq_unknown = 0,
            freq_11      = 11,
            freq_12      = 12,
            freq_13      = 13,
            freq_14      = 14,
            freq_15      = 15,
            freq_16      = 16,
            freq_17      = 17,
            freq_18      = 18,
            freq_19      = 19,
            freq_20      = 20,
            freq_21      = 21,
            freq_22      = 22,
            freq_23      = 23,
            freq_24      = 24,
            freq_25      = 25,
            freq_26      = 26
        };

        //=====================================================================================================
        //API Enums: TransmitPower
        //    Represents the transmit powers that can be used for Wireless Devices.
        //
        //    power_20dBm   - 20 - 20 dBm (100 mw)
        //    power_16dBm   - 16 - 16 dBm (39 mw)
        //    power_10dBm   - 10 - 10 dBm (10 mw)
        //    power_5dBm    - 5  - 5 dBm (3 mw)
        //    power_0dBm    - 0  - 0 dBm  (1 mw)
        //=====================================================================================================
        enum TransmitPower
        {
            power_20dBm   = 20,
            power_16dBm   = 16,
            power_10dBm   = 10,
            power_5dBm    = 5,
            power_0dBm    = 0
        };

        //=====================================================================================================
        //API Enums: NodeRetransmission
        //    Represents the possible Node Retransmission values.
        //
        //    retransmission_off        - 0 - Retransmission is turned off
        //    retransmission_on         - 1 - Retransmission is turned on
        //    retransmission_disabled   - 2 - Retransmission is disabled. It should not be set to on or off
        //=====================================================================================================
        enum NodeRetransmission
        {
            retransmission_off        = 0,
            retransmission_on         = 1,
            retransmission_disabled   = 2
        };

        //=====================================================================================================
        //API Enums: TriggerType
        //    Represents the different trigger types that can cause a datalogging session to be logged to a Wireless Node.
        //
        //    trigger_userInit       - 0 - The user started this datalogging session manually by sending a start sampling command.
        //    trigger_ceiling        - 1 - The trigger was caused by a ceiling event.
        //    trigger_floor          - 2 - The trigger was caused by a floor event.
        //    trigger_rampUp         - 3 - The trigger was caused by a ramp-up event.
        //    trigger_rampDown       - 4 - The trigger was caused by a ramp-down event.
        //=====================================================================================================
        enum TriggerType
        {
            trigger_userInit       = 0,
            trigger_ceiling        = 1,
            trigger_floor          = 2,
            trigger_rampUp         = 3,
            trigger_rampDown       = 4
        };

        //=====================================================================================================
        //API Enums: CalCoef_EquationType
        //    Represents the equation types that can be used for calibration coefficients on Wireless Nodes.
        //
        //    equation_none        - 0 - value = none, [bits/no equation applied]
        //    equation_standard    - 4 - value = (slope * bits) + offset
        //=====================================================================================================
        enum CalCoef_EquationType
        {
            equation_none        = 0,
            equation_standard    = 4
        };

        //=====================================================================================================
        //API Enums: CalCoef_Unit
        //    Represents the unit types that can be used for calibration coefficients on Wireless Nodes.
        //
        //    unit_none                           - 0   - no unit or unknown unit
        //    unit_accel_g                        - 4   - acceleration due to gravity
        //    unit_accel_mPerSec2                 - 5   - meters per second squared
        //    unit_accel_milliG                   - 34  - milli-G's
        //    unit_accel_ftPerSec2                - 35  - Feet/Second Squared
        //    unit_angDisplacement_degrees        - 26  - degrees
        //    unit_angDisplacement_radians        - 65  - Radians
        //    unit_angVelocity_degreesPerSec      - 27  - degrees per second
        //    unit_angVelocity_radiansPerSec      - 28  - radians per second
        //    unit_current_ampere                 - 58  - Ampere
        //    unit_current_milliampere            - 59  - Milliampere
        //    unit_current_microampere            - 60  - Microampere
        //    unit_density_kgPerMeter3            - 93  - kg per meter cubed
        //    unit_displacement_meters            - 12  - meters
        //    unit_displacement_millimeters       - 13  - millimeters
        //    unit_displacement_micrometers       - 14  - micrometers
        //    unit_displacement_feet              - 38  - Feet
        //    unit_displacement_inches            - 39  - Inches
        //    unit_displacement_yards             - 40  - Yards
        //    unit_displacement_miles             - 41  - Miles
        //    unit_displacement_nautMiles         - 42  - Nautical Miles
        //    unit_displacement_thouInch          - 43  - Thousandths of an Inch
        //    unit_displacement_hundInch          - 44  - Hundredths of an Inch
        //    unit_displacement_kilometers        - 45  - Kilometers
        //    unit_displacement_centimeters       - 46  - Centimers
        //    unit_energy_wattHour                - 54  - Watt-Hour
        //    unit_energy_kiloWattHour            - 55  - Kilowatt-Hour
        //    unit_flowRate_cubicMetersPerSec     - 75  - Cubic Meters Per Second
        //    unit_flowRate_cubicFtPerSec         - 76  - Cubic Feet Per Second
        //    unit_force_lbf                      - 15  - pound force
        //    unit_force_newtons                  - 16  - Newtons
        //    unit_force_kiloNewtons              - 17  - kiloNewtons
        //    unit_freq_rpm                       - 30  - revolutions per minute
        //    unit_freq_hertz                     - 31  - hertz
        //    unit_freq_kiloHertz                 - 64  - Kilohertz
        //    unit_irradiance_wattsPerSqMeter     - 47  - Watts per Square Meter
        //    unit_magneticFlux_gauss             - 89  - Gauss
        //    unit_mass_kilograms                 - 18  - kilograms
        //    unit_mass_pound                     - 49  - Pound
        //    unit_other_bits                     - 1   - raw bits
        //    unit_other_mVperV                   - 33  - milliVolt/Volt
        //    unit_other_percentLife              - 36  - % Life
        //    unit_other_count                    - 37  - Count
        //    unit_other_percent                  - 29  - percent
        //    unit_other_value                    - 88  - value
        //    unit_other_gSec                     - 90  - G-Seconds
        //    unit_other_secsPerSec               - 91  - Seconds per Second (clock-drift)
        //    unit_other_unitless                 - 94  - unitless
        //    unit_par_microEinstein              - 48  - microEinstein
        //    unit_power_watt                     - 50  - Watt
        //    unit_power_milliwatt                - 51  - Milliwatt
        //    unit_power_horsepower               - 52  - Horsepower
        //    unit_pressure_bar                   - 19  - bar
        //    unit_pressure_millibar              - 61  - MilliBar
        //    unit_pressure_psi                   - 20  - pounds per square inch
        //    unit_pressure_atm                   - 21  - atmospheric pressure
        //    unit_pressure_mmHg                  - 22  - millimeters of mercury
        //    unit_pressure_inHg                  - 62  - Inches of Mercury
        //    unit_pressure_pascal                - 23  - Pascal
        //    unit_pressure_megaPascal            - 24  - megaPascal
        //    unit_pressure_kiloPascal            - 25  - kiloPascal
        //    unit_reactiveEnergy_VARh            - 56  - Volt-Ampere Reactive Hour
        //    unit_reactiveEnergy_kVARh           - 57  - Kilovolt-Ampere Reactive Hour
        //    unit_reactivePower_var              - 53  - Volt-Ampere Reactive
        //    unit_rh_percentRh                   - 32  - percent relative humidity
        //    unit_rssi_dBm                       - 63  - dBm
        //    unit_rssi_dBHz                      - 92  - dBHz
        //    unit_strain_strain                  - 2   - Strain
        //    unit_strain_microStrain             - 3   - microStrain
        //    unit_temp_celsius                   - 9   - degrees Celsius
        //    unit_temp_kelvin                    - 10  - Kelvin
        //    unit_temp_fahrenheit                - 11  - degrees Fahrenheit
        //    unit_time_secs                      - 80  - Seconds
        //    unit_time_nanosecs                  - 81  - Nanoseconds
        //    unit_time_microsecs                 - 82  - Microseconds
        //    unit_time_millisecs                 - 83  - Milliseconds
        //    unit_time_minutes                   - 84  - Minutes
        //    unit_time_hours                     - 85  - Hours
        //    unit_time_days                      - 86  - Days
        //    unit_time_weeks                     - 87  - Weeks
        //    unit_torque_newtonMeter             - 77  - Newton Meter
        //    unit_torque_footPounds              - 78  - Foot Pounds
        //    unit_torque_inchPounds              - 79  - Inch Pounds
        //    unit_velocity_metersPerSec          - 66  - Meters per Second
        //    unit_velocity_kilometersPerSec      - 67  - Kilometers per Second
        //    unit_velocity_kilometersPerHr       - 68  - Kilometers per Hour
        //    unit_velocity_milesPerHr            - 69  - Miles per Hour
        //    unit_velocity_knots                 - 70  - Knots
        //    unit_volts_volts                    - 6   - volts
        //    unit_volts_millivolts               - 7   - milliVolts
        //    unit_volts_microvolts               - 8   - microVolts
        //    unit_volume_cubicMeter              - 71  - Cubic Meter
        //    unit_volume_cubicFt                 - 72  - Cubic Feet
        //    unit_volume_liters                  - 73  - Liters
        //    unit_volume_gallon                  - 74  - Gallon
        //=====================================================================================================
        enum CalCoef_Unit
        {
            unit_none                           = 0,
            unit_other_bits                     = 1,
            unit_strain_strain                  = 2,
            unit_strain_microStrain             = 3,
            unit_accel_g                        = 4,
            unit_accel_mPerSec2                 = 5,
            unit_volts_volts                    = 6,
            unit_volts_millivolts               = 7,
            unit_volts_microvolts               = 8,
            unit_temp_celsius                   = 9,
            unit_temp_kelvin                    = 10,
            unit_temp_fahrenheit                = 11,
            unit_displacement_meters            = 12,
            unit_displacement_millimeters       = 13,
            unit_displacement_micrometers       = 14,
            unit_force_lbf                      = 15,
            unit_force_newtons                  = 16,
            unit_force_kiloNewtons              = 17,
            unit_mass_kilograms                 = 18,
            unit_pressure_bar                   = 19,
            unit_pressure_psi                   = 20,
            unit_pressure_atm                   = 21,
            unit_pressure_mmHg                  = 22,
            unit_pressure_pascal                = 23,
            unit_pressure_megaPascal            = 24,
            unit_pressure_kiloPascal            = 25,
            unit_angDisplacement_degrees        = 26,
            unit_angVelocity_degreesPerSec      = 27,
            unit_angVelocity_radiansPerSec      = 28,
            unit_other_percent                  = 29,
            unit_freq_rpm                       = 30,
            unit_freq_hertz                     = 31,
            unit_rh_percentRh                   = 32,
            unit_other_mVperV                   = 33,
            unit_accel_milliG                   = 34,
            unit_accel_ftPerSec2                = 35,
            unit_other_percentLife              = 36,
            unit_other_count                    = 37,
            unit_displacement_feet              = 38,
            unit_displacement_inches            = 39,
            unit_displacement_yards             = 40,
            unit_displacement_miles             = 41,
            unit_displacement_nautMiles         = 42,
            unit_displacement_thouInch          = 43,
            unit_displacement_hundInch          = 44,
            unit_displacement_kilometers        = 45,
            unit_displacement_centimeters       = 46,
            unit_irradiance_wattsPerSqMeter     = 47,
            unit_par_microEinstein              = 48,
            unit_mass_pound                     = 49,
            unit_power_watt                     = 50,
            unit_power_milliwatt                = 51,
            unit_power_horsepower               = 52,
            unit_reactivePower_var              = 53,
            unit_energy_wattHour                = 54,
            unit_energy_kiloWattHour            = 55,
            unit_reactiveEnergy_VARh            = 56,
            unit_reactiveEnergy_kVARh           = 57,
            unit_current_ampere                 = 58,
            unit_current_milliampere            = 59,
            unit_current_microampere            = 60,
            unit_pressure_millibar              = 61,
            unit_pressure_inHg                  = 62,
            unit_rssi_dBm                       = 63,
            unit_freq_kiloHertz                 = 64,
            unit_angDisplacement_radians        = 65,
            unit_velocity_metersPerSec          = 66,
            unit_velocity_kilometersPerSec      = 67,
            unit_velocity_kilometersPerHr       = 68,
            unit_velocity_milesPerHr            = 69,
            unit_velocity_knots                 = 70,
            unit_volume_cubicMeter              = 71,
            unit_volume_cubicFt                 = 72,
            unit_volume_liters                  = 73,
            unit_volume_gallon                  = 74,
            unit_flowRate_cubicMetersPerSec     = 75,
            unit_flowRate_cubicFtPerSec         = 76,
            unit_torque_newtonMeter             = 77,
            unit_torque_footPounds              = 78,
            unit_torque_inchPounds              = 79,
            unit_time_secs                      = 80,
            unit_time_nanosecs                  = 81,
            unit_time_microsecs                 = 82,
            unit_time_millisecs                 = 83,
            unit_time_minutes                   = 84,
            unit_time_hours                     = 85,
            unit_time_days                      = 86,
            unit_time_weeks                     = 87,
            unit_other_value                    = 88,
            unit_magneticFlux_gauss             = 89,
            unit_other_gSec                     = 90,
            unit_other_secsPerSec               = 91,
            unit_rssi_dBHz                      = 92,
            unit_density_kgPerMeter3            = 93,
            unit_other_unitless                 = 94

            //170 (0xAA) needs to be reserved - treated as none
            //255 (0xFF) needs to be reserved - treated as none
        };

        //=====================================================================================================
        //API Enums: ChannelType
        //    Represents the types of channels on Wireless Nodes.
        //
        //    chType_none                    - 0 - No channel type (channel not supported)
        //    chType_fullDifferential        - 1 - Full Differential
        //    chType_singleEnded             - 2 - Single Ended
        //    chType_battery                 - 3 - Battery
        //    chType_temperature             - 4 - Temperature
        //    chType_rh                      - 5 - Relative Humidity
        //    chType_acceleration            - 6 - Acceleration
        //    chType_displacement            - 7 - Displacement
        //    chType_voltage                 - 8 - Voltage
        //    chType_diffTemperature         - 9 - Differential - Temperature (thermocouple, rtd)
        //=====================================================================================================
        enum ChannelType
        {
            chType_none                    = 0,
            chType_fullDifferential        = 1,
            chType_singleEnded             = 2,
            chType_battery                 = 3,
            chType_temperature             = 4,
            chType_rh                      = 5,
            chType_acceleration            = 6,
            chType_displacement            = 7,
            chType_voltage                 = 8,
            chType_diffTemperature         = 9
        };

        //=====================================================================================================
        //API Enums: SettlingTime
        //    Represents the settling times used for thermocouple and voltage inputs on some Wireless Nodes.
        //
        //    settling_4ms           - 1     - 4 millisecond settling time (fastest settling)
        //    settling_8ms           - 2     - 8 millisecond settling time
        //    settling_16ms          - 3     - 16 millisecond settling time
        //    settling_32ms          - 4     - 32 millisecond settling time
        //    settling_40ms          - 5     - 40 millisecond settling time
        //    settling_48ms          - 6     - 48 millisecond settling time
        //    settling_60ms          - 7     - 60 millisecond settling time
        //    settling_101ms_90db    - 8     - 101 millisecond settling time (90db [60Hz Rejection])
        //    settling_120ms_80db    - 9     - 120 millisecond settling time (80db [50Hz Rejection])
        //    settling_120ms_65db    - 10    - 120 millisecond settling time (65db [50+60Hz Rejection])
        //    settling_160ms_69db    - 11    - 160 millisecond settling time (69db [50+60Hz Rejection])
        //    settling_200ms         - 12    - 200 millisecond settling time (highest resolution)
        //=====================================================================================================
        enum SettlingTime
        {
            settling_4ms           = 1,
            settling_8ms           = 2,
            settling_16ms          = 3,
            settling_32ms          = 4,
            settling_40ms          = 5,
            settling_48ms          = 6,
            settling_60ms          = 7,
            settling_101ms_90db    = 8,
            settling_120ms_80db    = 9,
            settling_120ms_65db    = 10,
            settling_160ms_69db    = 11,
            settling_200ms         = 12
        };

        //=====================================================================================================
        //API Enums: ThermocoupleType
        //    Represents the thermocouple types supported by some Wireless Nodes.
        //
        //    tc_uncompensated    - 0        - Uncompensated (no thermocouple type)
        //    tc_K                - 1        - K Type Thermocouple
        //    tc_J                - 2        - J Type Thermocouple
        //    tc_R                - 3        - R Type Thermocouple
        //    tc_S                - 4        - S Type Thermocouple
        //    tc_T                - 5        - T Type Thermocouple
        //    tc_E                - 6        - E Type Thermocouple
        //    tc_B                - 7        - B Type Thermocouple
        //    tc_N                - 8        - N Type Thermocouple
        //    tc_customPolynomial - 9        - Custom Polynomial
        //=====================================================================================================
        enum ThermocoupleType
        {
            tc_uncompensated    = 0,
            tc_K                = 1,
            tc_J                = 2,
            tc_R                = 3,
            tc_S                = 4,
            tc_T                = 5,
            tc_E                = 6,
            tc_B                = 7,
            tc_N                = 8,
            tc_customPolynomial = 9
        };

        //=====================================================================================================
        //API Enums: WirelessSampleRate
        //    The possible sample rates that are available for Wireless Nodes.
        //
        //    sampleRate_104170Hz        - 60    - 104170 Hz 
        //    sampleRate_78125Hz         - 58    - 78125 Hz
        //    sampleRate_62500Hz         - 57    - 62500 Hz
        //    sampleRate_25000Hz         - 56    - 25000 Hz
        //    sampleRate_12500Hz         - 55    - 12500 Hz
        //
        //    sampleRate_3200Hz          - 49    - 3200 Hz
        //    sampleRate_1600Hz          - 48    - 1600 Hz
        //    sampleRate_800Hz           - 47    - 800 Hz
        //
        //    sampleRate_1kHz            - 62    - 1 kHz
        //    sampleRate_2kHz            - 63    - 2 kHz
        //    sampleRate_3kHz            - 64    - 3 kHz
        //    sampleRate_4kHz            - 65    - 4 kHz
        //    sampleRate_5kHz            - 66    - 5 kHz
        //    sampleRate_6kHz            - 67    - 6 kHz
        //    sampleRate_7kHz            - 68    - 7 kHz
        //    sampleRate_8kHz            - 69    - 8 kHz
        //    sampleRate_9kHz            - 70    - 9 kHz    
        //    sampleRate_10kHz           - 71    - 10 kHz
        //    sampleRate_20kHz           - 72    - 20 kHz
        //    sampleRate_30kHz           - 73    - 30 kHz
        //    sampleRate_40kHz           - 74    - 40 kHz
        //    sampleRate_50kHz           - 75    - 50 kHz
        //    sampleRate_60kHz           - 76    - 60 kHz
        //    sampleRate_70kHz           - 77    - 70 kHz
        //    sampleRate_80kHz           - 78    - 80 kHz
        //    sampleRate_90kHz           - 79    - 90 kHz
        //    sampleRate_100kHz          - 80    - 100 kHz
        //
        //    sampleRate_8192Hz          - 100    - 8192 Hz
        //    sampleRate_4096Hz          - 101    - 4096 Hz
        //    sampleRate_2048Hz          - 102    - 2048 Hz
        //    sampleRate_1024Hz          - 103    - 1024 Hz
        //    sampleRate_512Hz           - 104    - 512 Hz
        //    sampleRate_256Hz           - 105    - 256 Hz
        //    sampleRate_128Hz           - 106    - 128 Hz
        //    sampleRate_64Hz            - 107    - 64 Hz
        //    sampleRate_32Hz            - 108    - 32 Hz
        //    sampleRate_16Hz            - 109    - 16 Hz
        //    sampleRate_8Hz             - 110    - 8 Hz
        //    sampleRate_4Hz             - 111    - 4 Hz
        //    sampleRate_2Hz             - 112    - 2 Hz
        //    sampleRate_1Hz             - 113    - 1 Hz
        //
        //    sampleRate_2Sec            - 114    - 1 sample every 2 seconds
        //    sampleRate_5Sec            - 115    - 1 sample every 5 seconds
        //    sampleRate_10Sec           - 116    - 1 sample every 10 seconds
        //    sampleRate_30Sec           - 117    - 1 sample every 30 seconds
        //    sampleRate_1Min            - 118    - 1 sample every 1 minute
        //    sampleRate_2Min            - 119    - 1 sample every 2 minutes
        //    sampleRate_5Min            - 120    - 1 sample every 5 minutes
        //    sampleRate_10Min           - 121    - 1 sample every 10 minutes
        //    sampleRate_30Min           - 122    - 1 sample every 30 minutes
        //    sampleRate_60Min           - 123    - 1 sample every 60 minutes
        //    sampleRate_24Hours         - 127    - 1 sample every 24 hours
        //=====================================================================================================
        enum WirelessSampleRate
        {
            sampleRate_104170Hz        = 60,
            sampleRate_78125Hz         = 58,
            sampleRate_62500Hz         = 57,
            sampleRate_25000Hz         = 56,
            sampleRate_12500Hz         = 55,

            sampleRate_3200Hz          = 49,
            sampleRate_1600Hz          = 48,
            sampleRate_800Hz           = 47,

            sampleRate_1kHz            = 62,
            sampleRate_2kHz            = 63,
            sampleRate_3kHz            = 64,
            sampleRate_4kHz            = 65,
            sampleRate_5kHz            = 66,
            sampleRate_6kHz            = 67,
            sampleRate_7kHz            = 68,
            sampleRate_8kHz            = 69,
            sampleRate_9kHz            = 70,
            sampleRate_10kHz           = 71,
            sampleRate_20kHz           = 72,
            sampleRate_30kHz           = 73,
            sampleRate_40kHz           = 74,
            sampleRate_50kHz           = 75,
            sampleRate_60kHz           = 76,
            sampleRate_70kHz           = 77,
            sampleRate_80kHz           = 78,
            sampleRate_90kHz           = 79,
            sampleRate_100kHz          = 80,

            sampleRate_8192Hz          = 100,
            sampleRate_4096Hz          = 101,
            sampleRate_2048Hz          = 102,
            sampleRate_1024Hz          = 103,
            sampleRate_512Hz           = 104,
            sampleRate_256Hz           = 105,
            sampleRate_128Hz           = 106,
            sampleRate_64Hz            = 107,
            sampleRate_32Hz            = 108,
            sampleRate_16Hz            = 109,
            sampleRate_8Hz             = 110,
            sampleRate_4Hz             = 111,
            sampleRate_2Hz             = 112,
            sampleRate_1Hz             = 113,

            sampleRate_2Sec            = 114,
            sampleRate_5Sec            = 115,
            sampleRate_10Sec           = 116,
            sampleRate_30Sec           = 117,
            sampleRate_1Min            = 118,
            sampleRate_2Min            = 119,
            sampleRate_5Min            = 120,
            sampleRate_10Min           = 121,
            sampleRate_30Min           = 122,
            sampleRate_60Min           = 123,
            sampleRate_24Hours         = 127
        };

        //Enum: RegionCode
        //    The possible region codes for the device.
        //
        //    region_usa        - 0
        //    region_europe     - 1
        //    region_japan      - 2
        //    region_other      - 3
        //    region_brazil     - 4
        enum RegionCode
        {
            region_usa        = 0,
            region_europe     = 1,
            region_japan      = 2,
            region_other      = 3,
            region_brazil     = 4
        };

        //API Enum: ChannelGroupSetting
        //    The possible settings for ChannelGroups.
        //
        //    chSetting_hardwareGain            - 0 - Hardware Gain
        //    chSetting_filterSettlingTime      - 1 - Filter Settling Time
        //    chSetting_thermocoupleType        - 2 - Thermocouple Type
        //    chSetting_linearEquation          - 3 - Linear Equation
        //    chSetting_unit                    - 4 - Unit
        //    chSetting_equationType            - 5 - Equation Type
        //    chSetting_hardwareOffset          - 6 - Hardware Offset
        //    chSetting_autoBalance             - 7 - Autobalance Function
        //    chSetting_gaugeFactor             - 8 - Gauge Factor
        //    chSetting_lowPassFilter           - 9 - Low Pass Filter
        //    chSetting_shuntCal                - 10 - Shunt Cal (Note: the actual shunt cal operation is not a features in MSCL)
        enum ChannelGroupSetting
        {
            chSetting_hardwareGain            = 0,
            chSetting_filterSettlingTime      = 1,
            chSetting_thermocoupleType        = 2,
            chSetting_linearEquation          = 3,
            chSetting_unit                    = 4,
            chSetting_equationType            = 5,
            chSetting_hardwareOffset          = 6,
            chSetting_autoBalance             = 7,
            chSetting_gaugeFactor             = 8,
            chSetting_lowPassFilter           = 9,
            chSetting_shuntCal                = 10,
        };

        //API Enum: AutoBalanceErrorFlag
        //    The possible completion flags for the AutoBalance Wireless Node function.
        //
        //    autobalance_success                - 0      - AutoBalance was successful.
        //    autobalance_maybeInvalid           - 1      - AutoBalance completed, but the values look suspicious.
        //    autobalance_notSupportedByNode     - 2      - AutoBalance is not supported by the Node.
        //    autobalance_notSupportedByCh       - 3      - AutoBalance is not supported by the channel.
        //    autobalance_targetOutOfRange       - 4      - The target balance value is out of range for the channel.
        //    autobalance_legacyNone             - 998    - The legacy AutoBalance command was used, so no info was returned.
        //    autobalance_notComplete            - 999    - AutoBalance has not yet completed. 
        enum AutoBalanceErrorFlag
        {
            autobalance_success                = 0,
            autobalance_maybeInvalid           = 1,
            autobalance_notSupportedByNode     = 2,
            autobalance_notSupportedByCh       = 3,
            autobalance_targetOutOfRange       = 4,
            autobalance_legacyNone             = 998,
            autobalance_notComplete            = 999
        };

        //API Enum: AutoCalCompletionFlag
        //    The possible completion flags for the AutoCal Wireless Node function.
        //
        //    autocal_success            - 0        - AutoCal was successful.
        //    autocal_maybeInvalid       - 1        - AutoCal completed, but the values look suspicious.
        //    autocal_notComplete        - 999      - AutoCal has not yet completed.
        enum AutoCalCompletionFlag
        {
            autocal_success            = 0,
            autocal_maybeInvalid       = 1,
            autocal_notComplete        = 999
        };

        //API Enum: AutoCalErrorFlag
        //    The possible error flags for the AutoCal Wireless Node function.
        //
        //    autocalError_none              - 0        - AutoCal showed no sign of errors.
        //    autocalError_sensorDetached    - 1        - AutoCal indicated the sensor may be detached.
        //    autocalError_sensorShorted     - 2        - AutoCal indicated the sensor may have shorted.
        enum AutoCalErrorFlag
        {
            autocalError_none             = 0,
            autocalError_sensorDetached   = 1,
            autocalError_sensorShorted    = 2
        };

        //API Enum: FatigueMode
        //    The different modes a Fatigue Node can operate in.
        //
        //    fatigueMode_angleStrain            - 0        - Standard angle strain mode: can enter specific angles to sample.
        //    fatigueMode_distributedAngle       - 1        - Distributed angle mode: can enter a low, high, and # of angles (4-16) to sample.
        //    fatigueMode_rawGaugeStrain         - 2        - Raw Gauge Strain mode: sends the raw strain sensor data.
        enum FatigueMode
        {
            fatigueMode_angleStrain            = 0,
            fatigueMode_distributedAngle       = 1,
            fatigueMode_rawGaugeStrain         = 2
        };

        //API Enum: EventTriggerType
        //  The types of event triggers that are available.
        //
        //  eventTrigger_ceiling    - 1 - Ceiling Trigger (greater than)
        //  eventTrigger_floor      - 2 - Floor Trigger (less than)
        enum EventTriggerType
        {
            eventTrigger_ceiling    = 1,
            eventTrigger_floor      = 2
        };

        //API Enum: Filter
        //  The filter options (used for low pass filter)
        //
        //  filter_33000hz  - 33000
        //  filter_20000hz  - 20000
        //  filter_10000hz  - 10000
        //  filter_5000hz   - 5000
        //  filter_4096hz   - 4096
        //  filter_2048hz   - 2048
        //  filter_2000hz   - 2000
        //  filter_1024hz   - 1024
        //  filter_1000hz   - 1000
        //  filter_512hz    - 512
        //  filter_500hz    - 500
        //  filter_256hz    - 256
        //  filter_200hz    - 200
        //  filter_128hz    - 128
        //  filter_100hz    - 100
        //  filter_50hz     - 50
        //  filter_26hz     - 26
        enum Filter
        {
            filter_33000hz = 33000,
            filter_20000hz = 20000,
            filter_10000hz = 10000,
            filter_5000hz  = 5000,
            filter_4096hz  = 4096,
            filter_2048hz  = 2048,
            filter_2000hz  = 2000,
            filter_1024hz  = 1024,
            filter_1000hz  = 1000,
            filter_512hz   = 512,
            filter_500hz   = 500,
            filter_256hz   = 256,
            filter_200hz   = 200,
            filter_128hz   = 128,
            filter_100hz   = 100,
            filter_50hz    = 50,
            filter_26hz    = 26
        };

        //API Enum: StorageLimitMode
        //  The options available for configuring what happens when the storage limit is reached when logging to a Node.
        //
        //  storageLimit_overwrite  - 0 - When the max memory is reached, old data is overwritten (FIFO).
        //  storageLimit_stop       - 1 - When the max memory is reached, the Node stops sampling and returns to its idle state.
        enum StorageLimitMode
        {
            storageLimit_overwrite  = 0,
            storageLimit_stop       = 1
        };

    public:
        //API Typedefs:
        //    DataCollectionMethods      - A vector of <DataCollectionMethod> enums.
        //    DataFormats                - A vector of <DataFormat> enums.
        //    WirelessSampleRates        - A vector of <WirelessSampleRate> enums.
        //    SamplingModes              - A vector of <SamplingMode> enums.
        //    DefaultModes               - A vector of <DefaultMode> enums.
        //    TransmitPowers             - A vector of <TransmitPower> enums.
        //    ChannelGroupSettings       - A vector of <ChannelGroupSetting> enums.
        //    FatigueModes               - A vector of <FatigueMode> enums.
        //    StorageLimitModes          - A vector of <StorageLimitMode> enums.
        typedef std::vector<DataCollectionMethod> DataCollectionMethods;
        typedef std::vector<DataFormat> DataFormats;
        typedef std::vector<WirelessSampleRate> WirelessSampleRates;
        typedef std::vector<SamplingMode> SamplingModes;
        typedef std::vector<DefaultMode> DefaultModes;
        typedef std::vector<TransmitPower> TransmitPowers;
        typedef std::vector<ChannelGroupSetting> ChannelGroupSettings;
        typedef std::vector<FatigueMode> FatigueModes;
        typedef std::vector<Filter> Filters;
        typedef std::vector<StorageLimitMode> StorageLimitModes;

        //API Constant: UNKNOWN_RSSI = 999
        //    The value given for an unknown RSSI value.
        static const int16 UNKNOWN_RSSI = 999;

#ifndef SWIG
        //=====================================================================================================
        //Enums: LegacyTransmitPower
        //    Represents the legacy transmit powers supported by older devices.
        //
        //    legacyPower_16dBm    - 25619 - 16 dBm (39 mw)
        //    legacyPower_10dBm    - 25615 - 10 dBm (10 mw)
        //    legacyPower_5dBm     - 25611 - 5 dBm (3 mw)
        //    legacyPower_0dBm     - 25607 - 0 dBm  (1 mw)
        //=====================================================================================================
        enum LegacyTransmitPower
        {
            legacyPower_16dBm    = 25619,
            legacyPower_10dBm    = 25615,
            legacyPower_5dBm     = 25611,
            legacyPower_0dBm     = 25607
        };

        //Function: dataTypeSize
        //    Gets the byte size of the data type passed in
        //
        //Parameters:
        //    type - The <DataType> to find the size of
        //
        //Returns:
        //    The size of the <DataType> passed in
        static uint16 dataTypeSize(DataType type);

        //Function: dataTypeToValueType
        //    Converts a <DataType> (value received from a data packet) to a <ValueType> which describes how the data is stored (as a short, as a float, etc)
        //
        //Parameters:
        //    type - The <DataType> to convert to a <ValueType>
        //
        //Returns:
        //    A <ValueType> converted from the passed in <DataType>
        static ValueType dataTypeToValueType(DataType type);

        //Function: dataFormatSize
        //    Gets the number of bytes per data point for a specified <DataFormat>
        //
        //Parameters:
        //    dataFormat - The <DataFormat> to get the number of bytes for
        //
        //Returns:
        //    The number of bytes for the <DataFormat> 
        static uint8 dataFormatSize(DataFormat dataFormat);

        //Function: settlingTime
        //    Gets the actual settling time, in milliseconds, for the given <SettlingTime>.
        //
        //Parameters:
        //    file - The <SettlingTime> enum to get the actual settling time for.
        //
        //Returns:
        //    The settling time, in milliseconds for the given <SettlingTime>.
        static uint16 settlingTime(SettlingTime time);

        //Function: dataloggingRateToSampleRate
        //    Converts a datalogging sample rate value to a <WirelessSampleRate>.
        //
        //Parameters:
        //    eepromValue - The datalogging sample rate eeprom value that was on the Node.
        //
        //Returns:
        //    A <WirelessSampleRate> for the given datalogging sample rate.
        static WirelessSampleRate dataloggingRateToSampleRate(uint16 eepromValue);

        //Function: sampleRateToDataloggingRate
        //    Converts a <WirelessSampleRate> to a datalogging sample rate eeprom value.
        //
        //Parameters:
        //    rate - The <WirelessSampleRate> to convert to a datalogging sample rate.
        //
        //Returns:
        //    A datalogging sample rate eeprom value for the given <WirelessSampleRate>.
        static uint16 sampleRateToDataloggingRate(WirelessSampleRate rate);

        //Function: maxTransmitPower
        //    Gets the max transmit power that is allowed based on the region code.
        //
        //Parameters:
        //    region - the <WirelessTypes::RegionCode> to get the max power for.
        //
        //Returns:
        //    The max <WirelessTypes::TransmitPower>.
        static WirelessTypes::TransmitPower maxTransmitPower(WirelessTypes::RegionCode region);

        //Function: legacyToTransmitPower
        //    Converts the given <WirelessTypes::LegacyTransmitPower> to the <WirelessTypes::TransmitPower> equivalent.
        //
        //Parameters:
        //    legacyVal - the <WirelessTypes::LegacyTransmitPower> to convert.
        //
        //Returns:
        //    The <WirelessTypes::TransmitPower> equivalent of the legacy transmit power.
        static WirelessTypes::TransmitPower legacyToTransmitPower(WirelessTypes::LegacyTransmitPower legacyVal);

        //Function: transmitPowerToLegacy
        //    Converts the given <WirelessTypes::TransmitPower> to the <WirelessTypes::LegacyTransmitPower> equivalent.
        //
        //Parameters:
        //    power - the <WirelessTypes::TransmitPower> to convert.
        //
        //Returns:
        //    The <WirelessTypes::LegacyTransmitPower> equivalent of the current transmit power.
        //
        //Exceptions:
        //    - <Error>: Invalid transmit power.
        static WirelessTypes::LegacyTransmitPower transmitPowerToLegacy(WirelessTypes::TransmitPower power);
#endif
    };
}