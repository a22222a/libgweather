/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* weather-priv.h - Private header for weather server functions.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <http://www.gnu.org/licenses/>.
 */

#ifndef __WEATHER_PRIV_H_
#define __WEATHER_PRIV_H_

#include "config.h"

#include <time.h>
#include <libintl.h>
#include <math.h>
#ifdef HAVE_LIBSOUP_GNOME
#include <libsoup/soup-gnome.h>
#else
#include <libsoup/soup.h>
#endif

#include "weather.h"
#include "gweather-location.h"

const char *gweather_gettext (const char *str) G_GNUC_FORMAT (1);
#define _(str) (gweather_gettext (str))
#define N_(str) (str)


#define WEATHER_LOCATION_CODE_LEN 4

WeatherLocation *gweather_location_to_weather_location (GWeatherLocation *gloc,
							const char *name);

/*
 * Weather information.
 */

struct _WeatherConditions {
    gboolean significant;
    WeatherConditionPhenomenon phenomenon;
    WeatherConditionQualifier qualifier;
};

typedef struct _WeatherConditions WeatherConditions;

typedef gdouble WeatherTemperature;
typedef gdouble WeatherHumidity;
typedef gdouble WeatherWindSpeed;
typedef gdouble WeatherPressure;
typedef gdouble WeatherVisibility;
typedef time_t WeatherUpdate;

struct _WeatherInfo {
    WeatherForecastType forecast_type;

    TempUnit temperature_unit;
    SpeedUnit speed_unit;
    PressureUnit pressure_unit;
    DistanceUnit distance_unit;

    gboolean valid;
    gboolean network_error;
    gboolean sunValid;
    gboolean tempMinMaxValid;
    WeatherLocation *location;
    WeatherUpdate update;
    WeatherSky sky;
    WeatherConditions cond;
    WeatherTemperature temp;
    WeatherTemperature temp_min;
    WeatherTemperature temp_max;
    WeatherTemperature dew;
    WeatherWindDirection wind;
    WeatherWindSpeed windspeed;
    WeatherPressure pressure;
    WeatherVisibility visibility;
    WeatherUpdate sunrise;
    WeatherUpdate sunset;
    gchar *forecast;
    GSList *forecast_list; /* list of WeatherInfo* for the forecast, NULL if not available */
    gchar *radar_buffer;
    gchar *radar_url;
    GdkPixbufLoader *radar_loader;
    GdkPixbufAnimation *radar;
    SoupSession *session;
    gint requests_pending;

    WeatherInfoFunc finish_cb;
    gpointer cb_data;
};

/*
 * Enum -> string conversions.
 */

const gchar *	weather_wind_direction_string	(WeatherWindDirection wind);
const gchar *	weather_sky_string		(WeatherSky sky);
const gchar *	weather_conditions_string	(WeatherConditions cond);

/* Values common to the parsing source files */

#define DATA_SIZE			5000

#define CONST_DIGITS			"0123456789"
#define CONST_ALPHABET			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"

/* Units conversions and names */

#define TEMP_F_TO_C(f)			(((f) - 32.0) * 0.555556)
#define TEMP_F_TO_K(f)			(TEMP_F_TO_C (f) + 273.15)
#define TEMP_C_TO_F(c)			(((c) * 1.8) + 32.0)

#define WINDSPEED_KNOTS_TO_KPH(knots)	((knots) * 1.851965)
#define WINDSPEED_KNOTS_TO_MPH(knots)	((knots) * 1.150779)
#define WINDSPEED_KNOTS_TO_MS(knots)	((knots) * 0.514444)
#define WINDSPEED_MS_TO_KNOTS(ms)	((ms) / 0.514444)
/* 1 bft ~= (1 m/s / 0.836) ^ (2/3) */
#define WINDSPEED_KNOTS_TO_BFT(knots)	(pow ((knots) * 0.615363, 0.666666))

#define PRESSURE_INCH_TO_KPA(inch)	((inch) * 3.386)
#define PRESSURE_INCH_TO_HPA(inch)	((inch) * 33.86)
#define PRESSURE_INCH_TO_MM(inch)	((inch) * 25.40005)
#define PRESSURE_INCH_TO_MB(inch)	(PRESSURE_INCH_TO_HPA (inch))
#define PRESSURE_INCH_TO_ATM(inch)	((inch) * 0.033421052)
#define PRESSURE_MBAR_TO_INCH(mbar)	((mbar) * 0.029533373)

#define VISIBILITY_SM_TO_KM(sm)		((sm) * 1.609344)
#define VISIBILITY_SM_TO_M(sm)		(VISIBILITY_SM_TO_KM (sm) * 1000)

#define DEGREES_TO_RADIANS(deg)		((fmod (deg,360.) / 180.) * M_PI)
#define RADIANS_TO_DEGREES(rad)		((rad) * 180. / M_PI)
#define RADIANS_TO_HOURS(rad)		((rad) * 12. / M_PI)

void		metar_start_open	(WeatherInfo *info);
void		iwin_start_open		(WeatherInfo *info);
void		metoffice_start_open	(WeatherInfo *info);
void		bom_start_open		(WeatherInfo *info);
void		wx_start_open		(WeatherInfo *info);

gboolean	metar_parse		(gchar *metar,
					 WeatherInfo *info);

gboolean	requests_init		(WeatherInfo *info);
void		request_done		(WeatherInfo *info,
					 gboolean     ok);

gboolean	calc_sun		(WeatherInfo *info);

void		free_forecast_list	(WeatherInfo *info);

#endif /* __WEATHER_PRIV_H_ */

