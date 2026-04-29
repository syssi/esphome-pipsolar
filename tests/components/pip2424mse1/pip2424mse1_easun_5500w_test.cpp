#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "frames_easun_5500w_1ph.h"

namespace esphome::pip2424mse1::testing {

// ── QPIRI ────────────────────────────────────────────────────────────────────

TEST(Pip2424mse1Easun5500wTest, QpiriGridRatings) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_a;
  inv.set_grid_rating_voltage(&grid_v);
  inv.set_grid_rating_current(&grid_a);
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  EXPECT_FLOAT_EQ(grid_v.state, 230.0f);
  EXPECT_FLOAT_EQ(grid_a.state, 23.9f);
}

TEST(Pip2424mse1Easun5500wTest, QpiriOutputRatings) {
  TestablePipsolar inv;
  sensor::Sensor apparent, active;
  inv.set_ac_output_rating_apparent_power(&apparent);
  inv.set_ac_output_rating_active_power(&active);
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  EXPECT_FLOAT_EQ(apparent.state, 5500.0f);
  EXPECT_FLOAT_EQ(active.state, 5500.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpiriBatteryVoltages) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, recharge_v, under_v, bulk_v, float_v, redischarge_v;
  inv.set_battery_rating_voltage(&bat_v);
  inv.set_battery_recharge_voltage(&recharge_v);
  inv.set_battery_under_voltage(&under_v);
  inv.set_battery_bulk_voltage(&bulk_v);
  inv.set_battery_float_voltage(&float_v);
  inv.set_battery_redischarge_voltage(&redischarge_v);
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  EXPECT_FLOAT_EQ(bat_v.state, 48.0f);
  EXPECT_FLOAT_EQ(recharge_v.state, 51.0f);
  EXPECT_FLOAT_EQ(under_v.state, 48.0f);
  EXPECT_FLOAT_EQ(bulk_v.state, 55.3f);
  EXPECT_FLOAT_EQ(float_v.state, 55.3f);
  EXPECT_FLOAT_EQ(redischarge_v.state, 53.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpiriChargingCurrents) {
  TestablePipsolar inv;
  sensor::Sensor max_ac_chg, max_chg;
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 2.0f);
  EXPECT_FLOAT_EQ(max_chg.state, 100.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpiriSourcePriorities) {
  TestablePipsolar inv;
  sensor::Sensor out_prio, chg_prio;
  inv.set_output_source_priority(&out_prio);
  inv.set_charger_source_priority(&chg_prio);
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  EXPECT_FLOAT_EQ(out_prio.state, 2.0f);
  EXPECT_FLOAT_EQ(chg_prio.state, 3.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpiriPvConditions) {
  TestablePipsolar inv;
  sensor::Sensor pv_ok, pv_bal;
  inv.set_pv_ok_condition_for_parallel(&pv_ok);
  inv.set_pv_power_balance(&pv_bal);
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  EXPECT_FLOAT_EQ(pv_ok.state, 0.0f);
  EXPECT_FLOAT_EQ(pv_bal.state, 1.0f);
}

// ── QPIGS ────────────────────────────────────────────────────────────────────

TEST(Pip2424mse1Easun5500wTest, QpigsGridMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_hz;
  inv.set_grid_voltage(&grid_v);
  inv.set_grid_frequency(&grid_hz);
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  EXPECT_NEAR(grid_v.state, 239.2f, 0.01f);
  EXPECT_NEAR(grid_hz.state, 50.0f, 0.01f);
}

TEST(Pip2424mse1Easun5500wTest, QpigsOutputMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor out_v, out_hz, apparent, active, load_pct, bus_v;
  inv.set_ac_output_voltage(&out_v);
  inv.set_ac_output_frequency(&out_hz);
  inv.set_ac_output_apparent_power(&apparent);
  inv.set_ac_output_active_power(&active);
  inv.set_output_load_percent(&load_pct);
  inv.set_bus_voltage(&bus_v);
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  EXPECT_NEAR(out_v.state, 229.8f, 0.01f);
  EXPECT_NEAR(out_hz.state, 50.0f, 0.01f);
  EXPECT_FLOAT_EQ(apparent.state, 321.0f);
  EXPECT_FLOAT_EQ(active.state, 197.0f);
  EXPECT_FLOAT_EQ(load_pct.state, 5.0f);
  EXPECT_FLOAT_EQ(bus_v.state, 422.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpigsBatteryMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, chg_a, cap_pct, temp;
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.set_inverter_heat_sink_temperature(&temp);
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  EXPECT_NEAR(bat_v.state, 53.20f, 0.001f);
  EXPECT_FLOAT_EQ(chg_a.state, 0.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 50.0f);
  EXPECT_FLOAT_EQ(temp.state, 56.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpigsPvMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor pv_a, pv_v, scc_v, dis_a, pv_pwr;
  inv.set_pv_input_current_for_battery(&pv_a);
  inv.set_pv_input_voltage(&pv_v);
  inv.set_battery_voltage_scc(&scc_v);
  inv.set_battery_discharge_current(&dis_a);
  inv.set_pv_charging_power(&pv_pwr);
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  EXPECT_NEAR(pv_a.state, 1.7f, 0.01f);
  EXPECT_NEAR(pv_v.state, 229.2f, 0.01f);
  EXPECT_NEAR(scc_v.state, 0.0f, 0.01f);
  EXPECT_FLOAT_EQ(dis_a.state, 0.0f);
  EXPECT_FLOAT_EQ(pv_pwr.state, 407.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpigsDeviceStatus1Bits) {
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
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  // status1="00010110": bit3=load, bit5=charging, bit6=scc_charging
  EXPECT_FALSE(sbu.state);
  EXPECT_FALSE(config.state);
  EXPECT_FALSE(scc_fw.state);
  EXPECT_TRUE(load.state);
  EXPECT_FALSE(bat_steady.state);
  EXPECT_TRUE(charging.state);
  EXPECT_TRUE(scc_chg.state);
  EXPECT_FALSE(ac_chg.state);
}

TEST(Pip2424mse1Easun5500wTest, QpigsDeviceStatus2Bits) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor float_mode, sw_on, dust;
  inv.set_charging_to_floating_mode(&float_mode);
  inv.set_switch_on(&sw_on);
  inv.set_dustproof_installed(&dust);
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  // status2="010": bit1=switch_on
  EXPECT_FALSE(float_mode.state);
  EXPECT_TRUE(sw_on.state);
  EXPECT_FALSE(dust.state);
}

TEST(Pip2424mse1Easun5500wTest, QpigsMultipleCyclesPvVoltageChanges) {
  TestablePipsolar inv;
  sensor::Sensor pv_v, pv_pwr;
  inv.set_pv_input_voltage(&pv_v);
  inv.set_pv_charging_power(&pv_pwr);

  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  EXPECT_NEAR(pv_v.state, 229.2f, 0.01f);
  EXPECT_FLOAT_EQ(pv_pwr.state, 407.0f);

  inv.handle_qpigs_(EASUN_5500W_QPIGS_B);
  EXPECT_NEAR(pv_v.state, 224.7f, 0.01f);
  EXPECT_FLOAT_EQ(pv_pwr.state, 473.0f);

  inv.handle_qpigs_(EASUN_5500W_QPIGS_C);
  EXPECT_NEAR(pv_v.state, 209.8f, 0.01f);
  EXPECT_FLOAT_EQ(pv_pwr.state, 408.0f);

  inv.handle_qpigs_(EASUN_5500W_QPIGS_D);
  EXPECT_NEAR(pv_v.state, 215.9f, 0.01f);
  EXPECT_FLOAT_EQ(pv_pwr.state, 405.0f);
}

TEST(Pip2424mse1Easun5500wTest, QpigsTemperatureRisesInCycle4) {
  TestablePipsolar inv;
  sensor::Sensor temp;
  inv.set_inverter_heat_sink_temperature(&temp);

  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  EXPECT_FLOAT_EQ(temp.state, 56.0f);

  inv.handle_qpigs_(EASUN_5500W_QPIGS_D);
  EXPECT_FLOAT_EQ(temp.state, 57.0f);
}

// ── QMOD ─────────────────────────────────────────────────────────────────────

TEST(Pip2424mse1Easun5500wTest, QmodBatteryMode) {
  TestablePipsolar inv;
  text_sensor::TextSensor mode;
  inv.set_device_mode(&mode);
  inv.handle_qmod_(EASUN_5500W_QMOD);
  EXPECT_STREQ(mode.state.c_str(), "B");
}

// ── QFLAG ────────────────────────────────────────────────────────────────────

TEST(Pip2424mse1Easun5500wTest, QflagOnlyFaultRecordEnabled) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor fault_rec;
  inv.set_fault_code_record(&fault_rec);
  inv.handle_qflag_(EASUN_5500W_QFLAG);
  EXPECT_TRUE(fault_rec.state);
}

TEST(Pip2424mse1Easun5500wTest, QflagAllOtherFlagsDisabled) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor buzzer, bypass, power_save, lcd, overload_restart, over_temp, backlight, alarm;
  inv.set_silence_buzzer_open_buzzer(&buzzer);
  inv.set_overload_bypass_function(&bypass);
  inv.set_power_saving(&power_save);
  inv.set_lcd_escape_to_default(&lcd);
  inv.set_overload_restart_function(&overload_restart);
  inv.set_over_temperature_restart_function(&over_temp);
  inv.set_backlight_on(&backlight);
  inv.set_alarm_on_when_primary_source_interrupt(&alarm);
  inv.handle_qflag_(EASUN_5500W_QFLAG);
  EXPECT_FALSE(buzzer.state);
  EXPECT_FALSE(bypass.state);
  EXPECT_FALSE(power_save.state);
  EXPECT_FALSE(lcd.state);
  EXPECT_FALSE(overload_restart.state);
  EXPECT_FALSE(over_temp.state);
  EXPECT_FALSE(backlight.state);
  EXPECT_FALSE(alarm.state);
}

// ── Robustness ───────────────────────────────────────────────────────────────

TEST(Pip2424mse1Easun5500wTest, NullSensorsDoNotCrash) {
  TestablePipsolar inv;
  inv.handle_qpiri_(EASUN_5500W_QPIRI);
  inv.handle_qpigs_(EASUN_5500W_QPIGS_A);
  inv.handle_qmod_(EASUN_5500W_QMOD);
  inv.handle_qflag_(EASUN_5500W_QFLAG);
}

}  // namespace esphome::pip2424mse1::testing
