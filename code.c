/*
 * ============================================================
 *   WEATHER MONITORING & FORECAST SYSTEM v2.0
 *   Advanced Features: Forecast + Analytics + File Handling
 *   Extreme Weather Alerts | Multi-City Comparison | Reports
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_CITIES    5
#define MAX_DAYS      7
#define MAX_NAME_LEN  20
#define DATA_FILE     "weather_data.txt"
#define REPORT_FILE   "weather_report.txt"
#define LOG_FILE      "weather_log.txt"
#define ALERT_FILE    "weather_alerts.txt"

/* Temperature thresholds for alerts */
#define EXTREME_HEAT_THRESHOLD  40.0f
#define HIGH_HEAT_THRESHOLD     35.0f
#define EXTREME_COLD_THRESHOLD   0.0f
#define HIGH_HUMIDITY_THRESHOLD  85.0f
#define HIGH_WIND_THRESHOLD      25.0f

/* ---------- Data Structures ---------- */

typedef struct {
    char  day[15];
    float temp_high;
    float temp_low;
    float humidity;
    float wind_speed;
    char  condition[20];
} DayForecast;

typedef struct {
    char        name[MAX_NAME_LEN];
    DayForecast forecast[MAX_DAYS];
    int         num_days;
} City;

/* Weekly statistics structure */
typedef struct {
    float avg_high;
    float avg_low;
    float avg_humidity;
    float avg_wind;
    float max_temp;
    float min_temp;
    float max_wind;
    int hottest_day;
    int coldest_day;
} WeeklyStats;

/* ============================================================
 *  FORWARD DECLARATIONS
 * ============================================================ */
void print_separator(char ch, int count);
void fprint_separator(FILE *fp, char ch, int count);
void get_timestamp(char *buf, int size);
void log_activity(const char *message);
int validate_temperature(float temp);
int validate_humidity(float humidity);
int validate_wind_speed(float wind);

/* ============================================================
 *  UTILITY FUNCTIONS
 * ============================================================ */

void print_separator(char ch, int count) {
    int i;
    for (i = 0; i < count; i++) putchar(ch);
    putchar('\n');
}

void fprint_separator(FILE *fp, char ch, int count) {
    int i;
    for (i = 0; i < count; i++) fputc(ch, fp);
    fputc('\n', fp);
}

void print_banner(void) {
    print_separator('=', 70);
    printf("  +-------- WEATHER MONITORING & FORECAST SYSTEM v2.0 --------+\n");
    printf("  |          Advanced Analytics | Multi-City Support          |\n");
    printf("  |             First Year C Programming Project              |\n");
    print_separator('=', 70);
    printf("\n");
}

float to_fahrenheit(float c) { return (c * 9.0f / 5.0f) + 32.0f; }

const char* get_icon(const char *condition) {
    if (strstr(condition, "Sunny"))    return "[SUN] ";
    if (strstr(condition, "Rain"))     return "[RAIN]";
    if (strstr(condition, "Cloud"))    return "[CLOU]";
    if (strstr(condition, "Storm"))    return "[STRM]";
    if (strstr(condition, "Snow"))     return "[SNOW]";
    if (strstr(condition, "Pleasant")) return "[NICE]";
    return "[????]";
}

const char* heat_category(float temp) {
    if (temp >= 40) return "DANGER - Extreme Heat";
    if (temp >= 35) return "WARNING - Very Hot";
    if (temp >= 28) return "WARM";
    if (temp >= 20) return "PLEASANT";
    if (temp >= 10) return "COOL";
    return "COLD";
}

/* Humidity Risk Assessment */
const char* humidity_category(float humidity) {
    if (humidity >= 85) return "DANGER - Very High";
    if (humidity >= 70) return "HIGH";
    if (humidity >= 40) return "MODERATE";
    return "LOW";
}

/* Wind Speed Assessment */
const char* wind_category(float wind) {
    if (wind >= 25) return "DANGER - Severe";
    if (wind >= 20) return "STRONG";
    if (wind >= 10) return "MODERATE";
    return "LIGHT";
}

