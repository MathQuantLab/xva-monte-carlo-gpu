#!/bin/bash

echo "Initiating config script..."
which nvcc &> /dev/null

if [ $? -eq 0 ]; then
    echo "Command nvcc detected. No config needed"
else
    echo "Command not detected. Installing..."

    sudo apt update
    sudo apt install -y nvidia-cuda-toolkit make gcc g++

    echo "Successfully installed required tools"
fi

exit 0