# Usage Guide

## Getting Started

### Compilation

```bash
gcc code.c -o weather_system -Wall
```

### Running the Program

```bash
./weather_system
```

On Windows:
```bash
weather_system.exe
```

## Main Menu Navigation

Upon launching, you'll see the main menu with 11 options (0-10).

### Option 1: View 7-Day Forecast (City)

**What it does:** Displays a detailed 7-day weather forecast for a selected city.

**Output includes:**
- Day of the week
- High temperature (°C)
- Low temperature (°C)
- Humidity percentage
- Wind speed (km/h)
- Weather condition
- Status indicator (OK, HIGH, WARN, DANGER)

**Example:**
```
  7-DAY FORECAST FOR: Mumbai
  ===== Day | High C | Low C | Humidity | Wind | Condition | Status
  Monday   | 34.0   | 27.0  | 82%      | 15   | Cloudy    | OK
```

**Status Indicators:**
- `[OK]` - Normal weather
- `[HIGH]` - Elevated humidity/wind
- `[WARN]` - High temperature warning (>=35°C)
- `[DANGER]` - Extreme heat (>=40°C)

---

### Option 2: Temperature Graph (City)

**What it does:** Creates an ASCII-based visualization of temperature trends.

**Output includes:**
- Visual representation of daily high temperatures (█ blocks)
- Visual representation of daily low temperatures (- blocks)
- Exact temperature values in Celsius
- Scaled for easy comparison

**Example:**
```
  TEMPERATURE VISUALIZATION - Mumbai
  Mon  High |████████████ 34.0 C
       Low  |----------- 27.0 C
```

---

### Option 3: Weekly Summary (City)

**What it does:** Provides comprehensive statistics for the selected city.

**Categories:**

**TEMPERATURE ANALYSIS**
- Average high and low (with Fahrenheit conversion)
- Hottest day with temperature
- Coldest day with temperature
- Temperature range (max - min)

**WIND & HUMIDITY ANALYSIS**
- Average humidity with risk category
- Average wind speed with classification
- Maximum wind speed recorded

**RISK ASSESSMENT**
- Heat risk classification
- Humidity risk level
- Wind risk level

---

### Option 4: Compare All Cities

**What it does:** Side-by-side comparison of all loaded cities.

**Displays:**
- All city names
- Average high temperature for each
- Average low temperature for each
- Average humidity percentage
- Average wind speed
- Heat category classification

**Global statistics:**
- Global average high temperature
- Global average low temperature

---

### Option 5: Save Data to File

**What it does:** Saves current weather data to `weather_data.txt`

**File format:**
```
# Weather Monitoring System - Data File
# Saved on: 2024-08-18 14:35:22
NUM_CITIES 3

CITY Mumbai 7
DAY Mon 34.0 27.0 82.0 15.0 Cloudy
DAY Tue 35.0 27.0 85.0 18.0 Rainy
...
```

**Features:**
- Timestamped save records
- Human-readable plaintext format
- Can be manually edited
- Safe for backup and sharing

---

### Option 6: Load Data from File

**What it does:** Loads weather data from `weather_data.txt`

**Behavior:**
- Replaces current data with file data
- Handles missing files gracefully
- Falls back to default data if file not found
- Validates data format during loading

---

### Option 7: Export Full Report

**What it does:** Generates comprehensive analysis report to `weather_report.txt`

**Report contents:**
1. **Header** with generation timestamp
2. **For each city:**
   - Complete 7-day forecast table
   - Weekly analytics section:
     - Average temperatures (C and F)
     - Max/min temperatures with dates
     - Average humidity with risk level
     - Average wind with classification
     - Climate classification

3. **Footer** with summary statistics

**Automatically triggers:**
- Extreme weather alert generation
- Activity log update

---

### Option 8: View Activity Log

**What it does:** Displays the activity log from `weather_log.txt`

**Logged activities:**
- Program start/exit
- Data save/load operations
- Report generation
- City additions
- User actions (viewing forecast, summary, etc.)
- Alert generation

**Log format:**
```
[2024-08-18 14:35:22] Program started
[2024-08-18 14:36:05] Viewed forecast
[2024-08-18 14:37:12] New city added: Pune
```

---

### Option 9: View Weather Alerts

**What it does:** Displays extreme weather alerts from `weather_alerts.txt`

**Alert types:**
1. **Extreme Heat Alerts** (Temperature >= 40°C)
2. **High Humidity Alerts** (Humidity >= 85%)
3. **Severe Wind Alerts** (Wind >= 25 km/h)

**Alert format:**
```
[2024-08-18 14:37:12] ALERT: Delhi - Wed has EXTREME HEAT (42.0 C)
[2024-08-18 14:37:12] ALERT: Mumbai - Tue has HIGH HUMIDITY (85.0%)
```

---

### Option 10: Add New City

