#!/bin/bash

CODEPATH="" # enter your path here
GEM5PATH="" # enter your gem5 path here
SHAPATH="$CODEPATH/SHA256"
SIPPATH="$CODEPATH/SipHash24"

: > $CODEPATH/benchmark_scripts/output.txt
: > $CODEPATH/benchmark_scripts/memoutput.txt
: > $CODEPATH/benchmark_scripts/cacheoutput.txt
: > $CODEPATH/benchmark_scripts/cycleoutput.txt
: > $CODEPATH/benchmark_scripts/sipoutput.txt
: > $CODEPATH/benchmark_scripts/sipmemoutput.txt
: > $CODEPATH/benchmark_scripts/sipcacheoutput.txt
: > $CODEPATH/benchmark_scripts/sipcycleoutput.txt

gem5opts[0]="--l1d_size=64kB"
gem5opts[1]="--l1i_size=64kB"
gem5opts[2]="--l1d_assoc=4"
gem5opts[3]="--l1i_assoc=4"
gem5opts[4]="--mem-size=8192MB"

sipscriptopts[0]="$CODEPATH/benchmark_scripts/input.txt"
sipscriptopts[1]="qjskjxurstpvtenn"

GEM5OPTS="--cpu-type=MinorCPU --caches"
for i in {0..4}; do
    GEM5OPTS="${GEM5OPTS} ${gem5opts[i]}"
done

SCRIPTOPTS="$CODEPATH/benchmark_scripts/input.txt"

SIPSCRIPTOPTS=""
for i in {0..1}; do
    SIPSCRIPTOPTS="${SIPSCRIPTOPTS} ${sipscriptopts[i]}"
done

riscv64-unknown-linux-gnu-gcc $SHAPATH/sha256.c $SHAPATH/sha256_test.c -o $SHAPATH/sha256test -static -O2
riscv64-unknown-linux-gnu-gcc $SHAPATH/asm_sha256.c $SHAPATH/asm_sha256_test.c -o $SHAPATH/asm_sha256test -static -O2

riscv64-unknown-linux-gnu-gcc $SIPPATH/siphash24.c $SIPPATH/siphash24_test.c -o $SIPPATH/siphash24test -static -O2
riscv64-unknown-linux-gnu-gcc $SIPPATH/asm_siphash24.c $SIPPATH/siphash24_test.c -o $SIPPATH/asm_siphash24test -static -O2

## SHA256

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/asm_sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

GEM5OPTS="${GEM5OPTS} --cacheline_size=256"

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/asm_sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

GEM5OPTS="${GEM5OPTS} --l1i-hwp-type=TaggedPrefetcher --l1d-hwp-type=TaggedPrefetcher"

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/asm_sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

GEM5OPTS="${GEM5OPTS} --bp-type=LTAGE"

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/output.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/memoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/cycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SHAPATH/asm_sha256test --options="$length $SCRIPTOPTS"
    python3 getter.py;
done

## SIPHASH

GEM5OPTS="--cpu-type=MinorCPU --caches"
for i in {0..4}; do
    GEM5OPTS="${GEM5OPTS} ${gem5opts[i]}"
done

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/asm_siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

GEM5OPTS="${GEM5OPTS} --cacheline_size=256"

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/asm_siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

GEM5OPTS="${GEM5OPTS} --l1i-hwp-type=TaggedPrefetcher --l1d-hwp-type=TaggedPrefetcher"

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/asm_siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

GEM5OPTS="${GEM5OPTS} --bp-type=LTAGE"

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done

printf "\n" >> $CODEPATH/benchmark_scripts/sipoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipmemoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcacheoutput.txt;
printf "\n" >> $CODEPATH/benchmark_scripts/sipcycleoutput.txt;

for length in {100..10000..100}; do
    $GEM5PATH/build/RISCV/gem5.opt $GEM5PATH/configs/example/se.py $GEM5OPTS -c $SIPPATH/asm_siphash24test --options="$length $SIPSCRIPTOPTS"
    python3 sipgetter.py;
done


