#!/bin/bash

export AFL_PATH=/home/suhas/Downloads/AFL/mafl-2.35b/
export AFL_KEEP_ASSEMBLY=1
export TMPDIR=.
export AFL_SKIP_CPUFREQ=1
export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1

sudo sysctl -w kernel.core_pattern="core"