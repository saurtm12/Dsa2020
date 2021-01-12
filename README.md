# Repository info:
prg1: Phase 1
prog2: Phase 2
Phase 2 is a continuation of phase 1, the course made a seperation to grade easier.
The project contain an UI about the stops. If you want to use UI, use Qt creator. The other option is that normal compiling with gcc/g++ then run as command line.
# NOTE! About available test files in prg1

Short explanation of test files:
- Files named "...-in.txt" are input files containing commands for different tests.
- Files named "...-out.txt" are output files containing expected output of the corresponding input.

You can either just run the input file with command read "...-in.txt" or compare your own output with the expected output with command testread "...-in.txt" "...-out.txt" (the graphical UI's file selector allows you to select both files at once by control-clicking with mouse).

Files named "perftest-....txt" are performance tests of various types of operations. They can be run with command read "perftest-....txt".

Correctness tests:
- simpletest-compulsory/all-in/out.txt: Really simple test to find most obvious bugs
- example-compulsory/all-in/out.txt: The example in the assignment description, showing basic operation of all commands
- Tretest-compulsory/all-in/out.txt: More tests using real Tampere center bus stop data

Additionally, the following data files exist as data for your own testing:
- Tre-Ksksta-....txt: Real bus stop data of Tampere center (~37 stops, ~7 regions)
- Tre-A-....txt: Real bus stop data of Tampere pay zone A (~542 stops, ~12 regions)
- Tre-ABC-....txt: Real bus stop data of Tampere pay zones A, B, and C (~2296 stops, ~33 regions)
