#pragma once

// Traffic from jopl, Voltronic Axpert Max Twin 11.0kW 48V
// Source: https://github.com/syssi/esphome-pipsolar/issues/202#issuecomment-4281677534
//
// Frame strings represent read_buffer_ content after CRC verification:
// leading '(' preserved, CRC bytes and CR zeroed (null-terminated).
//
// Note: QPIGS responses in this capture contain corrupted bytes (non-ASCII values
// that passed CRC due to device-side transmission errors) and are therefore omitted.

namespace esphome::pip8048::testing {

// QPIRI — inverter ratings (identical across all 3 polling cycles)
// grid=230V/47.8A, out=230V/50Hz/47.8A/11000VA/11000W, bat=48V, soc=50V, under=48V
// bulk=56.0V, float=55.2V, type=2(AGM), acChg=10A, chg=100A, inVRange=1, outPrio=2, chgPrio=1
// redischarge=52V, pvOkParallel=0, pvBalance=1
static const char AXPERT_11KW_QPIRI[] =
    "(230.0 47.8 230.0 50.0 47.8 11000 11000 48.0 50.0 48.0 56.0 55.2 2 010 100 1 2 1 9 01 0 0 52.0 0 1 480 0 000";

// QPIGS2 — second MPPT input, three consecutive cycles
// pv2=3.0A/273.2V/825W
static const char AXPERT_11KW_QPIGS2_A[] = "(03.0 273.2 00825 ";
// pv2=2.8A/274.9V/796W
static const char AXPERT_11KW_QPIGS2_B[] = "(02.8 274.9 00796 ";
// pv2=2.7A/271.2V/736W (power decreasing)
static const char AXPERT_11KW_QPIGS2_C[] = "(02.7 271.2 00736 ";

// QMOD — device mode (identical across all cycles)
// mode=B (battery mode)
static const char AXPERT_11KW_QMOD[] = "(B";

// QFLAG — device flags (identical across all cycles, different from 8kW)
// Enabled: k(lcd_escape), u(overload_restart), v(over_temp_restart), x(backlight),
//          y(alarm), z(fault_record)
// Disabled: a(buzzer), b(bypass), j(power_save)
// (Unknown flag letters d, l, n are silently ignored by the parser)
static const char AXPERT_11KW_QFLAG[] = "(EkuvxyzDabdjln";

// QBATCD — battery charge/discharge control
// Response '(NAO': N/A/O are not '0'/'1', so all three bits parse as false.
// This is an atypical but CRC-valid response from this inverter model.
static const char AXPERT_11KW_QBATCD_NAO[] = "(NAO";

}  // namespace esphome::pip8048::testing
