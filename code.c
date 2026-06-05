/*
 * ============================================================
 *   WEATHER MONITORING SYSTEM
 *   Features: Forecast display + FILE HANDLING (save/load)
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CITIES    5
#define MAX_DAYS      7
#define MAX_NAME_LEN  20
#define DATA_FILE     "weather_data.txt"
#define REPORT_FILE   "weather_report.txt"
#define LOG_FILE      "weather_log.txt"

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

/* ---------- Utility Functions ---------- */

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
    print_separator('=', 65);
    printf("          *  WEATHER MONITORING & FORECAST SYSTEM  *\n");
    printf("                  First Year C Programming Project\n");
    print_separator('=', 65);
    printf("\n");
}

float to_fahrenheit(float c) { return (c * 9.0f / 5.0f) + 32.0f; }

const char* get_icon(const char *condition) {
    if (strstr(condition, "Sunny"))    return "[SUNNY] ";
    if (strstr(condition, "Rain"))     return "[RAINY] ";
    if (strstr(condition, "Cloud"))    return "[CLOUD] ";
    if (strstr(condition, "Storm"))    return "[STORM] ";
    if (strstr(condition, "Snow"))     return "[SNOW]  ";
    if (strstr(condition, "Pleasant")) return "[NICE]  ";
    return "[?]     ";
}

const char* heat_category(float temp) {
    if (temp >= 40) return "DANGER - Extreme Heat";
    if (temp >= 35) return "WARNING - Very Hot";
    if (temp >= 28) return "WARM";
    if (temp >= 20) return "PLEASANT";
    if (temp >= 10) return "COOL";
    return "COLD";
}

/* Get current timestamp as string */
void get_timestamp(char *buf, int size) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(buf, size, "%Y-%m-%d %H:%M:%S", tm_info);
}

/* ============================================================
 *  FILE HANDLING — Section 1: WRITE (Save data to file)
 * ============================================================ */

/* Save all city weather data to a plain-text file */
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
    printf("  Data saved successfully to '%s'.\n", DATA_FILE);
    return 1;   /* success */
}

/* ============================================================
 *  FILE HANDLING — Section 2: READ (Load data from file)
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
    printf("  Data loaded successfully from '%s'.\n", DATA_FILE);
    return 1;
}

/* ============================================================
 *  FILE HANDLING — Section 3: APPEND (Activity Log)
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
 *  FILE HANDLING — Section 4: REPORT (Formatted text report)
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
    fprint_separator(fp, '=', 65);
    fprintf(fp, "        WEATHER MONITORING SYSTEM - FULL REPORT\n");
    fprintf(fp, "        Generated: %s\n", ts);
    fprint_separator(fp, '=', 65);
    fprintf(fp, "\n");

    for (i = 0; i < num_cities; i++) {
        City *c = &cities[i];
        float total_high = 0, total_low = 0, total_hum = 0;
        float max_t = c->forecast[0].temp_high;
        float min_t = c->forecast[0].temp_low;
        int   hot = 0, cold = 0;

        /* Section header */
        fprint_separator(fp, '-', 65);
        fprintf(fp, "  City: %s\n", c->name);
        fprint_separator(fp, '-', 65);
        fprintf(fp, "  %-10s %-8s %-8s %-10s %-8s %-12s\n",
                "Day", "High C", "Low C", "Humidity", "Wind", "Condition");
        fprint_separator(fp, '-', 65);

        for (d = 0; d < c->num_days; d++) {
            DayForecast *f = &c->forecast[d];
            fprintf(fp, "  %-10s %-8.1f %-8.1f %-10.1f %-8.1f %-12s\n",
                    f->day, f->temp_high, f->temp_low,
                    f->humidity, f->wind_speed, f->condition);

            total_high += f->temp_high;
            total_low  += f->temp_low;
            total_hum  += f->humidity;
            if (f->temp_high > max_t) { max_t = f->temp_high; hot  = d; }
            if (f->temp_low  < min_t) { min_t = f->temp_low;  cold = d; }
        }

        fprintf(fp, "\n  --- Weekly Summary ---\n");
        fprintf(fp, "  Avg High : %.1f C  (%.1f F)\n",
                total_high / c->num_days,
                to_fahrenheit(total_high / c->num_days));
        fprintf(fp, "  Avg Low  : %.1f C  (%.1f F)\n",
                total_low / c->num_days,
                to_fahrenheit(total_low / c->num_days));
        fprintf(fp, "  Avg Hum  : %.1f%%\n", total_hum / c->num_days);
        fprintf(fp, "  Hottest  : %s (%.1f C)\n",
                c->forecast[hot].day, max_t);
        fprintf(fp, "  Coldest  : %s (%.1f C)\n",
                c->forecast[cold].day, min_t);
        fprintf(fp, "  Category : %s\n\n",
                heat_category(total_high / c->num_days));
    }

    fprint_separator(fp, '=', 65);
    fprintf(fp, "  END OF REPORT\n");
    fprint_separator(fp, '=', 65);

    fclose(fp);
    printf("  Full report exported to '%s'.\n", REPORT_FILE);
    log_activity("Report exported");
}

