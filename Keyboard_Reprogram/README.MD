# Keyboard Reprogram

Allows you to detect different keyboards from hotkey scripts by checking which F13-F24 Key is pressed.

## Resources & Dependencies

### Hardware

- An Arduino compatible to the Keyboard library (Leonardo, Esplora, Zero, Due and MKR Family)
- An Arduino [USB-Host-Shield](https://chome.nerpa.tech/arduino_usb_host_shield_projects/)
- Some keyboard (maybe without n-key rollover/ghosting workarounds)

### Software

- [Arduino IDE](https://www.arduino.cc/en/software) (of course)
- [USB_Host_Shield_2.0](https://github.com/felis/USB_Host_Shield_2.0) Library
- (Optional) [AutoHotkey (AHK)](https://autohotkey.com/)  
If hotkeys should be modified further on pc (and the arduino is only to differenciate between the keyboards) …

## How to set up

1. Get the Hardware
2. Install and start the Arduino IDE
3. Sketch → Include Library → Manage Libraries… → Search for "USB Host Shield Library 2.0" → Install
4. Open the [Keyboard_Reprogram.ino](Keyboard_Reprogram/Keyboard_Reprogram.ino) from this repository
5. Attach a compatibe Arduino (with the host-shield)
6. Optionally: modify the behaviour
7. Push the "Upload" button

Optionally: Install "AutoHotkey"

## How to use

1. Attach the Keyboard(s) to the Host Shield  
With USB-Hubs if multiple
2. Attach the Arduino to the PC
3. Wait for the blinking to stop (~20s)
4. Optionally: Run the [AutoHotkey script](Keyboard_Reprogram/Keyboard_Reprogram_Keyboard_1.ahk) (modified for your needs)
