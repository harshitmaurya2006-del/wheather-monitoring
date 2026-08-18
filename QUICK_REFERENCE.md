# Quick Reference Card

## 🚀 Quick Start

```bash
# Compile
gcc code.c -o weather_system -Wall

# Run
./weather_system

# Expected Output:
# [ASCII banner]
# MAIN MENU with options 1-10
```

## 📊 Main Menu Options

| Opt | Feature | Input | Output |
|-----|---------|-------|--------|
| 1 | View Forecast | City # | 7-day table |
| 2 | Temperature Graph | City # | ASCII visualization |
| 3 | Weekly Summary | City # | Statistics + risk |
| 4 | Compare Cities | - | Multi-city table |
| 5 | Save Data | - | weather_data.txt |
| 6 | Load Data | - | Read from file |
| 7 | Export Report | - | weather_report.txt + alerts |
| 8 | View Activity Log | - | weather_log.txt content |
| 9 | View Alerts | - | weather_alerts.txt content |
| 10 | Add City | User input | weather_data.txt |
| 0 | Exit | - | Save log + exit |

## 🌡️ Temperature Thresholds

**Heat Categories**
- DANGER ≥ 40°C
- WARNING ≥ 35°C
- WARM ≥ 28°C
- PLEASANT ≥ 20°C
- COOL ≥ 10°C
- COLD < 10°C

**Humidity Categories**
- DANGER ≥ 85%
- HIGH ≥ 70%
- MODERATE ≥ 40%
- LOW < 40%

**Wind Categories**
- DANGER ≥ 25 km/h
- STRONG ≥ 20 km/h
- MODERATE ≥ 10 km/h
- LIGHT < 10 km/h

## 📁 Files Generated

| File | Type | Created By | Read By |
|------|------|-----------|---------|
| weather_data.txt | Data | Option 5, 10 | Option 6, startup |
| weather_report.txt | Report | Option 7 | Manual viewing |
| weather_log.txt | Log | Every action | Option 8 |
| weather_alerts.txt | Alerts | Option 7 | Option 9 |

## ✅ Validation Rules

**Temperature**: -50°C to 60°C
**Humidity**: 0% to 100%
**Wind**: 0 to 150 km/h
**High Temp > Low Temp**: Required
**City Name**: Max 20 characters

## 📈 Data Calculations

| Metric | Formula | Used In |
|--------|---------|---------|
| Avg High | Sum / 7 days | Summary, Report |
| Avg Low | Sum / 7 days | Summary, Report |
| Avg Humidity | Sum / 7 days | Summary, Report |
| Avg Wind | Sum / 7 days | Summary, Report |
| To Fahrenheit | (C × 9/5) + 32 | Reports, Display |

## 🎨 Key Data Structures

```c
// Single day forecast
DayForecast {
  char day[15];           // "Mon", "Tue", etc
  float temp_high;        // -50 to 60
  float temp_low;         // -50 to 60
  float humidity;         // 0 to 100
  float wind_speed;       // 0 to 150
  char condition[20];     // "Sunny", "Rainy", etc
}

// City with 7-day forecast
City {
  char name[20];
  DayForecast forecast[7];
  int num_days;           // Always 7
}

// Calculated weekly stats
WeeklyStats {
  float avg_high, avg_low, avg_humidity, avg_wind;
  float max_temp, min_temp, max_wind;
  int hottest_day, coldest_day;
}
```

## 🔍 Main Functions

**Utilities**
- `print_separator()` - Draw lines
- `to_fahrenheit()` - Convert temp
- `heat_category()` - Classify temp
- `humidity_category()` - Classify humidity
- `wind_category()` - Classify wind
- `get_timestamp()` - Get time string

**Analytics**
- `calculate_weekly_stats()` - Compute statistics
- `check_extreme_weather_alerts()` - Generate alerts

**File I/O**
- `save_data_to_file()` - Write data
- `load_data_from_file()` - Read data
- `log_activity()` - Append to log
- `export_report()` - Generate report
- `view_log()` - Display log

**Display**
- `display_daily_forecast()` - Show table
- `display_temperature_graph()` - Show graph
- `display_weekly_summary()` - Show stats
- `display_city_comparison()` - Compare cities

**Data**
- `init_cities()` - Load defaults
- `add_city_from_user()` - User input

## ⚠️ Error Messages

| Error | Cause | Fix |
|-------|-------|-----|
| "Could not open file" | File I/O failed | Check disk space |
| "Invalid city!" | Out of range | Select 1-N |
| "ERROR: High temp must be..." | Out of range | Use -50 to 60 |
| "ERROR: High < Low" | Logic error | Swap values |
| "Maximum cities reached" | Limit (5) | Delete/Load data |

## 📋 Pre-loaded Cities

**Mumbai** - Cloudy, Rainy, Warm
- Avg High: 35°C | Avg Low: 27°C

**Delhi** - Sunny, Hot, Dry
- Avg High: 42°C | Avg Low: 30°C

**Bangalore** - Pleasant, Moderate
- Avg High: 28°C | Avg Low: 20°C

## 🎯 Common Workflows

### View All Forecasts
```
→ Option 1 → Select City 1 (Mumbai)
→ Option 1 → Select City 2 (Delhi)
→ Option 1 → Select City 3 (Bangalore)
```

### Generate Report & Alerts
```
→ Option 7 → Creates weather_report.txt + weather_alerts.txt
→ Option 8 → View weather_log.txt
```

### Backup & Load Data
```
→ Option 5 → Save to weather_data.txt
→ Option 6 → Load from weather_data.txt
```

### Add New City & Save
```
→ Option 10 → Enter city data (auto-saves)
→ Option 4 → Compare all cities including new one
```

## 🔧 Compilation Variants

```bash
# Standard
gcc code.c -o weather_system

# With warnings
gcc code.c -o weather_system -Wall -Wextra

# Debug build
gcc code.c -o weather_system -g

# Optimized
gcc code.c -o weather_system -O2

# Static linking
gcc code.c -o weather_system -static
```

## 📊 Console Output Example

```
7-DAY FORECAST FOR: Mumbai
=========================================================================
Day        High C  Low C  Humidity Wind     Condition Status
---------  ------  -----  -------- -----    --------- --------
Mon        [SUN]34.0     27.0     82%      15        Cloudy   [OK]
Tue        [RAIN]35.0    27.0     85%      18        Rainy    [HIGH]
Wed        [CLOU]33.0    26.0     80%      12        Cloudy   [OK]
...
=========================================================================
```

## 🎓 Learning Checklist

After studying this project, you should understand:
- [ ] C struct composition and arrays of structs
- [ ] File I/O operations (read, write, append)
- [ ] String handling and scanf buffer management
- [ ] Dynamic calculation and statistical analysis
- [ ] Input validation and error handling
- [ ] Menu-driven application design
- [ ] Modular code organization
- [ ] Time handling with strftime
- [ ] Floating-point arithmetic
- [ ] Professional code documentation

## 🚀 Next Steps

1. **Understand the code** - Read TECHNICAL.md
2. **Learn the features** - Read FEATURES.md
3. **Use the program** - Follow USAGE_GUIDE.md
4. **Extend it** - Try the enhancements in PROJECT_SUMMARY.md
5. **Share it** - This is portfolio-ready!

---

**Pro Tips:**
- Run with all warnings: `gcc code.c -Wall -Wextra`
- Test with invalid input to see validation
- Review generated .txt files for output format
- Check weather_log.txt to see activity history
- Export a report to see comprehensive analytics

**Version**: 2.0 | **Status**: Production-Ready ✅
