#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "frames_powmr_4200w.h"

namespace esphome::pip8048::testing {

// ── QPIRI ────────────────────────────────────────────────────────────────────

TEST(Pip8048Powmr4200wTest, QpiriGridRatings) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_a;
  inv.set_grid_rating_voltage(&grid_v);
  inv.set_grid_rating_current(&grid_a);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  EXPECT_FLOAT_EQ(grid_v.state, 230.0f);
  EXPECT_FLOAT_EQ(grid_a.state, 10.0f);
}

TEST(Pip8048Powmr4200wTest, QpiriOutputRatings) {
  TestablePipsolar inv;
  sensor::Sensor apparent, active;
  inv.set_ac_output_rating_apparent_power(&apparent);
  inv.set_ac_output_rating_active_power(&active);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  EXPECT_FLOAT_EQ(apparent.state, 2300.0f);
  EXPECT_FLOAT_EQ(active.state, 2300.0f);
}

TEST(Pip8048Powmr4200wTest, QpiriBatteryVoltages) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, recharge_v, bulk_v, float_v, redischarge_v;
  inv.set_battery_rating_voltage(&bat_v);
  inv.set_battery_recharge_voltage(&recharge_v);
  inv.set_battery_bulk_voltage(&bulk_v);
  inv.set_battery_float_voltage(&float_v);
  inv.set_battery_redischarge_voltage(&redischarge_v);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  EXPECT_FLOAT_EQ(bat_v.state, 12.0f);
  EXPECT_NEAR(recharge_v.state, 12.8f, 0.01f);
  EXPECT_NEAR(bulk_v.state, 14.2f, 0.01f);
  EXPECT_NEAR(float_v.state, 13.6f, 0.01f);
  EXPECT_NEAR(redischarge_v.state, 13.3f, 0.01f);
}

TEST(Pip8048Powmr4200wTest, QpiriChargingCurrents100A) {
  TestablePipsolar inv;
  sensor::Sensor max_ac_chg, max_chg;
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 20.0f);
  EXPECT_FLOAT_EQ(max_chg.state, 100.0f);
}

// Verifies that zero-padded "090" is parsed correctly to 90A
TEST(Pip8048Powmr4200wTest, QpiriChargingCurrentZeroPadded) {
  TestablePipsolar inv;
  sensor::Sensor max_ac_chg, max_chg;
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_090A);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 20.0f);
  EXPECT_FLOAT_EQ(max_chg.state, 90.0f);
}

TEST(Pip8048Powmr4200wTest, QpiriSourcePriorities) {
  TestablePipsolar inv;
  sensor::Sensor out_prio, chg_prio;
  inv.set_output_source_priority(&out_prio);
  inv.set_charger_source_priority(&chg_prio);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  EXPECT_FLOAT_EQ(out_prio.state, 2.0f);
  EXPECT_FLOAT_EQ(chg_prio.state, 2.0f);
}

TEST(Pip8048Powmr4200wTest, QpiriPvConditions) {
  TestablePipsolar inv;
  sensor::Sensor pv_ok, pv_bal;
  inv.set_pv_ok_condition_for_parallel(&pv_ok);
  inv.set_pv_power_balance(&pv_bal);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  EXPECT_FLOAT_EQ(pv_ok.state, 0.0f);
  EXPECT_FLOAT_EQ(pv_bal.state, 1.0f);
}

// ── QPIGS ────────────────────────────────────────────────────────────────────

TEST(Pip8048Powmr4200wTest, QpigsDischargeModeOutputMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_hz, out_v, out_hz, apparent, active, load_pct, bus_v;
  inv.set_grid_voltage(&grid_v);
  inv.set_grid_frequency(&grid_hz);
  inv.set_ac_output_voltage(&out_v);
  inv.set_ac_output_frequency(&out_hz);
  inv.set_ac_output_apparent_power(&apparent);
  inv.set_ac_output_active_power(&active);
  inv.set_output_load_percent(&load_pct);
  inv.set_bus_voltage(&bus_v);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_DISCHARGE);
  EXPECT_NEAR(grid_v.state, 224.4f, 0.01f);
  EXPECT_NEAR(grid_hz.state, 50.0f, 0.01f);
  EXPECT_NEAR(out_v.state, 229.2f, 0.01f);
  EXPECT_NEAR(out_hz.state, 50.0f, 0.01f);
  EXPECT_FLOAT_EQ(apparent.state, 366.0f);
  EXPECT_FLOAT_EQ(active.state, 283.0f);
  EXPECT_FLOAT_EQ(load_pct.state, 16.0f);
  EXPECT_FLOAT_EQ(bus_v.state, 379.0f);
}

