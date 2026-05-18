#include <gtest/gtest.h>
#include <cmath>
#include "common.h"
#include "frames_lv5048.h"

namespace esphome::pi18::testing {

// ── P005GS ────────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P005gsGridMeasurements) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_hz;
  inv.set_grid_voltage(&grid_v);
  inv.set_grid_frequency(&grid_hz);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_NEAR(grid_v.state, 231.2f, 0.01f);
  EXPECT_NEAR(grid_hz.state, 50.0f, 0.01f);
}

TEST(Pi18Lv5048Test, P005gsOutputZeroInStandby) {
  TestablePipsolar inv;
  sensor::Sensor out_v, out_hz, apparent, active, load_pct;
  inv.set_ac_output_voltage(&out_v);
  inv.set_ac_output_frequency(&out_hz);
  inv.set_ac_output_apparent_power(&apparent);
  inv.set_ac_output_active_power(&active);
  inv.set_output_load_percent(&load_pct);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_FLOAT_EQ(out_v.state, 0.0f);
  EXPECT_FLOAT_EQ(out_hz.state, 0.0f);
  EXPECT_FLOAT_EQ(apparent.state, 0.0f);
  EXPECT_FLOAT_EQ(active.state, 0.0f);
  EXPECT_FLOAT_EQ(load_pct.state, 0.0f);
}

TEST(Pi18Lv5048Test, P005gsBatteryCharging) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, chg_a, cap_pct, heat;
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.set_inverter_heat_sink_temperature(&heat);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_NEAR(bat_v.state, 51.8f, 0.01f);
  EXPECT_FLOAT_EQ(chg_a.state, 4.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 36.0f);
  EXPECT_FLOAT_EQ(heat.state, 27.0f);
}

TEST(Pi18Lv5048Test, P005gsBatteryDischargeCurrentNegated) {
  TestablePipsolar inv;
  sensor::Sensor dis_a;
  inv.set_battery_discharge_current(&dis_a);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_FLOAT_EQ(dis_a.state, 0.0f);
}

TEST(Pi18Lv5048Test, P005gsPvInputStandby) {
  TestablePipsolar inv;
  sensor::Sensor pv1_pwr, pv1_v, pv2_pwr, pv2_v;
  inv.set_pv1_input_power(&pv1_pwr);
  inv.set_pv1_input_voltage(&pv1_v);
  inv.set_pv2_input_power(&pv2_pwr);
  inv.set_pv2_input_voltage(&pv2_v);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_NEAR(pv1_pwr.state, 302.0f, 0.1f);
  EXPECT_NEAR(pv1_v.state, 243.8f, 0.01f);
  EXPECT_NEAR(pv2_pwr.state, 0.0f, 0.1f);
  EXPECT_NEAR(pv2_v.state, 0.0f, 0.01f);
}

TEST(Pi18Lv5048Test, P005gsMpptChargerStatus) {
  TestablePipsolar inv;
  text_sensor::TextSensor mppt1, mppt2;
  inv.set_mppt1_charger_status(&mppt1);
  inv.set_mppt2_charger_status(&mppt2);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_STREQ(mppt1.state.c_str(), "charging");
  EXPECT_STREQ(mppt2.state.c_str(), "abnormal");
}

TEST(Pi18Lv5048Test, P005gsLoadConnection) {
  TestablePipsolar inv;
  text_sensor::TextSensor load;
  inv.set_load_connection(&load);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_STREQ(load.state.c_str(), "disconnect");

  TestablePipsolar inv2;
  text_sensor::TextSensor load2;
  inv2.set_load_connection(&load2);
  inv2.handle_p005gs_(LV5048_P005GS_ACTIVE);
  EXPECT_STREQ(load2.state.c_str(), "connect");
}

TEST(Pi18Lv5048Test, P005gsBatteryPowerDirection) {
  TestablePipsolar inv;
  text_sensor::TextSensor bat_dir;
  inv.set_battery_power_direction(&bat_dir);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_STREQ(bat_dir.state.c_str(), "charge");
}

