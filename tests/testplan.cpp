#include "dive.h"
#include "testplan.h"
#include "planner.h"
#include "units.h"
#include "qthelper.h"
#include <QDebug>

#define DEBUG  1

// testing the dive plan algorithm
extern bool plan(struct diveplan *diveplan, char **cached_datap, bool is_planner, bool show_disclaimer);

void setupPrefs()
{
	prefs = default_prefs;
	prefs.ascrate50 = feet_to_mm(30) / 60;
	prefs.ascrate75 = prefs.ascrate50;
	prefs.ascratestops = prefs.ascrate50;
	prefs.ascratelast6m = feet_to_mm(10) / 60;
	prefs.last_stop = true;
}

void setupPrefsVpmb()
{
	prefs = default_prefs;
	prefs.ascrate50 = 10000 / 60;
	prefs.ascrate75 = prefs.ascrate50;
	prefs.ascratestops = prefs.ascrate50;
	prefs.ascratelast6m = prefs.ascrate50;
	prefs.descrate = 99000 / 60;
	prefs.last_stop = false;
	prefs.deco_mode = VPMB;
}

void setupPlan(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->gfhigh = 100;
	dp->gflow = 100;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {150}, {450} };
	struct gasmix ean36 = { {360}, {0} };
	struct gasmix oxygen = { {1000}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.cylinder[1].gasmix = ean36;
	displayed_dive.cylinder[2].gasmix = oxygen;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(79, 260) * 60 / M_OR_FT(23, 75);
	plan_add_segment(dp, droptime, M_OR_FT(79, 260), bottomgas, 0, 1);
	plan_add_segment(dp, 30*60 - droptime, M_OR_FT(79, 260), bottomgas, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&ean36, po2, &displayed_dive, M_OR_FT(3,10)).mm, ean36, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&oxygen, po2, &displayed_dive, M_OR_FT(3,10)).mm, oxygen, 0, 1);
}

void setupPlanVpmb60m30minAir(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {210}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.surface_pressure.mbar = 1013;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(60, 200) * 60 / M_OR_FT(99, 330);
	plan_add_segment(dp, droptime, M_OR_FT(60, 200), bottomgas, 0, 1);
	plan_add_segment(dp, 30*60 - droptime, M_OR_FT(60, 200), bottomgas, 0, 1);
}

void setupPlanVpmb60m30minEan50(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {210}, {0} };
	struct gasmix ean50 = { {500}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.cylinder[1].gasmix = ean50;
	displayed_dive.surface_pressure.mbar = 1013;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(60, 200) * 60 / M_OR_FT(99, 330);
	plan_add_segment(dp, droptime, M_OR_FT(60, 200), bottomgas, 0, 1);
	plan_add_segment(dp, 30*60 - droptime, M_OR_FT(60, 200), bottomgas, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&ean50, po2, &displayed_dive, M_OR_FT(3,10)).mm, ean50, 0, 1);
}

void setupPlanVpmb60m30minTx(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {180}, {450} };
	struct gasmix ean50 = { {500}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.cylinder[1].gasmix = ean50;
	displayed_dive.surface_pressure.mbar = 1013;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(60, 200) * 60 / M_OR_FT(99, 330);
	plan_add_segment(dp, droptime, M_OR_FT(60, 200), bottomgas, 0, 1);
	plan_add_segment(dp, 30*60 - droptime, M_OR_FT(60, 200), bottomgas, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&ean50, po2, &displayed_dive, M_OR_FT(3,10)).mm, ean50, 0, 1);
}

void setupPlanVpmbMultiLevelAir(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {210}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.surface_pressure.mbar = 1013;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(20, 66) * 60 / M_OR_FT(99, 330);
	plan_add_segment(dp, droptime, M_OR_FT(20, 66), bottomgas, 0, 1);
	plan_add_segment(dp, 10*60 - droptime, M_OR_FT(20, 66), bottomgas, 0, 1);
	plan_add_segment(dp, 1*60, M_OR_FT(60, 200), bottomgas, 0, 1);
	plan_add_segment(dp, 29*60, M_OR_FT(60, 200), bottomgas, 0, 1);
}

