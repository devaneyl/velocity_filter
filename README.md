# velocity_filter
An ALSA MIDI filter enabling velocity sensitivity for the KORG Volca FM

## Background
The Korg Volca FM ignores velocity data assiciated with "Note ON" MIDI inputs, but can accept a system velocity command.  When issued, the system velocity command will only change notes played after the command is issued.  This makes it possible to force velocity sensitivity over MIDI on the Volca FM by inserting system velocity messages before "Note On" messages.

## Operation
When a "Note On" message is received, a new system velocity message is created  with the velocity from the "Note On" message, and  is pushed before the original "Note On" message is relayed. The Volca FM will set the correct velocity before playing the note.

## Functional Diagram
![diagram](/assets/diagram.jpg)

## How to Build
* Ensure alsa-utils package is installed
* make velocity_filter

## How to Run
* Launch the filter
  * velocity_filter &
* Find MIDI device numbers
  * aconnect -l
* Connect input to velocity filter
  * aconnect _inputdevicenumber_:0 _velocityfilterdevicenumber_:0
* Connect velocity filter to Volca FM
  * aconnect _velocityfilterdevicenumber_:0 _volcadevicenumber_:0


:copyright: 2017 Louis F. Devaney