/* Get current timestamp as string */
void get_timestamp(char *buf, int size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

/* Calculate weekly statistics */
WeeklyStats calculate_weekly_stats(const City *city) {
    WeeklyStats stats;
    int i;
    
    stats.avg_high = stats.avg_low = stats.avg_humidity = stats.avg_wind = 0;
    stats.max_temp = city->forecast[0].temp_high;
    stats.min_temp = city->forecast[0].temp_low;
    stats.max_wind = city->forecast[0].wind_speed;
    stats.hottest_day = stats.coldest_day = 0;
    
    for (i = 0; i < city->num_days; i++) {
        const DayForecast *d = &city->forecast[i];
        stats.avg_high += d->temp_high;
        stats.avg_low += d->temp_low;
        stats.avg_humidity += d->humidity;
        stats.avg_wind += d->wind_speed;
        
        if (d->temp_high > stats.max_temp) {
            stats.max_temp = d->temp_high;
            stats.hottest_day = i;
        }
        if (d->temp_low < stats.min_temp) {
            stats.min_temp = d->temp_low;
            stats.coldest_day = i;
        }
        if (d->wind_speed > stats.max_wind) {
            stats.max_wind = d->wind_speed;
        }
    }
    
    stats.avg_high /= city->num_days;
    stats.avg_low /= city->num_days;
    stats.avg_humidity /= city->num_days;
    stats.avg_wind /= city->num_days;
    
    return stats;
}

/* Generate Extreme Weather Alerts */
void check_extreme_weather_alerts(City cities[], int num_cities) {
    FILE *fp;
    int i, d, has_alerts = 0;
    char ts[30];
    
    fp = fopen(ALERT_FILE, "a");
    if (fp == NULL) return;
    
    get_timestamp(ts, sizeof(ts));
    
    for (i = 0; i < num_cities; i++) {
        for (d = 0; d < cities[i].num_days; d++) {
            DayForecast *f = &cities[i].forecast[d];
            
            if (f->temp_high >= EXTREME_HEAT_THRESHOLD) {
                fprintf(fp, "[%s] ALERT: %s - %s has EXTREME HEAT (%.1f C)\n",
                        ts, cities[i].name, f->day, f->temp_high);
                has_alerts = 1;
            }
            if (f->humidity >= HIGH_HUMIDITY_THRESHOLD) {
                fprintf(fp, "[%s] ALERT: %s - %s has HIGH HUMIDITY (%.1f%%)\n",
                        ts, cities[i].name, f->day, f->humidity);
                has_alerts = 1;
            }
            if (f->wind_speed >= HIGH_WIND_THRESHOLD) {
                fprintf(fp, "[%s] ALERT: %s - %s has SEVERE WIND (%.1f km/h)\n",
                        ts, cities[i].name, f->day, f->wind_speed);
                has_alerts = 1;
            }
        }
    }
    
    fclose(fp);
    if (has_alerts) log_activity("Extreme weather alerts generated");
}

/* ============================================================
 *  INPUT VALIDATION & ERROR HANDLING
 * ============================================================ */

int validate_temperature(float temp) {
    return (temp >= -50 && temp <= 60);  /* Reasonable range */
}

int validate_humidity(float humidity) {
    return (humidity >= 0 && humidity <= 100);
}

int validate_wind_speed(float wind) {
    return (wind >= 0 && wind <= 150);
}

/* ============================================================
 *  FILE HANDLING - Section 1: WRITE (Save data to file)
 * ============================================================ */

int save_data_to_file(City cities[], int num_cities) {
    FILE *fp;
    int   i, d;

    fp = fopen(DATA_FILE, "w");
    if (fp == NULL) {
        printf("  ERROR: Could not open %s for writing!\n", DATA_FILE);
        return 0;   /* failure */
    }

    /* Write header */
    char ts[30];
    get_timestamp(ts, sizeof(ts));
    fprintf(fp, "# Weather Monitoring System - Data File\n");
    fprintf(fp, "# Saved on: %s\n", ts);
    fprintf(fp, "# Format: CITY name num_days\n");
    fprintf(fp, "#         DAY day high low humidity wind condition\n");
    fprintf(fp, "#-------------------------------------------------\n\n");

    /* Write number of cities */
    fprintf(fp, "NUM_CITIES %d\n\n", num_cities);

    for (i = 0; i < num_cities; i++) {
        fprintf(fp, "CITY %s %d\n", cities[i].name, cities[i].num_days);
        for (d = 0; d < cities[i].num_days; d++) {
            DayForecast *f = &cities[i].forecast[d];
            fprintf(fp, "DAY %s %.1f %.1f %.1f %.1f %s\n",
                    f->day, f->temp_high, f->temp_low,
                    f->humidity, f->wind_speed, f->condition);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("  [OK] Data saved successfully to '%s'.\n", DATA_FILE);
    return 1;   /* success */
}

/* ============================================================
 *  FILE HANDLING - Section 2: READ (Load data from file)
 * ============================================================ */

int load_data_from_file(City cities[], int *num_cities) {
    FILE *fp;
    char  line[100];
    int   i = -1, d = 0;

    fp = fopen(DATA_FILE, "r");
    if (fp == NULL) {
        printf("  INFO: '%s' not found. Using default data.\n", DATA_FILE);
        return 0;
    }

    *num_cities = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        /* Skip comment lines and blank lines */
        if (line[0] == '#' || line[0] == '\n') continue;

        if (strncmp(line, "NUM_CITIES", 10) == 0) {
            sscanf(line, "NUM_CITIES %d", num_cities);
        }
        else if (strncmp(line, "CITY", 4) == 0) {
            i++;
            d = 0;
            sscanf(line, "CITY %s %d",
                   cities[i].name, &cities[i].num_days);
        }
        else if (strncmp(line, "DAY", 3) == 0 && i >= 0) {
            DayForecast *f = &cities[i].forecast[d];
            sscanf(line, "DAY %s %f %f %f %f %s",
                   f->day, &f->temp_high, &f->temp_low,
                   &f->humidity, &f->wind_speed, f->condition);
            d++;
        }
    }

    fclose(fp);
    printf("  [OK] Data loaded from '%s'.\n", DATA_FILE);
    return 1;
}

/* ============================================================
 *  FILE HANDLING - Section 3: APPEND (Activity Log)
 * ============================================================ */

void log_activity(const char *message) {
    FILE *fp;
    char  ts[30];

    fp = fopen(LOG_FILE, "a");   /* "a" = append, never overwrites */
    if (fp == NULL) return;

    get_timestamp(ts, sizeof(ts));
    fprintf(fp, "[%s] %s\n", ts, message);
    fclose(fp);
}

/* ============================================================
 *  FILE HANDLING - Section 4: REPORT (Formatted text report)
 * ============================================================ */

void export_report(City cities[], int num_cities) {
    FILE *fp;
    int   i, d;
    char  ts[30];

    fp = fopen(REPORT_FILE, "w");
    if (fp == NULL) {
        printf("  ERROR: Could not create report file!\n");
        return;
    }

    get_timestamp(ts, sizeof(ts));
    fprint_separator(fp, '=', 70);
    fprintf(fp, "        WEATHER MONITORING SYSTEM - COMPREHENSIVE REPORT\n");
    fprintf(fp, "        Generated: %s\n", ts);
    fprint_separator(fp, '=', 70);
    fprintf(fp, "\n");

    for (i = 0; i < num_cities; i++) {
        City *c = &cities[i];
        WeeklyStats stats = calculate_weekly_stats(c);

        /* Section header */
        fprint_separator(fp, '-', 70);
        fprintf(fp, "  CITY: %s\n", c->name);
        fprint_separator(fp, '-', 70);
        fprintf(fp, "  %-10s %-8s %-8s %-10s %-8s %-12s\n",
                "Day", "High C", "Low C", "Humidity", "Wind", "Condition");
        fprint_separator(fp, '-', 70);

        for (d = 0; d < c->num_days; d++) {
            DayForecast *f = &c->forecast[d];
            fprintf(fp, "  %-10s %-8.1f %-8.1f %-10.1f%% %-8.1f  %-12s\n",
                    f->day, f->temp_high, f->temp_low,
                    f->humidity, f->wind_speed, f->condition);
        }

        fprintf(fp, "\n  [WEEKLY ANALYTICS]\n");
        fprintf(fp, "  Avg High  : %.1f C (%.1f F)\n",
                stats.avg_high, to_fahrenheit(stats.avg_high));
        fprintf(fp, "  Avg Low   : %.1f C (%.1f F)\n",
                stats.avg_low, to_fahrenheit(stats.avg_low));
        fprintf(fp, "  Max High  : %.1f C on %s\n",
                stats.max_temp, c->forecast[stats.hottest_day].day);
        fprintf(fp, "  Min Low   : %.1f C on %s\n",
                stats.min_temp, c->forecast[stats.coldest_day].day);
        fprintf(fp, "  Avg Humidity : %.1f%% (%s)\n",
                stats.avg_humidity, humidity_category(stats.avg_humidity));
        fprintf(fp, "  Avg Wind     : %.1f km/h (%s)\n",
                stats.avg_wind, wind_category(stats.avg_wind));
        fprintf(fp, "  Max Wind     : %.1f km/h\n", stats.max_wind);
        fprintf(fp, "  Classification: %s\n\n",
                heat_category(stats.avg_high));
    }

    fprint_separator(fp, '=', 70);
    fprintf(fp, "  Report generated by Weather Monitoring System v2.0\n");
    fprintf(fp, "  Total Cities Analyzed: %d | Total Days: %d\n", num_cities, num_cities * MAX_DAYS);
    fprint_separator(fp, '=', 70);

    fclose(fp);
    printf("  [OK] Report exported to '%s'.\n", REPORT_FILE);
    log_activity("Comprehensive report exported");
    
    /* Generate alerts */
    check_extreme_weather_alerts(cities, num_cities);
}

/* ============================================================
 *  FILE HANDLING - Section 5: READ LOG (display log)
 * ============================================================ */

void view_log(void) {
    FILE *fp;
    char  line[200];

    fp = fopen(LOG_FILE, "r");
    if (fp == NULL) {
        printf("  No activity log found yet.\n");
        return;
    }

    printf("\n");
    print_separator('=', 65);
    printf("  ACTIVITY LOG (%s)\n", LOG_FILE);
    print_separator('=', 65);

    while (fgets(line, sizeof(line), fp) != NULL)
        printf("  %s", line);

    fclose(fp);
    print_separator('=', 65);
}

/* ============================================================
 *  DISPLAY FUNCTIONS
 * ============================================================ */

void display_daily_forecast(const City *city) {
    int i;
    printf("\n");
    print_separator('=', 75);
    printf("  7-DAY FORECAST FOR: %s\n", city->name);
    print_separator('=', 75);
    printf("  %-12s %-8s %-8s %-10s %-8s %-12s %-8s\n",
           "Day", "High C", "Low C", "Humidity", "Wind", "Condition", "Status");
    print_separator('-', 75);
    for (i = 0; i < city->num_days; i++) {
        const DayForecast *d = &city->forecast[i];
        const char *status;
        if (d->temp_high >= EXTREME_HEAT_THRESHOLD) status = "[DANGER]";
        else if (d->temp_high >= HIGH_HEAT_THRESHOLD) status = "[WARN]";
        else if (d->humidity >= HIGH_HUMIDITY_THRESHOLD) status = "[HIGH]";
        else if (d->wind_speed >= HIGH_WIND_THRESHOLD) status = "[WINDY]";
        else status = "[OK]";
        
        printf("  %-12s %s%-5.1f    %-6.1f    %-8.1f%%  %-6.1f  %-12s %s\n",
               d->day, get_icon(d->condition),
               d->temp_high, d->temp_low,
               d->humidity, d->wind_speed, d->condition, status);
    }
    print_separator('=', 75);
}

void display_temperature_graph(const City *city) {
    int i, j;
    printf("\n");
    print_separator('=', 65);
    printf("  TEMPERATURE VISUALIZATION - %s\n", city->name);
    print_separator('=', 65);
    
    /* Find max and min for scaling */
    float max_temp = city->forecast[0].temp_high;
    float min_temp = city->forecast[0].temp_low;
    int k;
    for (k = 0; k < city->num_days; k++) {
        if (city->forecast[k].temp_high > max_temp) max_temp = city->forecast[k].temp_high;
        if (city->forecast[k].temp_low < min_temp) min_temp = city->forecast[k].temp_low;
    }
    
    for (i = 0; i < city->num_days; i++) {
        const DayForecast *d = &city->forecast[i];
        int bh = (int)((d->temp_high - min_temp) / (max_temp - min_temp) * 40);
        int bl = (int)((d->temp_low - min_temp) / (max_temp - min_temp) * 40);
        
        printf("  %-4s High |", d->day);
        for (j = 0; j < bh; j++) printf("#");
        printf(" %.1f C\n", d->temp_high);
        
        printf("       Low  |");
        for (j = 0; j < bl; j++) printf("-");
        printf(" %.1f C\n", d->temp_low);
        printf("            |\n");
    }
    print_separator('=', 65);
}

void display_weekly_summary(const City *city) {
    WeeklyStats stats = calculate_weekly_stats(city);
    
    printf("\n");
    print_separator('=', 65);
    printf("  WEEKLY SUMMARY FOR: %s\n", city->name);
    print_separator('=', 65);
    printf("\n  TEMPERATURE ANALYSIS\n");
    printf("  ├─ Average High   : %.1f C (%.1f F)\n", stats.avg_high, to_fahrenheit(stats.avg_high));
    printf("  ├─ Average Low    : %.1f C (%.1f F)\n", stats.avg_low, to_fahrenheit(stats.avg_low));
    printf("  ├─ Hottest Day    : %s (%.1f C)\n", city->forecast[stats.hottest_day].day, stats.max_temp);
    printf("  ├─ Coldest Day    : %s (%.1f C)\n", city->forecast[stats.coldest_day].day, stats.min_temp);
    printf("  └─ Temp Range     : %.1f C\n\n", stats.max_temp - stats.min_temp);
    
    printf("  WIND & HUMIDITY ANALYSIS\n");
    printf("  ├─ Average Humidity : %.1f%% (%s)\n", stats.avg_humidity, humidity_category(stats.avg_humidity));
    printf("  ├─ Average Wind     : %.1f km/h (%s)\n", stats.avg_wind, wind_category(stats.avg_wind));
    printf("  ├─ Max Wind Speed   : %.1f km/h\n", stats.max_wind);
    printf("  └─ Comfort Level    : %s\n\n", heat_category(stats.avg_high));
    
    printf("  RISK ASSESSMENT\n");
    printf("  ├─ Heat Risk      : %s\n", heat_category(stats.max_temp));
    printf("  ├─ Humidity Risk  : %s\n", humidity_category(stats.avg_humidity));
    printf("  └─ Wind Risk      : %s\n", wind_category(stats.max_wind));
    
    print_separator('=', 65);
}

void display_city_comparison(City cities[], int num_cities) {
    int i;
    printf("\n");
    print_separator('=', 80);
    printf("  MULTI-CITY COMPARISON (Weekly Averages)\n");
    print_separator('=', 80);
    printf("  %-20s %-12s %-12s %-12s %-10s %-8s\n", 
           "City", "Avg High C", "Avg Low C", "Humidity", "Wind Speed", "Category");
    print_separator('-', 80);
    
    float global_avg_high = 0, global_avg_low = 0;
    
    for (i = 0; i < num_cities; i++) {
        WeeklyStats stats = calculate_weekly_stats(&cities[i]);
        global_avg_high += stats.avg_high;
        global_avg_low += stats.avg_low;
        
        printf("  %-20s %-12.1f %-12.1f %-12.1f%% %-10.1f %-8s\n",
               cities[i].name, 
               stats.avg_high,
               stats.avg_low,
               stats.avg_humidity,
               stats.avg_wind,
               heat_category(stats.avg_high));
    }
    
    print_separator('-', 80);
    printf("  %-20s %-12.1f %-12.1f\n", 
           "GLOBAL AVERAGE", 
           global_avg_high / num_cities,
           global_avg_low / num_cities);
    print_separator('=', 80);
}

/* ============================================================
 *  FILE HANDLING - Section 6: ADD NEW CITY
 * ============================================================ */

void add_city_from_user(City cities[], int *num_cities) {
    if (*num_cities >= MAX_CITIES) {
        printf("  Maximum cities reached (%d).\n", MAX_CITIES);
        return;
    }

    City *c = &cities[*num_cities];
    int   d;
    char  days[7][15] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};

    printf("\n  Enter city name: ");
    scanf("%19s", c->name);  /* Safe input with buffer overflow protection */
    
    /* Validate city name */
    if (strlen(c->name) == 0) {
        printf("  ERROR: City name cannot be empty!\n");
        return;
    }
    
    c->num_days = MAX_DAYS;

    printf("  Enter data for 7 days (High Low Humidity Wind Condition):\n");
    printf("  NOTE: Temp (-50 to 60 C), Humidity (0-100%%), Wind (0-150 km/h)\n");
    
    for (d = 0; d < MAX_DAYS; d++) {
        int valid_input = 0;
        
        while (!valid_input) {
            printf("  %s: ", days[d]);
            int scan_result = scanf("%f %f %f %f %19s",
                  &c->forecast[d].temp_high,
                  &c->forecast[d].temp_low,
                  &c->forecast[d].humidity,
                  &c->forecast[d].wind_speed,
                  c->forecast[d].condition);
            
            /* Validate input parsing */
            if (scan_result != 5) {
                printf("  ERROR: Please enter all 5 values\n");
                while (getchar() != '\n');  /* Clear input buffer */
                continue;
            }
            
            /* Validate value ranges */
            if (!validate_temperature(c->forecast[d].temp_high)) {
                printf("  ERROR: High temp must be between -50 and 60 C\n");
                continue;
            }
            if (!validate_temperature(c->forecast[d].temp_low)) {
                printf("  ERROR: Low temp must be between -50 and 60 C\n");
                continue;
            }
            if (!validate_humidity(c->forecast[d].humidity)) {
                printf("  ERROR: Humidity must be between 0 and 100%%\n");
                continue;
            }
            if (!validate_wind_speed(c->forecast[d].wind_speed)) {
                printf("  ERROR: Wind speed must be between 0 and 150 km/h\n");
                continue;
            }
            
            /* Logical validation */
            if (c->forecast[d].temp_high < c->forecast[d].temp_low) {
                printf("  ERROR: High temp cannot be less than low temp!\n");
                continue;
            }
            
            strcpy(c->forecast[d].day, days[d]);
            valid_input = 1;
        }
    }

    (*num_cities)++;
    printf("  [OK] City '%s' added successfully!\n", c->name);

    /* Auto-save after adding */
    save_data_to_file(cities, *num_cities);

    char logmsg[60];
    sprintf(logmsg, "New city added: %s", c->name);
    log_activity(logmsg);
}

/* ============================================================
 *  DATA INITIALIZATION
 * ============================================================ */

void init_cities(City cities[], int *num_cities) {
    char *days[] = {"Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
    int d;

    strcpy(cities[0].name, "Mumbai"); cities[0].num_days = 7;
    { float h[]={34,35,33,36,37,35,34}, l[]={27,27,26,28,28,27,26};
      float hu[]={82,85,80,88,90,84,81}, w[]={15,18,12,20,22,16,14};
      char *c[]={"Cloudy","Rainy","Cloudy","Rainy","Rainy","Cloudy","Sunny"};
      for(d=0;d<7;d++){strcpy(cities[0].forecast[d].day,days[d]);strcpy(cities[0].forecast[d].condition,c[d]);
        cities[0].forecast[d].temp_high=h[d];cities[0].forecast[d].temp_low=l[d];
        cities[0].forecast[d].humidity=hu[d];cities[0].forecast[d].wind_speed=w[d];}}

    strcpy(cities[1].name, "Delhi"); cities[1].num_days = 7;
    { float h[]={42,43,41,44,43,40,39}, l[]={30,31,29,32,31,29,28};
      float hu[]={45,48,44,50,47,43,40}, w[]={10,12,8,14,11,9,10};
      char *c[]={"Sunny","Sunny","Sunny","Sunny","Cloudy","Cloudy","Sunny"};
      for(d=0;d<7;d++){strcpy(cities[1].forecast[d].day,days[d]);strcpy(cities[1].forecast[d].condition,c[d]);
        cities[1].forecast[d].temp_high=h[d];cities[1].forecast[d].temp_low=l[d];
        cities[1].forecast[d].humidity=hu[d];cities[1].forecast[d].wind_speed=w[d];}}

    strcpy(cities[2].name, "Bangalore"); cities[2].num_days = 7;
    { float h[]={28,29,27,30,28,27,29}, l[]={20,21,19,22,20,19,21};
      float hu[]={65,68,62,70,66,63,67}, w[]={12,14,10,16,13,11,12};
      char *c[]={"Pleasant","Cloudy","Sunny","Rainy","Cloudy","Sunny","Pleasant"};
      for(d=0;d<7;d++){strcpy(cities[2].forecast[d].day,days[d]);strcpy(cities[2].forecast[d].condition,c[d]);
        cities[2].forecast[d].temp_high=h[d];cities[2].forecast[d].temp_low=l[d];
        cities[2].forecast[d].humidity=hu[d];cities[2].forecast[d].wind_speed=w[d];}}

    *num_cities = 3;
}

/* ============================================================
 *  MAIN PROGRAM
 * ============================================================ */

int main(void) {
    City cities[MAX_CITIES];
    int  num_cities = 0;
    int  choice     = 0;
    int  city_choice;

    print_banner();

    /* Try loading from file first; fall back to defaults */
    if (!load_data_from_file(cities, &num_cities))
        init_cities(cities, &num_cities);

    log_activity("Program started");

    do {
        printf("\n");
        print_separator('=', 50);
        printf("  WEATHER MONITORING SYSTEM v2.0 - MAIN MENU\n");
        print_separator('=', 50);
        printf("  FORECAST & ANALYSIS\n");
        printf("     1. View 7-Day Forecast (City)\n");
        printf("     2. Temperature Graph   (City)\n");
        printf("     3. Weekly Summary      (City)\n");
        printf("     4. Compare All Cities\n");
        printf("\n  FILE & DATA MANAGEMENT\n");
        printf("     5. Save Data to File   (%s)\n", DATA_FILE);
        printf("     6. Load Data from File (%s)\n", DATA_FILE);
        printf("     7. Export Full Report  (%s)\n", REPORT_FILE);
        printf("     8. View Activity Log   (%s)\n", LOG_FILE);
        printf("     9. View Weather Alerts (%s)\n", ALERT_FILE);
        printf("    10. Add New City (& auto-save)\n");
        printf("\n  EXIT\n");
        printf("     0. Exit Program\n");
        print_separator('=', 50);
        printf("  Enter choice (0-10): ");
        scanf("%d", &choice);

        /* Options 1-3 need a city selection */
        if (choice >= 1 && choice <= 3) {
            int i;
            printf("\n  Select City:\n");
            for (i = 0; i < num_cities; i++)
                printf("  %d. %s\n", i + 1, cities[i].name);
            printf("  Enter city number: ");
            scanf("%d", &city_choice);
            city_choice--;
            if (city_choice < 0 || city_choice >= num_cities) {
                printf("  ERROR: Invalid city selection!\n"); 
                continue;
            }
        }

        switch (choice) {
            case 1: display_daily_forecast   (&cities[city_choice]);
                    log_activity("Viewed forecast"); break;
            case 2: display_temperature_graph(&cities[city_choice]);
                    log_activity("Viewed graph");   break;
            case 3: display_weekly_summary   (&cities[city_choice]);
                    log_activity("Viewed summary"); break;
            case 4: display_city_comparison  (cities, num_cities);
                    log_activity("Viewed comparison"); break;
            case 5: save_data_to_file(cities, num_cities);  break;
            case 6: load_data_from_file(cities, &num_cities); break;
            case 7: export_report(cities, num_cities);       break;
            case 8: view_log();                              break;
            case 9: view_log();
                    printf("  Note: Alerts are generated when reports are exported.\n");
                    log_activity("Viewed alerts"); break;
            case 10: add_city_from_user(cities, &num_cities); break;
            case 0: printf("\n  Goodbye! Saving activity log...\n\n");
                    log_activity("Program exited normally"); break;
            default: printf("  ERROR: Invalid option. Please enter 0-10.\n"); break;
        }

    } while (choice != 0);

    return 0;
}
