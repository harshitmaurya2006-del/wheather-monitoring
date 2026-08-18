# 🌡️ Weather Monitoring & Forecast System

A comprehensive **weather monitoring and forecasting application** built in C with advanced file handling, multi-city support, and intelligent analytics.

## ✨ Features

### 📊 Core Functionality
- **7-Day Weather Forecasts** - Display detailed forecasts for multiple cities
- **Multi-City Management** - Track up to 5 cities simultaneously
- **Temperature Analytics** - Calculate averages, highs, lows, and trends
- **Visual Representations** - ASCII-based temperature graphs and comparisons
- **Weather Alerts** - Automatic warnings for extreme weather conditions

### 📁 Advanced File Handling
- **Data Persistence** - Save/load weather data in plain-text format
- **Report Generation** - Export comprehensive weather reports with statistics
- **Activity Logging** - Timestamped logs of all system activities
- **Auto-Save** - Automatic data saving when cities are added

### 🔍 Analytics & Insights
- **Weekly Summaries** - Average temperatures, humidity, wind speed
- **Heat Category Classification** - DANGER, WARNING, WARM, PLEASANT, COOL, COLD
- **City Comparisons** - Side-by-side analysis of multiple cities
- **Extreme Weather Detection** - Identifies hottest/coldest days
- **Temperature Unit Conversion** - Celsius to Fahrenheit conversion

## 🎯 Use Cases
- Academic weather data analysis
- City climate comparison
- Weather alert monitoring
- Historical weather tracking and reporting
- Educational tool for learning C programming concepts

## 📋 Data Structure

### City Record
```
City Name: [up to 20 characters]
├── 7-Day Forecast
│   ├── High Temperature (°C)
│   ├── Low Temperature (°C)
│   ├── Humidity (%)
│   ├── Wind Speed (km/h)
│   └── Weather Condition
```

### Pre-loaded Cities
- **Mumbai** - Tropical, high humidity
- **Delhi** - Extreme heat in summer
- **Bangalore** - Moderate, pleasant climate

## 📖 Menu Options

| Option | Feature |
|--------|---------|
| 1 | View 7-Day Forecast for a city |
| 2 | Display Temperature Graph |
| 3 | Show Weekly Summary with Statistics |
| 4 | Compare Average Temperatures Across Cities |
| 5 | Save All Data to File |
| 6 | Load Data from File |
| 7 | Export Full Weather Report |
| 8 | View Activity Log |
| 9 | Add New City |
| 0 | Exit Program |

## 🛠️ Technical Stack
- **Language:** C (C89/C99 compatible)
- **File I/O:** Standard fopen/fclose operations
- **Data Format:** Plaintext with structured parsing
- **Time Handling:** System timestamps with strftime()

## 📊 Generated Files
- `weather_data.txt` - Serialized weather data
- `weather_report.txt` - Formatted analysis report
- `weather_log.txt` - Activity audit trail

## 🎓 Learning Outcomes
- File handling in C (read, write, append)
- Struct data structures and arrays
- String manipulation and parsing
- User input validation and menu systems
- Timestamp and time zone handling
- Data analysis and statistics

## 🚀 Future Enhancements
- Search/filter by city name or condition
- Weather trend analysis
- CSV export functionality
- Wind speed and humidity alerts
- Multi-day average calculations
- Data backup and version control