**What it does:** Interactive input to add a new city to the system.

**Process:**
1. Enter city name (max 20 characters)
2. For each day (Mon-Sun), enter:
   - High temperature (must be > low temp)
   - Low temperature
   - Humidity (0-100%)
   - Wind speed (0-150 km/h)
   - Weather condition (e.g., "Sunny", "Rainy")

**Validation:**
- Rejects empty city names
- Temperature range: -50°C to 60°C
- Humidity: 0-100%
- Wind: 0-150 km/h
- High > Low temperature check
- Rejects invalid input with helpful error messages

**Auto-save:**
Data is automatically saved after adding a city.

---

### Option 0: Exit Program

**What it does:** Gracefully exits the program.

**Before exiting:**
- Logs program exit to activity file
- Closes all open file handles
- Returns to system

---

## Temperature Classification Categories

### Heat Categories
- **DANGER - Extreme Heat**: >= 40°C
- **WARNING - Very Hot**: >= 35°C
- **WARM**: >= 28°C
- **PLEASANT**: >= 20°C
- **COOL**: >= 10°C
- **COLD**: < 10°C

### Humidity Categories
- **DANGER - Very High**: >= 85%
- **HIGH**: >= 70%
- **MODERATE**: >= 40%
- **LOW**: < 40%

### Wind Categories
- **DANGER - Severe**: >= 25 km/h
- **STRONG**: >= 20 km/h
- **MODERATE**: >= 10 km/h
- **LIGHT**: < 10 km/h

---

## Generated Files

### weather_data.txt
- **Purpose**: Data backup and portability
- **Format**: Plaintext with structured format
- **Created by**: Option 5, Option 10
- **Used by**: Option 6

### weather_report.txt
- **Purpose**: Professional analysis documentation
- **Format**: Formatted text with statistics
- **Created by**: Option 7
- **Size**: 2-5 KB per report

### weather_log.txt
- **Purpose**: Activity audit trail
- **Format**: Timestamped log entries
- **Appended by**: Every action
- **Viewed by**: Option 8

### weather_alerts.txt
- **Purpose**: Weather emergency tracking
- **Format**: Timestamped alert entries
- **Created by**: Option 7 (report export)
- **Viewed by**: Option 9

---

## Tips & Best Practices

### Data Management
1. **Backup frequently**: Use Option 5 to save data regularly
2. **Export reports**: Create reports for documentation
3. **Check logs**: Review activity logs for audit trails
4. **Monitor alerts**: Keep track of extreme weather conditions

### Accurate Forecasting
1. Enter realistic temperature ranges
2. Ensure high temp > low temp
3. Use standard weather condition names
4. Keep humidity between 0-100%
5. Use realistic wind speeds

### Analysis
1. Compare cities to understand climate differences
2. Look for temperature patterns in graphs
3. Review weekly summaries for trends
4. Monitor humidity and wind for health impacts
5. Use alerts for emergency preparedness

### Troubleshooting
- **Data not loading?**: Check that `weather_data.txt` exists in the same directory
- **File write errors?**: Ensure directory has write permissions
- **Invalid input?**: Follow the range guidelines shown in prompts
- **Want defaults back?**: Delete `weather_data.txt` and restart

---

## Example Workflow

```
1. Launch program
   └─> Loads data or shows defaults (Mumbai, Delhi, Bangalore)

2. View forecast for Mumbai
   └─> Select Option 1, choose Mumbai

3. Compare all cities
   └─> Select Option 4

4. Add new city (Pune)
   └─> Select Option 10, enter data (auto-saves)

5. Export professional report
   └─> Select Option 7 (generates alerts too)

6. View activity log
   └─> Select Option 8

7. Save before exit
   └─> Select Option 5 (optional, already auto-saved)

8. Exit gracefully
   └─> Select Option 0
```

---

## System Requirements

- **Language**: C (C89/C99 compatible)
- **Compiler**: GCC or compatible C compiler
- **Runtime**: Windows, Linux, or macOS
- **Storage**: ~100 KB (code + data files)
- **Memory**: ~50 KB RAM
- **File Access**: Read/Write permissions for data directory

---

## Limitations & Notes

- Maximum 5 cities can be stored simultaneously
- 7-day forecast cycles (fixed for all cities)
- Data is stored in plaintext (suitable for small datasets)
- No real-time weather API integration
- Manual data entry required
- Windows line-ending compatible

---

## Support for Common Tasks

### "How to reset to default data?"
Delete `weather_data.txt` and restart the program.

### "How to backup my data?"
Use Option 5 to save, then copy `weather_data.txt` to backup location.

### "How to add data from a different system?"
Create `weather_data.txt` in the correct format and place in program directory.

### "How to generate a report for documentation?"
Use Option 7 to export a professionally formatted report.

### "How to track what actions were performed?"
Review `weather_log.txt` (Option 8) for timestamped activity history.