TEST(Pip8048Powmr4200wTest, QpirsDischargeBatteryMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, chg_a, cap_pct, temp, dis_a;
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.set_inverter_heat_sink_temperature(&temp);
  inv.set_battery_discharge_current(&dis_a);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_DISCHARGE);
  EXPECT_NEAR(bat_v.state, 13.10f, 0.001f);
  EXPECT_FLOAT_EQ(chg_a.state, 0.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 80.0f);
  EXPECT_FLOAT_EQ(temp.state, 25.0f);
  EXPECT_FLOAT_EQ(dis_a.state, 24.0f);
}

TEST(Pip8048Powmr4200wTest, QpigsDischargePvZero) {
  TestablePipsolar inv;
  sensor::Sensor pv1_a, pv1_v, scc_v, pv1_pwr;
  inv.set_pv1_input_current(&pv1_a);
  inv.set_pv1_input_voltage(&pv1_v);
  inv.set_battery_voltage_scc(&scc_v);
  inv.set_pv1_charging_power(&pv1_pwr);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_DISCHARGE);
  EXPECT_FLOAT_EQ(pv1_a.state, 0.0f);
  EXPECT_FLOAT_EQ(pv1_v.state, 0.0f);
  EXPECT_FLOAT_EQ(scc_v.state, 0.0f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 1.0f);
}

TEST(Pip8048Powmr4200wTest, QpigsDischargeStatusBits) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor sbu, config, scc_fw, load, bat_steady, charging, scc_chg, ac_chg;
  inv.set_add_sbu_priority_version(&sbu);
  inv.set_configuration_status(&config);
  inv.set_scc_firmware_version(&scc_fw);
  inv.set_load_status(&load);
  inv.set_battery_voltage_to_steady_while_charging(&bat_steady);
  inv.set_charging_status(&charging);
  inv.set_scc_charging_status(&scc_chg);
  inv.set_ac_charging_status(&ac_chg);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_DISCHARGE);
  // status1="00010000": only bit3(load) set
  EXPECT_FALSE(sbu.state);
  EXPECT_FALSE(config.state);
  EXPECT_FALSE(scc_fw.state);
  EXPECT_TRUE(load.state);
  EXPECT_FALSE(bat_steady.state);
  EXPECT_FALSE(charging.state);
  EXPECT_FALSE(scc_chg.state);
  EXPECT_FALSE(ac_chg.state);
}

TEST(Pip8048Powmr4200wTest, QpigsDischargeStatus2SwitchOn) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor float_mode, sw_on, dust;
  inv.set_charging_to_floating_mode(&float_mode);
  inv.set_switch_on(&sw_on);
  inv.set_dustproof_installed(&dust);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_DISCHARGE);
  // status2="010": bit1=switch_on
  EXPECT_FALSE(float_mode.state);
  EXPECT_TRUE(sw_on.state);
  EXPECT_FALSE(dust.state);
}

TEST(Pip8048Powmr4200wTest, QpigsSolarChargingMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, chg_a, cap_pct, pv1_a, pv1_v, pv1_pwr, dis_a;
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.set_pv1_input_current(&pv1_a);
  inv.set_pv1_input_voltage(&pv1_v);
  inv.set_pv1_charging_power(&pv1_pwr);
  inv.set_battery_discharge_current(&dis_a);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_SOLAR_A);
  EXPECT_NEAR(bat_v.state, 13.00f, 0.001f);
  EXPECT_FLOAT_EQ(chg_a.state, 10.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 76.0f);
  EXPECT_NEAR(pv1_a.state, 11.0f, 0.01f);
  EXPECT_NEAR(pv1_v.state, 134.3f, 0.01f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 177.0f);
  EXPECT_FLOAT_EQ(dis_a.state, 0.0f);
}

TEST(Pip8048Powmr4200wTest, QpigsSolarChargingStatusBits) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor load, charging, scc_chg, ac_chg;
  inv.set_load_status(&load);
  inv.set_charging_status(&charging);
  inv.set_scc_charging_status(&scc_chg);
  inv.set_ac_charging_status(&ac_chg);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_SOLAR_A);
  // status1="00010110": bit3(load)+bit5(charging)+bit6(scc_charging)
  EXPECT_TRUE(load.state);
  EXPECT_TRUE(charging.state);
  EXPECT_TRUE(scc_chg.state);
  EXPECT_FALSE(ac_chg.state);
}

