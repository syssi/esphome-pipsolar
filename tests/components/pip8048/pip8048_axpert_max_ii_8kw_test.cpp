#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "frames_axpert_max_ii_8kw.h"

namespace esphome::pip8048::testing {

// ── QPIRI ────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert8kwTest, QpiriGridRatings) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_a;
  inv.set_grid_rating_voltage(&grid_v);
  inv.set_grid_rating_current(&grid_a);
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  EXPECT_FLOAT_EQ(grid_v.state, 230.0f);
  EXPECT_FLOAT_EQ(grid_a.state, 34.7f);
}

TEST(Pip8048Axpert8kwTest, QpiriOutputRatings) {
  TestablePipsolar inv;
  sensor::Sensor apparent, active;
  inv.set_ac_output_rating_apparent_power(&apparent);
  inv.set_ac_output_rating_active_power(&active);
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  EXPECT_FLOAT_EQ(apparent.state, 8000.0f);
  EXPECT_FLOAT_EQ(active.state, 8000.0f);
}

TEST(Pip8048Axpert8kwTest, QpiriBatteryVoltages) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, recharge_v, bulk_v, float_v, redischarge_v;
  inv.set_battery_rating_voltage(&bat_v);
  inv.set_battery_recharge_voltage(&recharge_v);
  inv.set_battery_bulk_voltage(&bulk_v);
  inv.set_battery_float_voltage(&float_v);
  inv.set_battery_redischarge_voltage(&redischarge_v);
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  EXPECT_FLOAT_EQ(bat_v.state, 48.0f);
  EXPECT_FLOAT_EQ(recharge_v.state, 51.0f);
  EXPECT_FLOAT_EQ(bulk_v.state, 55.2f);
  EXPECT_FLOAT_EQ(float_v.state, 55.0f);
  EXPECT_FLOAT_EQ(redischarge_v.state, 52.0f);
}

TEST(Pip8048Axpert8kwTest, QpiriChargingCurrents) {
  TestablePipsolar inv;
  sensor::Sensor max_ac_chg, max_chg;
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 10.0f);
  EXPECT_FLOAT_EQ(max_chg.state, 40.0f);
}

TEST(Pip8048Axpert8kwTest, QpiriSourcePriorities) {
  TestablePipsolar inv;
  sensor::Sensor out_prio, chg_prio;
  inv.set_output_source_priority(&out_prio);
  inv.set_charger_source_priority(&chg_prio);
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  EXPECT_FLOAT_EQ(out_prio.state, 2.0f);
  EXPECT_FLOAT_EQ(chg_prio.state, 3.0f);
}

TEST(Pip8048Axpert8kwTest, QpiriPvConditions) {
  TestablePipsolar inv;
  sensor::Sensor pv_ok, pv_bal;
  inv.set_pv_ok_condition_for_parallel(&pv_ok);
  inv.set_pv_power_balance(&pv_bal);
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  EXPECT_FLOAT_EQ(pv_ok.state, 1.0f);
  EXPECT_FLOAT_EQ(pv_bal.state, 1.0f);
}

// ── QPIGS ────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert8kwTest, QpigsGridMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_hz;
  inv.set_grid_voltage(&grid_v);
  inv.set_grid_frequency(&grid_hz);
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  EXPECT_NEAR(grid_v.state, 222.6f, 0.01f);
  EXPECT_NEAR(grid_hz.state, 49.9f, 0.01f);
}

TEST(Pip8048Axpert8kwTest, QpigsOutputMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor out_v, out_hz, apparent, active, load_pct, bus_v;
  inv.set_ac_output_voltage(&out_v);
  inv.set_ac_output_frequency(&out_hz);
  inv.set_ac_output_apparent_power(&apparent);
  inv.set_ac_output_active_power(&active);
  inv.set_output_load_percent(&load_pct);
  inv.set_bus_voltage(&bus_v);
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  EXPECT_NEAR(out_v.state, 230.1f, 0.01f);
  EXPECT_NEAR(out_hz.state, 50.0f, 0.01f);
  EXPECT_FLOAT_EQ(apparent.state, 551.0f);
  EXPECT_FLOAT_EQ(active.state, 499.0f);
  EXPECT_FLOAT_EQ(load_pct.state, 6.0f);
  EXPECT_FLOAT_EQ(bus_v.state, 398.0f);
}

TEST(Pip8048Axpert8kwTest, QpigsBatteryMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, chg_a, cap_pct, temp;
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.set_inverter_heat_sink_temperature(&temp);
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  EXPECT_NEAR(bat_v.state, 54.30f, 0.001f);
  EXPECT_FLOAT_EQ(chg_a.state, 0.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 61.0f);
  EXPECT_FLOAT_EQ(temp.state, 39.0f);
}

TEST(Pip8048Axpert8kwTest, QpigsPv1Measurements) {
  TestablePipsolar inv;
  sensor::Sensor pv1_a, pv1_v, scc_v, dis_a, pv1_pwr;
  inv.set_pv1_input_current(&pv1_a);
  inv.set_pv1_input_voltage(&pv1_v);
  inv.set_battery_voltage_scc(&scc_v);
  inv.set_battery_discharge_current(&dis_a);
  inv.set_pv1_charging_power(&pv1_pwr);
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  EXPECT_NEAR(pv1_a.state, 1.0f, 0.01f);
  EXPECT_NEAR(pv1_v.state, 214.4f, 0.01f);
  EXPECT_NEAR(scc_v.state, 0.0f, 0.01f);
  EXPECT_FLOAT_EQ(dis_a.state, 0.0f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 226.0f);
}

