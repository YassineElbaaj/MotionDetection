#!/bin/bash

g++ -fopenmp checkvideo.cpp -o motion_detection_output `pkg-config --cflags --libs opencv4`
OMP_NUM_THREADS=5 ./motion_detection_output
python3 motion_detection_plot.py 