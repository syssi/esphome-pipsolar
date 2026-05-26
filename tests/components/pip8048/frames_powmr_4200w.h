#pragma once

// Traffic from a POWMR 4200W 12V inverter
// Source: https://github.com/syssi/esphome-pipsolar/issues/231
//
// Frame strings represent read_buffer_ content after CRC verification:
// leading '(' preserved, CRC bytes and CR zeroed (null-terminated).

namespace esphome::pip8048::testing {

// QPIRI — inverter ratings
// grid=230V/10A, out=230V/50Hz/10A/2300VA/2300W, bat=12V
// recharge=12.8V, under=11.0V, bulk=14.2V, float=13.6V, type=2(user)
// acChg=20A, chg=100A, inputRange=0, outPrio=2(SBU), chgPrio=2(solar+util)
// parallelMax=1, machineType=1, topology=0, outputMode=0
// redischarge=13.3V, pvOkParallel=0, pvBalance=1
static const char POWMR_4200W_QPIRI_100A[] =
    "(230.0 10.0 230.0 50.0 10.0 2300 2300 12.0 12.8 11.0 14.2 13.6 2 20 100 0 2 2 1 01 0 0 13.3 0 1";

// Same config but inverter reports chg=090A (zero-padded) in subsequent polls
static const char POWMR_4200W_QPIRI_090A[] =
    "(230.0 10.0 230.0 50.0 10.0 2300 2300 12.0 12.8 11.0 14.2 13.6 2 20 090 0 2 2 1 01 0 0 13.3 0 1";

// QPIGS — battery discharge, no grid, no PV
// grid=224.4V/50Hz (present but not used), out=229.2V/50Hz, load=366VA/283W/16%
// bus=379V, bat=13.10V/0A chg/80%, temp=25°C
// pv=0A/0V, scc=0V, dis=24A
// status1=00010000(load only), fans=0V, eeprom=0, pv1pwr=1W, status2=010(switch_on)
static const char POWMR_4200W_QPIGS_DISCHARGE[] =
    "(224.4 50.0 229.2 50.0 0366 0283 016 379 13.10 000 080 0025 0000 000.0 00.00 00024 00010000 00 00 00001 010";

// QPIGS — solar charging, no grid
// grid=0V/0Hz, out=229.4V/50Hz, load=13VA/13W/0%
// bus=395V, bat=13.00V/10A chg/76%, temp=22°C
// pv1=11A/134.3V, scc=0V, dis=0A
// status1=00010110(load+charging+scc_charging), fans=0V, eeprom=0, pv1pwr=177W, status2=010(switch_on)
static const char POWMR_4200W_QPIGS_SOLAR_A[] =
    "(000.0 00.0 229.4 50.0 0013 0013 000 395 13.00 010 076 0022 0011 134.3 00.00 00000 00010110 00 00 00177 010";

// pv1_input_voltage=131.9V, bat=13.10V/11A/80% (next polling cycle)
static const char POWMR_4200W_QPIGS_SOLAR_B[] =
    "(000.0 00.0 229.5 50.0 0010 0010 000 395 13.10 011 080 0022 0011 131.9 00.00 00000 00010110 00 00 00177 010";

// QMOD — device mode
// mode=B (battery mode)
static const char POWMR_4200W_QMOD[] = "(B";

// QFLAG — device flags
// Enabled (E): a(buzzer), u(overload_restart), x(backlight), y(alarm), z(fault_record)
// Disabled (D): b(bypass), j(power_save), k(lcd_escape), v(over_temp_restart)
// ';' is an unrecognized flag character sent by this model, silently ignored
static const char POWMR_4200W_QFLAG[] = "(EauxyzDbjkv;";

// QMUCHGCR — supported max utility charging currents (3-digit format, max 100A)
// POWMR uses MUCHGC%03.0f (e.g. MUCHGC010), not the 4-digit MUCHGC0%03.0f used by modern Voltronic
static const char POWMR_4200W_QMUCHGCR[] = "(010 020 030 040 050 060 070 080 090 100";

// QMCHGCR — supported max charging currents (zero-padded, includes 002A minimum)
static const char POWMR_4200W_QMCHGCR[] = "(002 010 020 030 040 050 060 070 080 090 100";

}  // namespace esphome::pip8048::testing