void setupPlanVpmb100m60min(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {180}, {450} };
	struct gasmix ean50 = { {500}, {0} };
	struct gasmix oxygen = { {1000}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.cylinder[1].gasmix = ean50;
	displayed_dive.cylinder[2].gasmix = oxygen;
	displayed_dive.surface_pressure.mbar = 1013;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(100, 330) * 60 / M_OR_FT(99, 330);
	plan_add_segment(dp, droptime, M_OR_FT(100, 330), bottomgas, 0, 1);
	plan_add_segment(dp, 60*60 - droptime, M_OR_FT(100, 330), bottomgas, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&ean50, po2, &displayed_dive, M_OR_FT(3,10)).mm, ean50, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&oxygen, po2, &displayed_dive, M_OR_FT(3,10)).mm, oxygen, 0, 1);
}

void setupPlanVpmb100m10min(struct diveplan *dp)
{
	dp->salinity = 10300;
	dp->surface_pressure = 1013;
	dp->bottomsac = 0;
	dp->decosac = 0;

	struct gasmix bottomgas = { {180}, {450} };
	struct gasmix ean50 = { {500}, {0} };
	struct gasmix oxygen = { {1000}, {0} };
	pressure_t po2 = { 1600 };
	displayed_dive.cylinder[0].gasmix = bottomgas;
	displayed_dive.cylinder[1].gasmix = ean50;
	displayed_dive.cylinder[2].gasmix = oxygen;
	displayed_dive.surface_pressure.mbar = 1013;
	reset_cylinders(&displayed_dive, true);
	free_dps(dp);

	int droptime = M_OR_FT(100, 330) * 60 / M_OR_FT(99, 330);
	plan_add_segment(dp, droptime, M_OR_FT(100, 330), bottomgas, 0, 1);
	plan_add_segment(dp, 10*60 - droptime, M_OR_FT(100, 330), bottomgas, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&ean50, po2, &displayed_dive, M_OR_FT(3,10)).mm, ean50, 0, 1);
	plan_add_segment(dp, 0, gas_mod(&oxygen, po2, &displayed_dive, M_OR_FT(3,10)).mm, oxygen, 0, 1);
}

bool compareDecoTime(int actualRunTimeSeconds, int expectedRunTimeSeconds)
{
	// If the calculated run time equals the expected run time, do a simple comparison
	if (actualRunTimeSeconds == expectedRunTimeSeconds) {
		return true;
	} else {
		/* We want the difference between the expected and calculated total run time to be not more than
		* 1% of total run time + 1 minute */
		int permilDifferenceAllowed = 1 * 10;
		int absoluteDifferenceAllowedSeconds = 60;
		int totalDifferenceAllowed = 0.001 * permilDifferenceAllowed * expectedRunTimeSeconds + absoluteDifferenceAllowedSeconds;
		int totalDifference = abs(actualRunTimeSeconds - expectedRunTimeSeconds);

		printf("Calculated run time = %d seconds\n", actualRunTimeSeconds);
		printf("Expected run time = %d seconds\n", expectedRunTimeSeconds);
		printf("Allowed time difference is %g percent plus %d seconds = %d seconds\n",
		       permilDifferenceAllowed * 0.1, absoluteDifferenceAllowedSeconds, totalDifferenceAllowed);
		printf("total difference = %d seconds\n", totalDifference);

		return (totalDifference <= totalDifferenceAllowed);
	}
}

void TestPlan::testMetric()
{
	char *cache = NULL;

	setupPrefs();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;
	prefs.deco_mode = BUEHLMANN;

	struct diveplan testPlan = { 0 };
	setupPlan(&testPlan);

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check first gas change to EAN36 at 33m
	struct event *ev = displayed_dive.dc.events;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 1);
	QCOMPARE(ev->value, 36);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 33000);
	// check second gas change to Oxygen at 6m
	ev = ev->next;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 2);
	QCOMPARE(ev->value, 100);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 6000);
	// check expected run time of 105 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 108u * 60u));
}

