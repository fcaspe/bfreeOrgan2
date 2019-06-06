# bfreeOrgan2
## A flexYnth-based Tonewheel Organ Synthesizer
### By Franco Caspe (francocaspe@hotmail.com)

**bfreeOrgan2** is a C++ additive syntesizer project based on the **flexYnth** development library.

## Introduction
I love the Hammond Organ! but (to date, June 2019) there was no open-source homebrew box that implements a nice Drawbar Organ, so I decided to code one myself.

In the efforts of doing so, I learned a lot of efficient programming on C++ so I opted to create a base library to modularize the code, and to allow me to easily 
expand this project in the future, or to develop new synths! Thus creating **flexYnth** and **bfreeOrgan2**.

## Scope
This project implements a modularly-coded tonewheel drawbar organ synthesizer, implemented on two different **flexYnth Engines**, each one in different branches.
1. A Linux/ALSA testbench implementation.
2. A STM32F4 Discovery Board Implementation.

## Features

 * 10 voice Polyphony.
 * Real time 9 Drawbar control.
 * Chorus effect with adjustable Rate and Amplitude.
 * Envelope and Optional Click Generator.
 * Commands are controled over MIDI CC's. (See the MIDI mapping in utils/vmpk.ins)
 * 16bit@48Khz Mono output.

# Build it!

## On Linux/ALSA:

NOTE: Make sure you have the Alsa Development Libs installed.

*FIRST switch to the linux_alsa branch:* 

1. Run: 'git checkout linux_alsa'
2. Run 'make'
3. Execute the testbench: './bin/testbench'

An ALSA MIDI port will open for you to connect your favorite controller! ( I use alsaconnectgui )

## For the Discovery STM32F407

*FIRST switch to the Discovery_F407 branch.* 

1. Run: 'git checkout discovery_f4'

NOTE: sure the arm-gcc toolset path is set in the PATH bash's enviroment variable when you run make.

2. Run: 'make'
3. Connect the STM32F4 Discovery Board to the PC.
4. Run: 'make flash'

After the Discovery Board is flashed, connect the User USB Port to your computer. It will recognize a MIDI Device.
Route a controller to the MIDI input of the Board. The audio output is provided by the 3.5mm Jack of the Discovery Board.

## License

Released under Apache V2.0 License.