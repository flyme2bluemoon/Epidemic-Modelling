# Epidemic Modelling

Epidemic Simulator is a C program that can be used to run a basic simulation of an epidemic.

## Inspiration

This CS50 final project was inspired by the 2019-20 novel Coronavirus Disease (COVID-19) pandemic.

## Installation

The simulation can be installed using git clone and building it using make. A `Mach-O 64-bit executable` and `ELF 64-bit LSB executable` have also been included for your convenience.
```sh
git clone https://github.com/flyme2bluemoon/Epidemic-Modelling.git
cd Epidemic-Modelling
make simulation
```

## Usage

Run and then enter your desired values when prompted.
```sh
./simulation
```
The following is an example of some test values.
```
Width (recommended: 10-100): 600
Population (requirement: < width * width): 288000
Numbers of days (recommended: 30): 40
Range of movements (recommended: 2): 10
Range of virus transmission (recommended: 1): 2
Chance of transmission (recommended: 50, range: 0-100): 50
Recovery time (recommended: 14): 14
Chance of death (recommended: 2, range: 0-100): 5
```

### Optional Arguments

```
Usage: ./simulation [--help] [--display-map] [--display-status] [--verbose]

optional arguments:
  -h, --help             show this help message and exit.
  --display-map          Display the map after every simulated day.
  --display-status       Display the status after every simulated day..
  --display-counts       Displays the daily and cumulative case and death counts in stdin.
  --verbose              Behaves as if --display-map and --display-status were specified.
```

## How the code works

Here is a basic explaination of how the code works...

### Data structure

There is a map array that contains -1 if a spot is empty or the id of a person. The id of a person can be used to index into the people array which contains information about the different people. I have created a custom Person type that contains a person's coordinates, status and the number of people they have infected.

### Algorithm

When run, the program simulated each day in a loop. For each day, it moves people around, simulated virus transmission. count the cases and deaths and calculates the basic reproduction number. Then a summary is displayed at the end. Many functions use the random function to help create a realistic simulation. The random function is initalized using the time.

## Output

This program prints to `stdout` as well as files. By default, it prints the daily summary and final summary to `stdout` and creates csv files for daily and cumulative case and death counts. These files can be opened in Microsoft Excel, Apple Numbers, Google Sheets or you favourite spreadsheet editor to create a nice graph.

### Example outputs

Here are some sample outputs

#### Example daily output

```
[*] Day 15
==========
Incrementing...
Moving...
Simulating virus transmission...
Tallying case counts...
Daily case count: 15430
Daily death count: 6
Basic Reproduction Number: 1.108949
```

#### Example summary output

```
====SUMMARY====
Patient Zero: 12141
Percentage infected: 79.68%
Total number of cases: 229482
Death rate: 0.536861%
Total number of deaths: 1232
Basic Reproduction Number: 9.232426
Percentage of population immunized to stop spread required: 89.17%
```