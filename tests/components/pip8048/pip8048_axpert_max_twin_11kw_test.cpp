#include <gtest/gtest.h>
#include "common.h"
#include "frames_axpert_max_twin_11kw.h"

namespace esphome::pip8048::testing {

// ── QPIRI ────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert11kwTest, QpiriGridRatings) {
  TestablePipsolar inv;
  sensor::Sensor grid_v, grid_a;
  inv.set_grid_rating_voltage(&grid_v);
  inv.set_grid_rating_current(&grid_a);
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  EXPECT_FLOAT_EQ(grid_v.state, 230.0f);
  EXPECT_NEAR(grid_a.state, 47.8f, 0.01f);
}

TEST(Pip8048Axpert11kwTest, QpiriOutputRatings11kw) {
  TestablePipsolar inv;
  sensor::Sensor apparent, active;
  inv.set_ac_output_rating_apparent_power(&apparent);
  inv.set_ac_output_rating_active_power(&active);
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  EXPECT_FLOAT_EQ(apparent.state, 11000.0f);
  EXPECT_FLOAT_EQ(active.state, 11000.0f);
}

TEST(Pip8048Axpert11kwTest, QpiriBatteryVoltages) {
  TestablePipsolar inv;
  sensor::Sensor bat_v, recharge_v, bulk_v, float_v;
  inv.set_battery_rating_voltage(&bat_v);
  inv.set_battery_recharge_voltage(&recharge_v);
  inv.set_battery_bulk_voltage(&bulk_v);
  inv.set_battery_float_voltage(&float_v);
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  EXPECT_FLOAT_EQ(bat_v.state, 48.0f);
  EXPECT_FLOAT_EQ(recharge_v.state, 50.0f);
  EXPECT_FLOAT_EQ(bulk_v.state, 56.0f);
  EXPECT_NEAR(float_v.state, 55.2f, 0.01f);
}

TEST(Pip8048Axpert11kwTest, QpiriChargingCurrentHigherRating) {
  TestablePipsolar inv;
  sensor::Sensor max_ac_chg, max_chg;
  inv.set_current_max_ac_charging_current(&max_ac_chg);
  inv.set_current_max_charging_current(&max_chg);
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  EXPECT_FLOAT_EQ(max_ac_chg.state, 10.0f);
  // 11kW twin supports 100A charging (vs 40A on the 8kW)
  EXPECT_FLOAT_EQ(max_chg.state, 100.0f);
}

TEST(Pip8048Axpert11kwTest, QpiriInputVoltageRangeWide) {
  TestablePipsolar inv;
  sensor::Sensor in_range;
  inv.set_input_voltage_range(&in_range);
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  // input_voltage_range=1 (wide input range, vs 0=appliance on the 8kW)
  EXPECT_FLOAT_EQ(in_range.state, 1.0f);
}

TEST(Pip8048Axpert11kwTest, QpiriPvOkConditionDisabled) {
  TestablePipsolar inv;
  sensor::Sensor pv_ok, pv_bal;
  inv.set_pv_ok_condition_for_parallel(&pv_ok);
  inv.set_pv_power_balance(&pv_bal);
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  EXPECT_FLOAT_EQ(pv_ok.state, 0.0f);
  EXPECT_FLOAT_EQ(pv_bal.state, 1.0f);
}

// ── QPIGS2 ───────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert11kwTest, Qpigs2HighPower) {
  TestablePipsolar inv;
  sensor::Sensor pv2_a, pv2_v, pv2_pwr;
  inv.set_pv2_input_current(&pv2_a);
  inv.set_pv2_input_voltage(&pv2_v);
  inv.set_pv2_charging_power(&pv2_pwr);
  inv.handle_qpigs2_(AXPERT_11KW_QPIGS2_A);
  EXPECT_NEAR(pv2_a.state, 3.0f, 0.01f);
  EXPECT_NEAR(pv2_v.state, 273.2f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 825.0f);
}

TEST(Pip8048Axpert11kwTest, Qpigs2MultipleCyclesPowerDecreasing) {
  TestablePipsolar inv;
  sensor::Sensor pv2_a, pv2_pwr;
  inv.set_pv2_input_current(&pv2_a);
  inv.set_pv2_charging_power(&pv2_pwr);

  inv.handle_qpigs2_(AXPERT_11KW_QPIGS2_A);
  EXPECT_NEAR(pv2_a.state, 3.0f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 825.0f);

  inv.handle_qpigs2_(AXPERT_11KW_QPIGS2_B);
  EXPECT_NEAR(pv2_a.state, 2.8f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 796.0f);

  inv.handle_qpigs2_(AXPERT_11KW_QPIGS2_C);
  EXPECT_NEAR(pv2_a.state, 2.7f, 0.01f);
  EXPECT_FLOAT_EQ(pv2_pwr.state, 736.0f);
}

// ── QMOD ─────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert11kwTest, QmodBatteryMode) {
  TestablePipsolar inv;
  text_sensor::TextSensor mode;
  inv.set_device_mode(&mode);
  inv.handle_qmod_(AXPERT_11KW_QMOD);
  EXPECT_STREQ(mode.state.c_str(), "B");
}

// ── QFLAG ────────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert11kwTest, QflagBacklightAndAlarmEnabled) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor backlight, alarm, over_temp, fault_rec;
  inv.set_backlight_on(&backlight);
  inv.set_alarm_on_when_primary_source_interrupt(&alarm);
  inv.set_over_temperature_restart_function(&over_temp);
  inv.set_fault_code_record(&fault_rec);
  inv.handle_qflag_(AXPERT_11KW_QFLAG);
  // 11kW has backlight+alarm enabled (vs disabled on 8kW)
  EXPECT_TRUE(backlight.state);
  EXPECT_TRUE(alarm.state);
  EXPECT_TRUE(over_temp.state);
  EXPECT_TRUE(fault_rec.state);
}

TEST(Pip8048Axpert11kwTest, QflagBypassDisabled) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor bypass, buzzer;
  inv.set_overload_bypass_function(&bypass);
  inv.set_silence_buzzer_open_buzzer(&buzzer);
  inv.handle_qflag_(AXPERT_11KW_QFLAG);
  // 11kW has bypass disabled (vs enabled on 8kW)
  EXPECT_FALSE(bypass.state);
  EXPECT_FALSE(buzzer.state);
}

// ── QBATCD ───────────────────────────────────────────────────────────────────

TEST(Pip8048Axpert11kwTest, QbatcdNaoResponseAllFalse) {
  TestablePipsolar inv;
  binary_sensor::BinarySensor discharge, standby, charge;
  inv.set_discharge_onoff(&discharge);
  inv.set_discharge_with_standby_onoff(&standby);
  inv.set_charge_onoff(&charge);
  inv.handle_qbatcd_(AXPERT_11KW_QBATCD_NAO);
  // '(NAO': N/A/O all != '1', so all three bits are false
  EXPECT_FALSE(discharge.state);
  EXPECT_FALSE(standby.state);
  EXPECT_FALSE(charge.state);
}

// ── Robustness ───────────────────────────────────────────────────────────────

TEST(Pip8048Axpert11kwTest, NullSensorsDoNotCrash) {
  TestablePipsolar inv;
  inv.handle_qpiri_(AXPERT_11KW_QPIRI);
  inv.handle_qpigs2_(AXPERT_11KW_QPIGS2_A);
  inv.handle_qmod_(AXPERT_11KW_QMOD);
  inv.handle_qflag_(AXPERT_11KW_QFLAG);
  inv.handle_qbatcd_(AXPERT_11KW_QBATCD_NAO);
}

}  // namespace esphome::pip8048::testing
