#!/bin/zsh

width=25;
population=500;
days=5;
range=2;
transmission_distance=1;
recovery_time=14;

make simulation

echo "$width\n$population\n$days\n$range\n$transmission_distance\n$recovery_time\n" | ./simulation