TEST(Pi18Lv5048Test, P005gsDcAcPowerDirection) {
  TestablePipsolar inv;
  text_sensor::TextSensor dc_ac;
  inv.set_dc_ac_power_direction(&dc_ac);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_STREQ(dc_ac.state.c_str(), "DC-AC");
}

TEST(Pi18Lv5048Test, P005gsLinePowerDirection) {
  TestablePipsolar inv;
  text_sensor::TextSensor line;
  inv.set_line_power_direction(&line);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_STREQ(line.state.c_str(), "output");
}

TEST(Pi18Lv5048Test, P005gsAcOutputFrequencyActive) {
  TestablePipsolar inv;
  sensor::Sensor out_hz;
  inv.set_ac_output_frequency(&out_hz);
  inv.handle_p005gs_(LV5048_P005GS_ACTIVE);
  EXPECT_NEAR(out_hz.state, 49.9f, 0.01f);
}

TEST(Pi18Lv5048Test, P005gsZeroFieldsInStandby) {
  TestablePipsolar inv;
  sensor::Sensor bat_scc, bat_scc2, mppt1_temp, mppt2_temp, pv2_pwr, pv2_v;
  binary_sensor::BinarySensor config_state;
  inv.set_battery_voltage_scc(&bat_scc);
  inv.set_battery_voltage_scc2(&bat_scc2);
  inv.set_mppt1_charger_temperature(&mppt1_temp);
  inv.set_mppt2_charger_temperature(&mppt2_temp);
  inv.set_pv2_input_power(&pv2_pwr);
  inv.set_pv2_input_voltage(&pv2_v);
  inv.set_setting_value_configuration_state(&config_state);
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  EXPECT_FLOAT_EQ(bat_scc.state, 0.0f);
  EXPECT_FLOAT_EQ(bat_scc2.state, 0.0f);
  EXPECT_FLOAT_EQ(mppt1_temp.state, 0.0f);
  EXPECT_FLOAT_EQ(mppt2_temp.state, 0.0f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 0.0f);
  EXPECT_FLOAT_EQ(pv2_v.state, 0.0f);
  EXPECT_FALSE(config_state.state);
}

TEST(Pi18Lv5048Test, P005gsActiveLoadBeckoPopov) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, out_v, apparent, active, load_pct, bat_v, chg_a, cap_pct;
  inv.set_grid_voltage(&grid_v);
  inv.set_ac_output_voltage(&out_v);
  inv.set_ac_output_apparent_power(&apparent);
  inv.set_ac_output_active_power(&active);
  inv.set_output_load_percent(&load_pct);
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.handle_p005gs_(LV5048_P005GS_ACTIVE);
  EXPECT_NEAR(grid_v.state, 242.2f, 0.01f);
  EXPECT_NEAR(out_v.state, 230.5f, 0.01f);
  EXPECT_FLOAT_EQ(apparent.state, 1475.0f);
  EXPECT_FLOAT_EQ(active.state, 1458.0f);
  EXPECT_FLOAT_EQ(load_pct.state, 26.0f);
  EXPECT_NEAR(bat_v.state, 52.6f, 0.01f);
  EXPECT_FLOAT_EQ(chg_a.state, 7.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 84.0f);
}

TEST(Pi18Lv5048Test, P005gsChargingCycleValues) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, chg_a, cap_pct, pv1_pwr, pv1_v;
  inv.set_battery_voltage(&bat_v);
  inv.set_battery_charging_current(&chg_a);
  inv.set_battery_capacity_percent(&cap_pct);
  inv.set_pv1_input_power(&pv1_pwr);
  inv.set_pv1_input_voltage(&pv1_v);
  inv.handle_p005gs_(LV5048_P005GS_CHARGING);
  EXPECT_NEAR(bat_v.state, 52.2f, 0.01f);
  EXPECT_FLOAT_EQ(chg_a.state, 9.0f);
  EXPECT_FLOAT_EQ(cap_pct.state, 40.0f);
  EXPECT_NEAR(pv1_pwr.state, 557.0f, 0.1f);
  EXPECT_NEAR(pv1_v.state, 243.6f, 0.01f);
}

