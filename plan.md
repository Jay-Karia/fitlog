# The Planning Document

## Commands

### 1. Add Workout

```bash
fitlog add <exercise> <sets> <reps> [weight] [options]

# Examples
fitlog add "Bench Press" 3 8 2.5
fitlog add "Pull Ups" 3 8
fitlog add "Pull Ups" 3 8 --notes "-2 Reps on set number 2"
fitlog add "Pull Ups" 3 8 --date 2025-9-8

# Options
--date YYYY-MM-DD
--notes "Your note"
```

### 2. Display Workout/s

```bash
fitlog show [options]

# Examples
fitlog show                      # today's workout
fitlog show --date 2025-09-07    # workouts on specific date
fitlog show --recent             # last N=5 workouts (configurable)
fitlog show --all                # all workouts
fitlog show --week               # this week's workouts
fitlog show --verbose            # add with other options (not a standalone one)
```

### 3. Remove Workout

```bash
fitlog delete [options]

# Examples
fitlog delete --date 2025-09-07          # delete workouts on specific date
fitlog delete --exercise "Bad Form Squats" --date 2025-09-08 # delete a specific workout
fitlog delete --last                      # delete last workout

# Options
--confirm           # skip confirmation prompt
```

### 4. Stats

```bash
fitlog stats [exercise] [options]

# Examples
fitlog stats                     # overall statistics
fitlog stats "Bench Press"       # exercise-specific stats
fitlog stats --exercise "Squats" --days 30
fitlog stats --pr                # personal records

# Options
--exercise NAME      # specific exercise
--days N            # last N days
--weeks N           # last N weeks
--pr                # personal records only
```

### 5. Config

```bash
fitlog config <subcommand> [options]

fitlog config show [key]         # show current config
fitlog config set <key> <value>  # set config value
fitlog config reset [key]        # reset to default
fitlog config list              # list all config options

```

### 6. Export/Import Data

```bash
fitlog export [options]
fitlog import <file> [options]

# Examples
fitlog export --csv workouts.csv
fitlog export --json --days 30 recent.json
fitlog import workouts.csv
fitlog import --merge olddata.json

# Export Options
--csv               # CSV format
--json              # JSON format
--days N            # last N days only
--exercise NAME     # specific exercise only

# Import Options
--merge             # merge with existing data
--replace           # replace existing data
--preview           # preview import without saving
```

## Global Options

```bash
-h, --help          # show help
-v, --version       # show version
```

## Features

- Add workouts with specified sets and reps and an optional date input
- Delete workouts by date, specific workout on a data, or the last workout. Followed by a confirmation statement showing the brief of the workout.
- Display workouts (all, current day, current week, recent, date)
- See the stats of all time, of a specific workout, or filter by last N dyas
- See personal records
- Import and export your local workout database

## Config Options

```ini
# Date and time formatting
date_format=YYYY-MM-DD              # YYYY-MM-DD, MM/DD/YYYY, DD/MM/YYYY, DD-MMM-YYYY
time_format=24h                     # 12h, 24h
show_timestamps=true                # show time with exercises

# Units and measurements
weight_unit=lbs                     # lbs, kg
distance_unit=miles                 # miles, km (for cardio)
decimal_places=1                    # precision for weight display
```
