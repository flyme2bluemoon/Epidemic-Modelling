#!/bin/zsh

width=25;
population=5;
days=5;
range=2;
recovery_time=14;

make simulation

echo "$width\n$population\n$days\n$range\n$recovery_time\n" | ./simulation