// ── P007PIRI ──────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P007piriRatedVoltagesAndCurrents) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_a, out_v, out_hz, out_a, apparent, active;
  inv.set_grid_rating_voltage(&grid_v);
  inv.set_grid_rating_current(&grid_a);
  inv.set_ac_output_rating_voltage(&out_v);
  inv.set_ac_output_rating_frequency(&out_hz);
  inv.set_ac_output_rating_current(&out_a);
  inv.set_ac_output_rating_apparent_power(&apparent);
  inv.set_ac_output_rating_active_power(&active);
  inv.handle_p007piri_(LV5048_P007PIRI_BANANNAJOE);
  EXPECT_NEAR(grid_v.state, 230.0f, 0.01f);
  EXPECT_NEAR(grid_a.state, 24.3f, 0.01f);
  EXPECT_NEAR(out_v.state, 230.0f, 0.01f);
  EXPECT_NEAR(out_hz.state, 50.0f, 0.01f);
  EXPECT_NEAR(out_a.state, 24.3f, 0.01f);
  EXPECT_FLOAT_EQ(apparent.state, 5600.0f);
  EXPECT_FLOAT_EQ(active.state, 5600.0f);
}

TEST(Pi18Lv5048Test, P007piriBatteryVoltagesBanannaJoe) {
  TestablePipsolar inv;
  sensor::Sensor rating_v, recharge_v, redischarge_v, under_v, bulk_v, float_v;
  inv.set_battery_rating_voltage(&rating_v);
  inv.set_battery_recharge_voltage(&recharge_v);
  inv.set_battery_redischarge_voltage(&redischarge_v);
  inv.set_battery_under_voltage(&under_v);
  inv.set_battery_bulk_voltage(&bulk_v);
  inv.set_battery_float_voltage(&float_v);
  inv.handle_p007piri_(LV5048_P007PIRI_BANANNAJOE);
  EXPECT_NEAR(rating_v.state, 48.0f, 0.01f);
  EXPECT_NEAR(recharge_v.state, 50.0f, 0.01f);
  EXPECT_NEAR(redischarge_v.state, 52.0f, 0.01f);
  EXPECT_NEAR(under_v.state, 48.0f, 0.01f);
  EXPECT_NEAR(bulk_v.state, 55.4f, 0.01f);
  EXPECT_NEAR(float_v.state, 54.0f, 0.01f);
}

TEST(Pi18Lv5048Test, P007piriChargingCurrentsBanannaJoe) {
  TestablePipsolar inv;
  sensor::Sensor max_ac_chg, max_chg;
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_p007piri_(LV5048_P007PIRI_BANANNAJOE);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 10.0f);
  EXPECT_FLOAT_EQ(max_chg.state, 60.0f);
}

TEST(Pi18Lv5048Test, P007piriSourcePrioritiesBanannaJoe) {
  TestablePipsolar inv;
  sensor::Sensor out_prio, chg_prio;
  inv.set_output_source_priority(&out_prio);
  inv.set_charger_source_priority(&chg_prio);
  inv.handle_p007piri_(LV5048_P007PIRI_BANANNAJOE);
  EXPECT_FLOAT_EQ(out_prio.state, 0.0f);  // 0=utility
  EXPECT_FLOAT_EQ(chg_prio.state, 2.0f);  // 2=solar first
}

TEST(Pi18Lv5048Test, P007piriUntestedFieldsBanannaJoe) {
  TestablePipsolar inv;
  sensor::Sensor bat_type, par_max, machine, topo, out_mode, solar_prio, mppt_str;
  inv.set_battery_type(&bat_type);
  inv.set_parallel_max_num(&par_max);
  inv.set_machine_type(&machine);
  inv.set_topology(&topo);
  inv.set_output_mode(&out_mode);
  inv.set_solar_power_priority(&solar_prio);
  inv.set_mppt_string(&mppt_str);
  inv.handle_p007piri_(LV5048_P007PIRI_BANANNAJOE);
  EXPECT_FLOAT_EQ(bat_type.state, 2.0f);
  EXPECT_FLOAT_EQ(par_max.state, 9.0f);
  EXPECT_FLOAT_EQ(machine.state, 1.0f);
  EXPECT_FLOAT_EQ(topo.state, 0.0f);
  EXPECT_FLOAT_EQ(out_mode.state, 0.0f);
  EXPECT_FLOAT_EQ(solar_prio.state, 0.0f);
  EXPECT_FLOAT_EQ(mppt_str.state, 1.0f);
}

