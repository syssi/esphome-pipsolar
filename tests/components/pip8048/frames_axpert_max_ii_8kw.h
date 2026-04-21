#pragma once

// Traffic from rem72, Voltronic Axpert Max II 8kW 48V
// Source: https://github.com/syssi/esphome-pipsolar/issues/202#issuecomment-4282809460
//
// Frame strings represent read_buffer_ content after CRC verification:
// leading '(' preserved, CRC bytes and CR zeroed (null-terminated).

namespace esphome::pip8048::testing {

// QPIRI — inverter ratings (identical across all 4 polling cycles in the capture)
// grid=230V/34.7A, out=230V/50Hz/34.7A/8000VA/8000W, bat=48V, soc=51V, under=48V
// bulk=55.2V, float=55V, type=2(AGM), acChg=10A, chg=40A, outPrio=2(SBU), chgPrio=3
// redischarge=52V, pvOkParallel=1, pvBalance=1
static const char AXPERT_8KW_QPIRI[] =
    "(230.0 34.7 230.0 50.0 34.7 8000 8000 48.0 51.0 48.0 55.2 55.0 2 010 040 0 2 3 9 01 0 0 52.0 1 1 480 0 140";

// QPIGS — general status, three consecutive polling cycles showing PV voltage variation
// grid=222.6V/49.9Hz, out=230.1V/50.0Hz, load=551VA/499W/6%, bus=398V
// bat=54.30V/0A/61%, temp=39°C, pv1=1.0A/214.4V, scc=0.0V, dis=0A
// status1=00010110(load+charging+scc_charging), fans=0V, eeprom=0, pv1pwr=226W, status2=010(switch_on)
static const char AXPERT_8KW_QPIGS_A[] =
    "(222.6 49.9 230.1 50.0 0551 0499 006 398 54.30 000 061 0039 01.0 214.4 00.00 00000 00010110 00 00 00226 010";

// pv1_charging_power=227W (up 1W from cycle A)
static const char AXPERT_8KW_QPIGS_B[] =
    "(222.2 50.0 230.2 49.9 0505 0475 006 398 54.30 000 061 0039 01.0 214.4 00.00 00000 00010110 00 00 00227 010";

// pv1_input_voltage=215.0V (up from 214.4V), pv1_charging_power=226W
static const char AXPERT_8KW_QPIGS_C[] =
    "(222.0 50.0 230.0 50.0 0529 0472 006 398 54.30 000 061 0039 01.0 215.0 00.00 00000 00010110 00 00 00226 010";

// QPIGS2 — second MPPT input, four consecutive cycles showing PV voltage variation
// pv2=1.0A, pwr=205W
static const char AXPERT_8KW_QPIGS2_A[] = "(01.0 202.0 00205 ";
static const char AXPERT_8KW_QPIGS2_B[] = "(01.0 201.7 00205 ";
// pv2_input_voltage=198.8V (dip), pv2_charging_power=204W
static const char AXPERT_8KW_QPIGS2_C[] = "(01.0 198.8 00204 ";
static const char AXPERT_8KW_QPIGS2_D[] = "(01.0 201.0 00205 ";

// QMOD — device mode (identical across all cycles)
// mode=B (battery mode)
static const char AXPERT_8KW_QMOD[] = "(B";

// QFLAG — device flags (identical across all cycles)
// Enabled: b(bypass), k(lcd_escape), u(overload_restart), z(fault_record)
// Disabled: a(buzzer), j(power_save), v(over_temp_restart), x(backlight), y(alarm)
static const char AXPERT_8KW_QFLAG[] = "(EbkuzDajvxy";

}  // namespace esphome::pip8048::testing