void TestPlan::testImperial()
{
	char *cache = NULL;

	setupPrefs();
	prefs.unit_system = IMPERIAL;
	prefs.units.length = units::FEET;
	prefs.deco_mode = BUEHLMANN;

	struct diveplan testPlan = { 0 };
	setupPlan(&testPlan);

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check first gas change to EAN36 at 33m
	struct event *ev = displayed_dive.dc.events;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 1);
	QCOMPARE(ev->value, 36);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 33528);
	// check second gas change to Oxygen at 6m
	ev = ev->next;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 2);
	QCOMPARE(ev->value, 100);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 6096);
	// check expected run time of 105 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 110u * 60u - 2u));
}

void TestPlan::testVpmbMetric60m30minAir()
{
	char *cache = NULL;

	setupPrefsVpmb();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;

	struct diveplan testPlan = { 0 };
	setupPlanVpmb60m30minAir(&testPlan);
	setCurrentAppState("PlanDive");

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check expected run time of 141 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 141u * 60u + 20u));
}

void TestPlan::testVpmbMetric60m30minEan50()
{
	char *cache = NULL;

	setupPrefsVpmb();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;

	struct diveplan testPlan = { 0 };
	setupPlanVpmb60m30minEan50(&testPlan);
	setCurrentAppState("PlanDive");

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check first gas change to EAN50 at 21m
	struct event *ev = displayed_dive.dc.events;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 1);
	QCOMPARE(ev->value, 50);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 21000);
	// check expected run time of 95 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 95u * 60u + 20u));
}

void TestPlan::testVpmbMetric60m30minTx()
{
	char *cache = NULL;

	setupPrefsVpmb();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;

	struct diveplan testPlan = { 0 };
	setupPlanVpmb60m30minTx(&testPlan);
	setCurrentAppState("PlanDive");

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check first gas change to EAN50 at 21m
	struct event *ev = displayed_dive.dc.events;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 1);
	QCOMPARE(ev->value, 50);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 21000);
	// check expected run time of 89 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 89u * 60u + 20u));
}

void TestPlan::testVpmbMetric100m60min()
{
	char *cache = NULL;

	setupPrefsVpmb();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;

	struct diveplan testPlan = { 0 };
	setupPlanVpmb100m60min(&testPlan);
	setCurrentAppState("PlanDive");

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check first gas change to EAN50 at 21m
	struct event *ev = displayed_dive.dc.events;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 1);
	QCOMPARE(ev->value, 50);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 21000);
	// check second gas change to Oxygen at 6m
	ev = ev->next;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 2);
	QCOMPARE(ev->value, 100);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 6000);
	// check expected run time of 316 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 316u * 60u + 20u));
}

void TestPlan::testVpmbMetricMultiLevelAir()
{
	char *cache = NULL;

	setupPrefsVpmb();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;

	struct diveplan testPlan = { 0 };
	setupPlanVpmbMultiLevelAir(&testPlan);
	setCurrentAppState("PlanDive");

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check expected run time of 167 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 167u * 60u + 20u));
}

void TestPlan::testVpmbMetric100m10min()
{
	char *cache = NULL;

	setupPrefsVpmb();
	prefs.unit_system = METRIC;
	prefs.units.length = units::METERS;

	struct diveplan testPlan = { 0 };
	setupPlanVpmb100m10min(&testPlan);
	setCurrentAppState("PlanDive");

	plan(&testPlan, &cache, 1, 0);

#if DEBUG
	free(displayed_dive.notes);
	displayed_dive.notes = NULL;
	save_dive(stdout, &displayed_dive);
#endif

	// check first gas change to EAN50 at 21m
	struct event *ev = displayed_dive.dc.events;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 1);
	QCOMPARE(ev->value, 50);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 21000);
	// check second gas change to Oxygen at 6m
	ev = ev->next;
	QVERIFY(ev != NULL);
	QCOMPARE(ev->gas.index, 2);
	QCOMPARE(ev->value, 100);
	QCOMPARE(get_depth_at_time(&displayed_dive.dc, ev->time.seconds), 6000);
	// check expected run time of 58 minutes
	QVERIFY(compareDecoTime(displayed_dive.dc.duration.seconds, 58u * 60u + 20u));
}

QTEST_MAIN(TestPlan)