TEST(Pi18Lv5048Test, P007piriUntestedFieldsBeckoPopov) {
  TestablePipsolar inv;
  sensor::Sensor bat_type, machine, input_range, out_prio, out_mode, mppt_str, max_ac_chg;
  inv.set_battery_type(&bat_type);
  inv.set_machine_type(&machine);
  inv.set_input_voltage_range(&input_range);
  inv.set_output_source_priority(&out_prio);
  inv.set_output_mode(&out_mode);
  inv.set_mppt_string(&mppt_str);
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.handle_p007piri_(LV5048_P007PIRI_BECKOPOPOV);
  EXPECT_FLOAT_EQ(bat_type.state, 2.0f);
  EXPECT_FLOAT_EQ(machine.state, 0.0f);
  EXPECT_FLOAT_EQ(input_range.state, 1.0f);
  EXPECT_FLOAT_EQ(out_prio.state, 1.0f);
  EXPECT_FLOAT_EQ(out_mode.state, 1.0f);
  EXPECT_FLOAT_EQ(mppt_str.state, 1.0f);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 30.0f);
}

TEST(Pi18Lv5048Test, P007piriBatteryVoltagesBeckoPopov) {
  TestablePipsolar inv;
  sensor::Sensor bulk_v, float_v, max_chg;
  inv.set_battery_bulk_voltage(&bulk_v);
  inv.set_battery_float_voltage(&float_v);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_p007piri_(LV5048_P007PIRI_BECKOPOPOV);
  EXPECT_NEAR(bulk_v.state, 55.5f, 0.01f);
  EXPECT_NEAR(float_v.state, 54.4f, 0.01f);
  EXPECT_FLOAT_EQ(max_chg.state, 90.0f);
}

// ── P006MOD ───────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P006modBatteryMode) {
  TestablePipsolar inv;
  text_sensor::TextSensor mode;
  sensor::Sensor mode_id;
  inv.set_device_mode(&mode);
  inv.set_device_mode_id(&mode_id);
  inv.handle_p006mod_(LV5048_P006MOD_BATTERY);
  EXPECT_STREQ(mode.state.c_str(), "Battery mode");
  EXPECT_EQ(mode_id.state, 3);
}

TEST(Pi18Lv5048Test, P006modHybridMode) {
  TestablePipsolar inv;
  text_sensor::TextSensor mode;
  sensor::Sensor mode_id;
  inv.set_device_mode(&mode);
  inv.set_device_mode_id(&mode_id);
  inv.handle_p006mod_(LV5048_P006MOD_HYBRID);
  EXPECT_STREQ(mode.state.c_str(), "Hybrid mode");
  EXPECT_EQ(mode_id.state, 5);
}

// ── P007FLAG ──────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P007flagEnabledFlags) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor buzzer, bypass, lcd;
  binary_sensor::BinarySensor backlight, fault_rec;
  inv.set_silence_buzzer_open_buzzer(&buzzer);
  inv.set_overload_bypass_function(&bypass);
  inv.set_lcd_escape_to_default(&lcd);
  inv.set_backlight_on(&backlight);
  inv.set_fault_code_record(&fault_rec);
  inv.handle_p007flag_(LV5048_P007FLAG);
  EXPECT_TRUE(buzzer.state);
  EXPECT_TRUE(bypass.state);
  EXPECT_TRUE(lcd.state);
  EXPECT_TRUE(backlight.state);
  EXPECT_TRUE(fault_rec.state);
}