/* ============================================================
 *  FILE HANDLING — Section 5: READ LOG (display log on screen)
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
 *  FILE HANDLING — Section 6: ADD NEW CITY (user input ? file)
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
    scanf("%s", c->name);
    c->num_days = MAX_DAYS;

    printf("  Enter data for 7 days (High Low Humidity Wind Condition):\n");
    for (d = 0; d < MAX_DAYS; d++) {
        strcpy(c->forecast[d].day, days[d]);
        printf("  %s: ", days[d]);
        scanf("%f %f %f %f %s",
              &c->forecast[d].temp_high,
              &c->forecast[d].temp_low,
              &c->forecast[d].humidity,
              &c->forecast[d].wind_speed,
              c->forecast[d].condition);
    }

    (*num_cities)++;
    printf("  City '%s' added!\n", c->name);

    /* Auto-save after adding */
    save_data_to_file(cities, *num_cities);

    char logmsg[60];
    sprintf(logmsg, "New city added: %s", c->name);
    log_activity(logmsg);
}

/* ---------- Display Functions (unchanged) ---------- */

void display_daily_forecast(const City *city) {
    int i;
    printf("\n");
    print_separator('-', 65);
    printf("  7-DAY FORECAST FOR: %s\n", city->name);
    print_separator('-', 65);
    printf("  %-12s %-8s %-8s %-10s %-8s %-12s\n",
           "Day", "High C", "Low C", "Humidity", "Wind", "Condition");
    print_separator('-', 65);
    for (i = 0; i < city->num_days; i++) {
        const DayForecast *d = &city->forecast[i];
        printf("  %-12s %s%-5.1f    %-6.1f    %-8.1f%%  %-6.1f  %-12s\n",
               d->day, get_icon(d->condition),
               d->temp_high, d->temp_low,
               d->humidity, d->wind_speed, d->condition);
    }
    print_separator('-', 65);
}

void display_temperature_graph(const City *city) {
    int i, j;
    printf("\n  Temperature Graph (High / Low) for %s\n", city->name);
    print_separator('-', 50);
    for (i = 0; i < city->num_days; i++) {
        const DayForecast *d = &city->forecast[i];
        int bh = (int)(d->temp_high / 2);
        int bl = (int)(d->temp_low  / 2);
        printf("  %-4s High |", d->day);
        for (j = 0; j < bh; j++) printf("#");
        printf(" %.1fC\n", d->temp_high);
        printf("       Low  |");
        for (j = 0; j < bl; j++) printf("-");
        printf(" %.1fC\n", d->temp_low);
    }
    print_separator('-', 50);
}

void display_weekly_summary(const City *city) {
    float th = 0, tl = 0, thum = 0;
    float max_t = city->forecast[0].temp_high;
    float min_t = city->forecast[0].temp_low;
    int   i, hot = 0, cold = 0;

    for (i = 0; i < city->num_days; i++) {
        const DayForecast *d = &city->forecast[i];
        th += d->temp_high; tl += d->temp_low; thum += d->humidity;
        if (d->temp_high > max_t) { max_t = d->temp_high; hot  = i; }
        if (d->temp_low  < min_t) { min_t = d->temp_low;  cold = i; }
    }
    printf("\n");
    print_separator('=', 65);
    printf("  WEEKLY SUMMARY FOR: %s\n", city->name);
    print_separator('=', 65);
    printf("  Avg High   : %.1f C (%.1f F)\n", th/city->num_days, to_fahrenheit(th/city->num_days));
    printf("  Avg Low    : %.1f C (%.1f F)\n", tl/city->num_days, to_fahrenheit(tl/city->num_days));
    printf("  Avg Humidity: %.1f%%\n", thum/city->num_days);
    printf("  Hottest Day : %s (%.1f C)\n", city->forecast[hot].day,  max_t);
    printf("  Coldest Day : %s (%.1f C)\n", city->forecast[cold].day, min_t);
    printf("  Category    : %s\n", heat_category(th/city->num_days));
    print_separator('=', 65);
}

void display_city_comparison(City cities[], int num_cities) {
    int i;
    printf("\n");
    print_separator('=', 65);
    printf("  CITY-WISE COMPARISON (Average High Temperature)\n");
    print_separator('=', 65);
    printf("  %-20s  %-10s  %-20s\n", "City", "Avg High C", "Status");
    print_separator('-', 65);
    for (i = 0; i < num_cities; i++) {
        int j; float t = 0;
        for (j = 0; j < cities[i].num_days; j++) t += cities[i].forecast[j].temp_high;
        printf("  %-20s  %-10.1f  %-20s\n",
               cities[i].name, t/cities[i].num_days,
               heat_category(t/cities[i].num_days));
    }
    print_separator('=', 65);
}

/* ---------- Data Initialisation ---------- */

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

/* ---------- Main ---------- */

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
        printf("\n  MAIN MENU\n");
        print_separator('-', 45);
        printf("  --- Forecast ---\n");
        printf("  1. View 7-Day Forecast (City)\n");
        printf("  2. Temperature Graph   (City)\n");
        printf("  3. Weekly Summary      (City)\n");
        printf("  4. Compare All Cities\n");
        printf("  --- File Handling ---\n");
        printf("  5. Save Data to File   (%s)\n", DATA_FILE);
        printf("  6. Load Data from File (%s)\n", DATA_FILE);
        printf("  7. Export Full Report  (%s)\n", REPORT_FILE);
        printf("  8. View Activity Log   (%s)\n", LOG_FILE);
        printf("  9. Add New City (& auto-save)\n");
        printf("  0. Exit\n");
        print_separator('-', 45);
        printf("  Enter choice (0-9): ");
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
                printf("  Invalid city!\n"); continue;
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
            case 9: add_city_from_user(cities, &num_cities); break;
            case 0: printf("\n  Goodbye! Saving log...\n");
                    log_activity("Program exited"); break;
            default: printf("  Invalid option.\n"); break;
        }

    } while (choice != 0);

    return 0;
}