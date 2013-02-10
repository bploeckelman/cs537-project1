#!/bin/bash

clear; make clean; make;
valgrind --track-origins=yes --leak-check=full ./537sh