TEST(Pi18Lv5048Test, P007flagDisabledFlags) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor overload_restart, over_temp, alarm, power_save;
  inv.set_overload_restart_function(&overload_restart);
  inv.set_over_temperature_restart_function(&over_temp);
  inv.set_alarm_on_when_primary_source_interrupt(&alarm);
  inv.set_power_saving(&power_save);
  inv.handle_p007flag_(LV5048_P007FLAG);
  EXPECT_FALSE(overload_restart.state);
  EXPECT_FALSE(over_temp.state);
  EXPECT_FALSE(alarm.state);
  EXPECT_FALSE(power_save.state);
}

// ── P005FWS ───────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P005fwsEepromWarningActive) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor eeprom;
  sensor::Sensor fault_code;
  inv.set_warning_eeprom_failed(&eeprom);
  inv.set_fault_code(&fault_code);
  inv.handle_p005fws_(LV5048_P005FWS_EEPROM_WARN);
  EXPECT_TRUE(eeprom.state);
  EXPECT_FLOAT_EQ(fault_code.state, 0.0f);
}

TEST(Pi18Lv5048Test, P005fwsAllOtherWarningsClear) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor line_fail, output_short, over_temp, fan_lock;
  binary_sensor::BinarySensor bat_high, bat_low, bat_shutdown, overload;
  binary_sensor::BinarySensor power_limit, pv1_high, pv2_high;
  binary_sensor::BinarySensor mppt1_ovld, mppt2_ovld, scc1_low, scc2_low;
  inv.set_warning_line_fail(&line_fail);
  inv.set_warning_output_circuit_short(&output_short);
  inv.set_warning_over_temperature(&over_temp);
  inv.set_warning_fan_lock(&fan_lock);
  inv.set_warning_battery_voltage_high(&bat_high);
  inv.set_warning_battery_low_alarm(&bat_low);
  inv.set_warning_battery_under_shutdown(&bat_shutdown);
  inv.set_warning_over_load(&overload);
  inv.set_warning_power_limit(&power_limit);
  inv.set_warning_pv1_voltage_high(&pv1_high);
  inv.set_warning_pv2_voltage_high(&pv2_high);
  inv.set_warning_mppt1_overload(&mppt1_ovld);
  inv.set_warning_mppt2_overload(&mppt2_ovld);
  inv.set_warning_scc1_battery_too_low_to_charge(&scc1_low);
  inv.set_warning_scc2_battery_too_low_to_charge(&scc2_low);
  inv.handle_p005fws_(LV5048_P005FWS_EEPROM_WARN);
  EXPECT_FALSE(line_fail.state);
  EXPECT_FALSE(output_short.state);
  EXPECT_FALSE(over_temp.state);
  EXPECT_FALSE(fan_lock.state);
  EXPECT_FALSE(bat_high.state);
  EXPECT_FALSE(bat_low.state);
  EXPECT_FALSE(bat_shutdown.state);
  EXPECT_FALSE(overload.state);
  EXPECT_FALSE(power_limit.state);
  EXPECT_FALSE(pv1_high.state);
  EXPECT_FALSE(pv2_high.state);
  EXPECT_FALSE(mppt1_ovld.state);
  EXPECT_FALSE(mppt2_ovld.state);
  EXPECT_FALSE(scc1_low.state);
  EXPECT_FALSE(scc2_low.state);
}

TEST(Pi18Lv5048Test, P005fwsMultiFaultNonZeroFaultCode) {
  TestablePipsolar inv;
  sensor::Sensor fault_code;
  inv.set_fault_code(&fault_code);
  inv.handle_p005fws_(LV5048_P005FWS_MULTI_FAULT);
  EXPECT_FLOAT_EQ(fault_code.state, 5.0f);
}

