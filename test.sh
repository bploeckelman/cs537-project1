#!/bin/bash

clear; make clean; make;
valgrind ./537sh -v --track-origins=yes --leak-check=full

