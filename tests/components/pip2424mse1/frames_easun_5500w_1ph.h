#pragma once

// Traffic pip2424mse1, EASUN 5.5 kW single-phase (2424MSE1)
//
// Frame strings represent read_buffer_ content after CRC verification:
// leading '(' preserved, CRC bytes and CR zeroed (null-terminated).
//
// Note: QPIGS cycle 1 response has only 1 CRC byte (CF) in the capture —
// likely a capture artifact; data content is intact and consistent.

namespace esphome::pip2424mse1::testing {

// QPIRI — inverter ratings (identical across all 4 polling cycles)
// grid=230V/23.9A, out=230V/50Hz/23.9A/5500VA/5500W, bat=48V, recharge=51V, under=48V
// bulk=55.3V, float=55.3V, type=2(AGM), acChg=2A, chg=100A, inVRange=0(appliance)
// outPrio=2(SBU), chgPrio=3, redischarge=53.0V, pvOkParallel=0, pvBalance=1
static const char EASUN_5500W_QPIRI[] =
    "(230.0 23.9 230.0 50.0 23.9 5500 5500 48.0 51.0 48.0 55.3 55.3 2 02 100 0 2 3 1 01 0 0 53.0 0 1";

// QPIGS — general status, four consecutive polling cycles showing PV voltage variation
// Constant across all cycles: out=229.8–230.2V/50Hz, bat=53.20V/0A/50%, status=00010110
// status1=00010110 (bit3=load, bit5=charging, bit6=scc_charging)
// status2=010 (bit1=switch_on)
//
// Cycle 1: grid=239.2V, out=229.8V, load=321VA/197W/5%, bus=422V, temp=56°C
//          pv=1.7A/229.2V, pv_pwr=407W
static const char EASUN_5500W_QPIGS_A[] =
    "(239.2 50.0 229.8 50.0 0321 0197 005 422 53.20 000 050 0056 01.7 229.2 00.00 00000 00010110 00 00 00407 010";

// Cycle 2: grid=238.3V, out=230.2V, load=322VA/205W/5%, pv=2.1A/224.7V, pv_pwr=473W
static const char EASUN_5500W_QPIGS_B[] =
    "(238.3 50.0 230.2 50.0 0322 0205 005 422 53.20 000 050 0056 02.1 224.7 00.00 00000 00010110 00 00 00473 010";

// Cycle 3: grid=239.3V, bus=423V, pv=1.9A/209.8V, pv_pwr=408W
static const char EASUN_5500W_QPIGS_C[] =
    "(239.3 50.0 230.2 50.0 0299 0196 005 423 53.20 000 050 0056 01.9 209.8 00.00 00000 00010110 00 00 00408 010";

// Cycle 4: grid=239.1V, temp=57°C, pv=1.8A/215.9V, pv_pwr=405W
static const char EASUN_5500W_QPIGS_D[] =
    "(239.1 50.0 230.2 50.0 0322 0198 005 423 53.20 000 050 0057 01.8 215.9 00.00 00000 00010110 00 00 00405 010";

// QMOD — device mode (identical across all cycles)
// mode=B (battery mode)
static const char EASUN_5500W_QMOD[] = "(B";

// QFLAG — device flags (identical across all cycles)
// Enabled: z(fault_record) only
// Disabled: a(buzzer), b(bypass), j(power_save), k(lcd_escape), u(overload_restart),
//           v(over_temp_restart), x(backlight), y(alarm)
static const char EASUN_5500W_QFLAG[] = "(EzDabjkuvxy";

}  // namespace esphome::pip2424mse1::testing