TEST(Pi18Lv5048Test, P005fwsMultiFaultWarningsTrue) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor line_fail, output_short, over_temp, fan_lock, bat_high;
  binary_sensor::BinarySensor bat_low, bat_shutdown, overload;
  inv.set_warning_line_fail(&line_fail);
  inv.set_warning_output_circuit_short(&output_short);
  inv.set_warning_over_temperature(&over_temp);
  inv.set_warning_fan_lock(&fan_lock);
  inv.set_warning_battery_voltage_high(&bat_high);
  inv.set_warning_battery_low_alarm(&bat_low);
  inv.set_warning_battery_under_shutdown(&bat_shutdown);
  inv.set_warning_over_load(&overload);
  inv.handle_p005fws_(LV5048_P005FWS_MULTI_FAULT);
  EXPECT_TRUE(line_fail.state);
  EXPECT_TRUE(output_short.state);
  EXPECT_TRUE(over_temp.state);
  EXPECT_TRUE(fan_lock.state);
  EXPECT_TRUE(bat_high.state);
  EXPECT_FALSE(bat_low.state);
  EXPECT_FALSE(bat_shutdown.state);
  EXPECT_FALSE(overload.state);
}

// ── P005ET ────────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P005etBeckoPopovTotalEnergy) {
  TestablePipsolar inv;
  sensor::Sensor energy;
  inv.set_total_generated_energy(&energy);
  inv.handle_p005et_(LV5048_P005ET_BECKOPOPOV);
  EXPECT_FLOAT_EQ(energy.state, 802946.0f);
}

TEST(Pi18Lv5048Test, P005etBanannaJoeTotalEnergy) {
  TestablePipsolar inv;
  sensor::Sensor energy;
  inv.set_total_generated_energy(&energy);
  inv.handle_p005et_(LV5048_P005ET_BANANNAJOE);
  EXPECT_FLOAT_EQ(energy.state, 320400.0f);
}

// ── P007PGS0 ──────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, P007pgs0TotalApparentAndActivePower) {
  TestablePipsolar inv;
  sensor::Sensor apparent, active;
  inv.set_total_ac_output_apparent_power(&apparent);
  inv.set_total_ac_output_active_power(&active);
  inv.handle_p007pgs0_(LV5048_P007PGS0_ACTIVE);
  EXPECT_FLOAT_EQ(apparent.state, 12338.0f);
  EXPECT_FLOAT_EQ(active.state, 12336.0f);
}

TEST(Pi18Lv5048Test, P007pgs0TotalLoadAndChargingCurrent) {
  TestablePipsolar inv;
  sensor::Sensor load_pct, chg_a;
  inv.set_total_output_load_percent(&load_pct);
  inv.set_total_battery_charging_current(&chg_a);
  inv.handle_p007pgs0_(LV5048_P007PGS0_ACTIVE);
  EXPECT_FLOAT_EQ(load_pct.state, 0.0f);
  EXPECT_FLOAT_EQ(chg_a.state, 337.0f);
}

// ── Robustness ────────────────────────────────────────────────────────────────

TEST(Pi18Lv5048Test, NullSensorsDoNotCrash) {
  TestablePipsolar inv;
  inv.handle_p005gs_(LV5048_P005GS_STANDBY_PV);
  inv.handle_p007piri_(LV5048_P007PIRI_BANANNAJOE);
  inv.handle_p006mod_(LV5048_P006MOD_HYBRID);
  inv.handle_p007flag_(LV5048_P007FLAG);
  inv.handle_p005fws_(LV5048_P005FWS_EEPROM_WARN);
  inv.handle_p005fws_(LV5048_P005FWS_MULTI_FAULT);
  inv.handle_p005et_(LV5048_P005ET_BANANNAJOE);
  inv.handle_p007pgs0_(LV5048_P007PGS0_ACTIVE);
}

TEST(Pi18Lv5048Test, EmptyMessageDoesNotCrash) {
  TestablePipsolar inv;
  sensor::Sensor grid_v;
  inv.set_grid_voltage(&grid_v);
  inv.handle_p005gs_("");
  inv.handle_p007piri_("");
  inv.handle_p006mod_("");
  inv.handle_p007flag_("");
  inv.handle_p005fws_("");
  inv.handle_p005et_("");
  inv.handle_p007pgs0_("");
}

}  // namespace esphome::pi18::testing
