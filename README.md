# Metal Compute Shader enhanced Game Of Life Test

## Controls
q -- quit \
space -- start/stop \
leftClick -- toggle cell


## Performance
223.000.000 GOL/s - CPU only \
4.800.000.000 GOL/s - Metal shader \
(GOL/s -- Game Of Life cell updates per second)

=> 21x Performance!!

Measured on a 10000x10000 Grid without accounting for image updating and rendering cost.
Specs: MacBook Pro 16 2023 M2 Pro
