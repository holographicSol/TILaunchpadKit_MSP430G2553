Serial

TXD in main transmits once for example sake.

RXD as an interrupt sub routine checks received data against internal commands in rxdexec.

Variables in the MCU can be modified usefully over RXD with packaged type conversions due to
limited space on the MSP430G2553 for running orthadox library functions to achieve the same effects.