TEST(Pip8048Axpert8kwTest, QpigsDeviceStatus1Bits) {
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
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
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

TEST(Pip8048Axpert8kwTest, QpigsDeviceStatus2Bits) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor float_mode, sw_on, dust;
  inv.set_charging_to_floating_mode(&float_mode);
  inv.set_switch_on(&sw_on);
  inv.set_dustproof_installed(&dust);
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  // status2="010": bit1=switch_on
  EXPECT_FALSE(float_mode.state);
  EXPECT_TRUE(sw_on.state);
  EXPECT_FALSE(dust.state);
}

TEST(Pip8048Axpert8kwTest, QpigsMultipleCyclesPv1VoltageChanges) {
  TestablePipsolar inv;
  sensor::Sensor pv1_v, pv1_pwr;
  inv.set_pv1_input_voltage(&pv1_v);
  inv.set_pv1_charging_power(&pv1_pwr);

  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  EXPECT_NEAR(pv1_v.state, 214.4f, 0.01f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 226.0f);

  inv.handle_qpigs_(AXPERT_8KW_QPIGS_B);
  EXPECT_NEAR(pv1_v.state, 214.4f, 0.01f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 227.0f);

  inv.handle_qpigs_(AXPERT_8KW_QPIGS_C);
  EXPECT_NEAR(pv1_v.state, 215.0f, 0.01f);
  EXPECT_FLOAT_EQ(pv1_pwr.state, 226.0f);
}

// ── QPIGS2 ───────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert8kwTest, Qpigs2Pv2Measurements) {
  TestablePipsolar inv;
  sensor::Sensor pv2_a, pv2_v, pv2_pwr;
  inv.set_pv2_input_current(&pv2_a);
  inv.set_pv2_input_voltage(&pv2_v);
  inv.set_pv2_charging_power(&pv2_pwr);
  inv.handle_qpigs2_(AXPERT_8KW_QPIGS2_A);
  EXPECT_NEAR(pv2_a.state, 1.0f, 0.01f);
  EXPECT_NEAR(pv2_v.state, 202.0f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 205.0f);
}

TEST(Pip8048Axpert8kwTest, Qpigs2MultipleCyclesVoltageVariation) {
  TestablePipsolar inv;
  sensor::Sensor pv2_v, pv2_pwr;
  inv.set_pv2_input_voltage(&pv2_v);
  inv.set_pv2_charging_power(&pv2_pwr);

  inv.handle_qpigs2_(AXPERT_8KW_QPIGS2_A);
  EXPECT_NEAR(pv2_v.state, 202.0f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 205.0f);

  inv.handle_qpigs2_(AXPERT_8KW_QPIGS2_B);
  EXPECT_NEAR(pv2_v.state, 201.7f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 205.0f);

  inv.handle_qpigs2_(AXPERT_8KW_QPIGS2_C);
  EXPECT_NEAR(pv2_v.state, 198.8f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 204.0f);

  inv.handle_qpigs2_(AXPERT_8KW_QPIGS2_D);
  EXPECT_NEAR(pv2_v.state, 201.0f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 205.0f);
}

// ── QMOD ─────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert8kwTest, QmodBatteryMode) {
  TestablePipsolar inv;
  text_sensor::TextSensor mode;
  inv.set_device_mode(&mode);
  inv.handle_qmod_(AXPERT_8KW_QMOD);
  EXPECT_STREQ(mode.state.c_str(), "B");
}

// ── QFLAG ────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert8kwTest, QflagEnabledFlags) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor bypass, lcd, overload_restart, fault_rec;
  inv.set_overload_bypass_function(&bypass);
  inv.set_lcd_escape_to_default(&lcd);
  inv.set_overload_restart_function(&overload_restart);
  inv.set_fault_code_record(&fault_rec);
  inv.handle_qflag_(AXPERT_8KW_QFLAG);
  EXPECT_TRUE(bypass.state);
  EXPECT_TRUE(lcd.state);
  EXPECT_TRUE(overload_restart.state);
  EXPECT_TRUE(fault_rec.state);
}

TEST(Pip8048Axpert8kwTest, QflagDisabledFlags) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor buzzer, power_save, over_temp, backlight, alarm;
  inv.set_silence_buzzer_open_buzzer(&buzzer);
  inv.set_power_saving(&power_save);
  inv.set_over_temperature_restart_function(&over_temp);
  inv.set_backlight_on(&backlight);
  inv.set_alarm_on_when_primary_source_interrupt(&alarm);
  inv.handle_qflag_(AXPERT_8KW_QFLAG);
  EXPECT_FALSE(buzzer.state);
  EXPECT_FALSE(power_save.state);
  EXPECT_FALSE(over_temp.state);
  EXPECT_FALSE(backlight.state);
  EXPECT_FALSE(alarm.state);
}

// ── Robustness ───────────────────────────────────────────────────────────────

TEST(Pip8048Axpert8kwTest, NullSensorsDoNotCrash) {
  TestablePipsolar inv;
  // All handlers must tolerate null sensor pointers
  inv.handle_qpiri_(AXPERT_8KW_QPIRI);
  inv.handle_qpigs_(AXPERT_8KW_QPIGS_A);
  inv.handle_qpigs2_(AXPERT_8KW_QPIGS2_A);
  inv.handle_qmod_(AXPERT_8KW_QMOD);
  inv.handle_qflag_(AXPERT_8KW_QFLAG);
}

}  // namespace esphome::pip8048::testing
