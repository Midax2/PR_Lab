#!/bin/bash

run_command() {
	"$@" &
	PID=$!
	echo "Process $PID has started"
}

commands=(
	"sleep 10"
	"ping -c 10 example.com"
	"ls -l /"
	"sleep 15"
	"df -h"
	"sleep 20"
	"uptime"
)

for command in "${commands[@]}"; do
	run_command $command
done

wait

echo "Finish!"
