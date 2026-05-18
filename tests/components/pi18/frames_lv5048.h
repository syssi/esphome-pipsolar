#pragma once

// PI18 frames recorded from LV5048 Hybrid V2 and compatible inverters.
// Source: https://github.com/syssi/esphome-pipsolar/issues/54
// Contributors: BanannaJoe (LV5048 Hybrid V2, 5600W, 48V) and BeckoPopov (LV5048 compatible)
//
// Frame strings represent read_buffer_ content after CRC verification:
// leading '^' preserved, CRC bytes and CR zeroed (null-terminated).
// Raw hex → ASCII with trailing [CRC_hi, CRC_lo, 0x0D] zeroed.

namespace esphome::pi18::testing {

// ── ^P005GS responses ─────────────────────────────────────────────────────────
//
// BanannaJoe — standby, PV only (no AC output, no grid)
// grid=231.2V/50.0Hz, out=0V, bat=51.8V/36%/4A_charging
// pv1=243.8V/302W, mppt1=charging(2), mppt2=abnormal(0)
// bat_dir=charge(1), dc_ac_dir=DC-AC(2), line_dir=output(2), load_conn=disconnect(0)
static const char LV5048_P005GS_STANDBY_PV[] =
    "^D1062312,500,0000,000,00000,00000,000,518,000,000,000,004,036,027,000,000,0302,0000,2438,0000,0,2,0,0,1,2,2,0";

// BeckoPopov — active load, solar+grid, 48V battery
// grid=242.2V/49.9Hz, out=230.5V/1475VA/1458W/26%
// bat=52.6V/84%/7A_charging, pv1=265.4V/1924W
// mppt1=charging(2), mppt2=abnormal(0)
// load_conn=connect(1), bat_dir=charge(1), dc_ac_dir=DC-AC(2), line_dir=donothing(0)
static const char LV5048_P005GS_ACTIVE[] =
    "^D1062422,499,2305,499,1475,1458,026,526,000,000,000,007,084,046,000,000,1924,0000,2654,0000,0,2,0,1,1,2,0,1";

// BanannaJoe — repeated measurement (battery charging, standby AC output)
// grid=230.1V/49.9Hz, bat=52.2V/40%/9A_charging, pv1=243.6V/557W
static const char LV5048_P005GS_CHARGING[] =
    "^D1062301,499,0000,000,00000,00000,000,522,000,000,000,009,040,032,000,000,0557,0000,2436,0000,0,2,0,0,1,2,2,0";

// ── ^P007PIRI responses ───────────────────────────────────────────────────────
//
// BanannaJoe — 5600W 48V system, solar-first charger, 60A max charging
// rated: 230V/24.3A/5600VA, battery 48V, bulk 55.4V, float 54.0V
// charger=solar-first(2), out_priority=utility(0), max_ac_chg=10A, max_chg=60A
static const char LV5048_P007PIRI_BANANNAJOE[] =
    "^D0892300,243,2300,500,243,5600,5600,480,500,520,480,554,540,2,010,060,0,0,2,9,1,0,0,0,1,00";

// BeckoPopov — 5600W 48V, 90A max charging, solar+utility charger priority 2
// rated: 230V/24.3A/5600VA, battery 48V, bulk 55.5V, float 54.4V, max_chg=90A
static const char LV5048_P007PIRI_BECKOPOPOV[] =
    "^D0892300,243,2300,500,243,5600,5600,480,470,530,440,555,544,2,030,090,1,1,2,9,0,0,1,0,1,00";

// ── ^P006MOD responses ────────────────────────────────────────────────────────
//
// BeckoPopov — mode 3 (battery/hybrid)
static const char LV5048_P006MOD_BATTERY[] = "^D00503";

// BanannaJoe — mode 5 (hybrid)
static const char LV5048_P006MOD_HYBRID[] = "^D00505";

// ── ^P007PGS0 responses ───────────────────────────────────────────────────────
//
// Single unit in parallel system, unit 0 idle (no grid, no AC output),
// aggregate: 12338 VA / 12336 W from other units, total_chg=337 A
static const char LV5048_P007PGS0_ACTIVE[] = "^D1131,0,00,0000,171,0000,000,0000,0000,12338,12336,000,000,257,337,257,"
                                             "337,000,5535,2345,0514,0000,0,0,0,0,0,0,051";

// ── ^P007FLAG responses ───────────────────────────────────────────────────────
//
// BeckoPopov — silence_buzzer=1, overload_bypass=1, lcd_escape=1, overload_restart=0,
//              over_temp_restart=0, backlight=1, alarm=0, fault_record=1, power_saving=0
static const char LV5048_P007FLAG[] = "^D0201,1,1,0,0,1,0,1,0";

// ── ^P005FWS responses ────────────────────────────────────────────────────────
//
// No active faults; eeprom warning active (w9=1, zero-indexed w[8])
static const char LV5048_P005FWS_EEPROM_WARN[] = "^D03900,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0";

// ── ^P005ET responses ─────────────────────────────────────────────────────────
//
// BeckoPopov — 802946 kWh total generated
static const char LV5048_P005ET_BECKOPOPOV[] = "^D01100802946";

// BanannaJoe — 320400 kWh total generated
static const char LV5048_P005ET_BANANNAJOE[] = "^D01100320400";

// ── ^P004T responses ──────────────────────────────────────────────────────────
//
// >>> ^P004T  (query current device time)
// <<< ^D017<YYYYMMDDHHmmss>
//
// BeckoPopov — 2023-03-20 11:46:13
static const char LV5048_P004T[] = "^D01720230320114613";

// ── ^P005ACCT responses ───────────────────────────────────────────────────────
//
// >>> ^P005ACCT  (AC charger time totals)
// <<< ^D012<charge_time>,<load_time>  (unit unknown, likely minutes)
//
// Both zero on a freshly configured system
static const char LV5048_P005ACCT[] = "^D0120000,0000";

// ── ^P005ACLT responses ───────────────────────────────────────────────────────
//
// >>> ^P005ACLT  (AC load time totals)
// <<< ^D012<load_time_1>,<load_time_2>
//
// Both zero on a freshly configured system
static const char LV5048_P005ACLT[] = "^D0120000,0000";

// ── ^P005DI responses ─────────────────────────────────────────────────────────
//
// >>> ^P005DI  (default / factory settings, 23 comma-separated fields)
// <<< ^D068<ac_in_v>,<ac_in_f>,<ac_in_range>,<bat_absorb_v>,<bat_bulk_v>,
//            <bat_float_v>,<bat_recharge_v>,<bat_redischarge_v>,<max_chg_a>,
//            <input_v_range>,<out_src_priority>,<chg_src_priority>,<bat_type>,
//            <buzzer>,<overload_bypass>,<lcd_escape>,<overload_restart>,
//            <over_temp_restart>,<backlight>,<alarm>,<fault_record>,
//            <power_saving>,<bat_equalization>
//
// BeckoPopov — 230V/50Hz grid, 48V battery (bulk=56.4V, float=54.0V, recharge=46.0V)
static const char LV5048_P005DI[] = "^D0682300,500,0,408,540,564,460,540,060,30,0,0,1,0,0,0,1,0,0,1,1,0,1,1";

// ── ^P005ID responses ─────────────────────────────────────────────────────────
//
// >>> ^P005ID  (product ID / serial number, 22-char string)
// <<< ^D025<serial_number_22_chars>
//
// BeckoPopov — serial: 1496132209100190000000
static const char LV5048_P005ID[] = "^D0251496132209100190000000";

// ── ^P005PI responses ─────────────────────────────────────────────────────────
//
// >>> ^P005PI  (product info / protocol version, 2 chars)
// <<< ^D005<protocol_id_2chars>
//
// BeckoPopov — protocol identifier "18" (PI18)
static const char LV5048_P005PI[] = "^D00518";

// ── ^P006VFW responses ────────────────────────────────────────────────────────
//
// >>> ^P006VFW  (firmware versions, 3 × 5-digit fields)
// <<< ^D020<fw_main>,<fw_secondary>,<fw_reserved>
//
// BeckoPopov — main=07601, secondary=08028, reserved=00000
static const char LV5048_P006VFW[] = "^D02007601,08028,00000";

// ── ^P007PRI responses ────────────────────────────────────────────────────────
//
// >>> ^P007PRI  (parallel rated info, per-unit)
// <<< ^D040<unit_id>,<work_mode>,<fault_code>,\x02<fw_version_str>,<...>,<...>,<...>
//
// Note: the \x02 byte (STX) appears to delimit a free-form firmware/version
// field; exact sub-field layout is not yet fully documented.
//
// BeckoPopov — unit 0, mode 1, no fault; fw-string contains "020"
static const char LV5048_P007PRI_UNIT0[] = "^D0401,00,0\x02"
                                           "001 020  00.000000,0,000,000,0";

// BeckoPopov — unit 1, mode 1, no fault; fw-string contains "010"
static const char LV5048_P007PRI_UNIT1[] = "^D0401,00,0\x02"
                                           "001 010  00.000000,0,000,000,0";

// ── ^P009MCHGCR responses ─────────────────────────────────────────────────────
//
// >>> ^P009MCHGCR  (valid max-charging-current values, comma-separated)
// <<< ^D050<a1>,<a2>,...,<aN>  (in amperes, 3-digit zero-padded)
//
// BeckoPopov — 12 steps: 10 A – 120 A in 10 A increments
static const char LV5048_P009MCHGCR[] = "^D050010,020,030,040,050,060,070,080,090,100,110,120";

// ── ^P005FWS (synthetic test vector) ─────────────────────────────────────────
//
// Not a captured frame — exercises non-zero fault_code and true paths for
// w[0..4]: line_fail, output_short, over_temp, fan_lock, bat_voltage_high.
static const char LV5048_P005FWS_MULTI_FAULT[] = "^D03905,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0";

// ── Unsupported-command responses ─────────────────────────────────────────────
//
// Commands not implemented by this firmware return a non-standard NAK:
// ^0 + CRC(2) + CR  (i.e. '^' 0x30 instead of the usual "(NAK").
// Observed for: ^P009EY, ^P011EM, ^P013ED (daily/monthly energy history).
//
// Stored as the 2-byte payload after CRC stripping:
static const char LV5048_RESPONSE_UNSUPPORTED[] = "^0";

// Standard PI18 NAK (e.g. response to malformed ^MUCHGCR):
// "(NAK" + CRC(2) + CR  — payload after CRC stripping is "(NAK"
static const char LV5048_RESPONSE_NAK[] = "(NAK";

}  // namespace esphome::pi18::testing