TEST(Pip8048Powmr4200wTest, QpigsMultipleCyclesPv1VoltageChanges) {
  TestablePipsolar inv;
  sensor::Sensor pv1_v, pv1_pwr, bat_v, chg_a;
  inv.set_pv1_input_voltage(&pv1_v);
  inv.set_pv1_charging_power(&pv1_pwr);
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);

  inv.handle_qpigs_(POWMR_4200W_QPIGS_SOLAR_A);
  EXPECT_NEAR(pv1_v.state, 134.3f, 0.01f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 177.0f);
  EXPECT_NEAR(bat_v.state, 13.00f, 0.001f);
  EXPECT_FLOAT_EQ(chg_a.state, 10.0f);

  inv.handle_qpigs_(POWMR_4200W_QPIGS_SOLAR_B);
  EXPECT_NEAR(pv1_v.state, 131.9f, 0.01f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 177.0f);
  EXPECT_NEAR(bat_v.state, 13.10f, 0.001f);
  EXPECT_FLOAT_EQ(chg_a.state, 11.0f);
}

// ── QMOD ─────────────────────────────────────────────────────────────────────

TEST(Pip8048Powmr4200wTest, QmodBatteryMode) {
  TestablePipsolar inv;
  text_sensor::TextSensor mode;
  inv.set_device_mode(&mode);
  inv.handle_qmod_(POWMR_4200W_QMOD);
  EXPECT_STREQ(mode.state.c_str(), "B");
}

// ── QFLAG ────────────────────────────────────────────────────────────────────

TEST(Pip8048Powmr4200wTest, QflagEnabledFlags) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor buzzer, overload_restart, backlight, alarm, fault_rec;
  inv.set_silence_buzzer_open_buzzer(&buzzer);
  inv.set_overload_restart_function(&overload_restart);
  inv.set_backlight_on(&backlight);
  inv.set_alarm_on_when_primary_source_interrupt(&alarm);
  inv.set_fault_code_record(&fault_rec);
  inv.handle_qflag_(POWMR_4200W_QFLAG);
  // "(EauxyzDbjkv;" — enabled: a,u,x,y,z
  EXPECT_TRUE(buzzer.state);
  EXPECT_TRUE(overload_restart.state);
  EXPECT_TRUE(backlight.state);
  EXPECT_TRUE(alarm.state);
  EXPECT_TRUE(fault_rec.state);
}

TEST(Pip8048Powmr4200wTest, QflagDisabledFlags) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor bypass, lcd, power_save, over_temp;
  inv.set_overload_bypass_function(&bypass);
  inv.set_lcd_escape_to_default(&lcd);
  inv.set_power_saving(&power_save);
  inv.set_over_temperature_restart_function(&over_temp);
  inv.handle_qflag_(POWMR_4200W_QFLAG);
  // "(EauxyzDbjkv;" — disabled: b,j,k,v
  EXPECT_FALSE(bypass.state);
  EXPECT_FALSE(lcd.state);
  EXPECT_FALSE(power_save.state);
  EXPECT_FALSE(over_temp.state);
}

// ── QMCHGCR / QMUCHGCR ───────────────────────────────────────────────────────

TEST(Pip8048Powmr4200wTest, QmchgcrRawFrame) {
  TestablePipsolar inv;
  text_sensor::TextSensor last;
  inv.set_last_qmchgcr(&last);
  inv.handle_qmchgcr_(POWMR_4200W_QMCHGCR);
  EXPECT_STREQ(last.state.c_str(), "(002 010 020 030 040 050 060 070 080 090 100");
}

TEST(Pip8048Powmr4200wTest, QmuchgcrRawFrame) {
  TestablePipsolar inv;
  text_sensor::TextSensor last;
  inv.set_last_qmuchgcr(&last);
  inv.handle_qmuchgcr_(POWMR_4200W_QMUCHGCR);
  EXPECT_STREQ(last.state.c_str(), "(010 020 030 040 050 060 070 080 090 100");
}

// ── Robustness ───────────────────────────────────────────────────────────────

TEST(Pip8048Powmr4200wTest, NullSensorsDoNotCrash) {
  TestablePipsolar inv;
  inv.handle_qpiri_(POWMR_4200W_QPIRI_100A);
  inv.handle_qpiri_(POWMR_4200W_QPIRI_090A);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_DISCHARGE);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_SOLAR_A);
  inv.handle_qpigs_(POWMR_4200W_QPIGS_SOLAR_B);
  inv.handle_qmod_(POWMR_4200W_QMOD);
  inv.handle_qflag_(POWMR_4200W_QFLAG);
  inv.handle_qmchgcr_(POWMR_4200W_QMCHGCR);
  inv.handle_qmuchgcr_(POWMR_4200W_QMUCHGCR);
}

}  // namespace esphome::pip8048::testing
