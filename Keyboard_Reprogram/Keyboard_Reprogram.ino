// Keyboard Reprogram
// 
// Allows you to detect different keyboards from hotkey scripts by checking which F13-F24 Key is pressed.
//
// E.g. with Auto Hotkey:
// ----------------------
// #SingleInstance force
// #InstallKeybdHook
// 
// #if (getKeyState("F13", "P"))
// F13::return
// 
// ; normal hotkeys from first keyboard connected to arduino e.g.
// .::Send {U+2026}
//
// #if
// ----------------------

#include <hidboot.h>
#include <usbhub.h>

#include <SPI.h>
#include <Keyboard.h>

// In this File:
// - KeyboardWrapper
// - Global USB Hub Variables
// - setup() & loop()

//==================
// KeyboardWrapper:
//==================
const int modifierKeys[] = {KEY_F13, KEY_F14, KEY_F15, KEY_F16, KEY_F17, KEY_F18, KEY_F19, KEY_F20, KEY_F21, KEY_F22, KEY_F23, KEY_F24};

// A KeyboardReportParser that relays all pressed keys to the PC
// with an assigned modifier/wrap Key that gets relayed along side the other keys
class KeyboardWrapper : public KeyboardReportParser
{
  public:
    KeyboardWrapper(char wrapKey);
  private:
    uint8_t wrapKey;
    bool normalKeyPressed;
  protected:
    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnControlKeysChanged(uint8_t before, uint8_t after);
    void PressWrapKey(uint8_t mod, uint8_t key);
};

KeyboardWrapper::KeyboardWrapper(char varWrapKey) {
  wrapKey=varWrapKey;
}

// Handle pressing/releasing modifier keys
void KeyboardWrapper::OnControlKeysChanged(uint8_t before, uint8_t after) 
{
  // nothing on input is pressed anymore, release all for PC
  if (after == 0 && !normalKeyPressed) {
    Keyboard.releaseAll();
    normalKeyPressed = false; // on PC side this isn't the case anymore
    exit;
  }

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  // This is the first key that gets pressed. Press the WrapKey first
  if (!normalKeyPressed && before == 0 && after != 0)
    PressWrapKey(after, 0);
    
  // pressing new keys
  if(beforeMod.bmLeftCtrl == 0 && afterMod.bmLeftCtrl == 1 ) Keyboard.press(KEY_LEFT_CTRL);
  if(beforeMod.bmLeftShift == 0 && afterMod.bmLeftShift == 1 ) Keyboard.press(KEY_LEFT_SHIFT);
  if(beforeMod.bmLeftAlt == 0 && afterMod.bmLeftAlt == 1 ) Keyboard.press(KEY_LEFT_ALT);
  if(beforeMod.bmLeftGUI == 0 && afterMod.bmLeftGUI == 1 ) Keyboard.press(KEY_LEFT_GUI);
  if(beforeMod.bmRightCtrl == 0 && afterMod.bmRightCtrl == 1 ) Keyboard.press(KEY_RIGHT_CTRL);
  if(beforeMod.bmRightShift == 0 && afterMod.bmRightShift == 1 ) Keyboard.press(KEY_RIGHT_SHIFT);
  if(beforeMod.bmRightAlt == 0 && afterMod.bmRightAlt == 1 ) Keyboard.press(KEY_RIGHT_ALT);
  if(beforeMod.bmRightGUI == 0 && afterMod.bmRightGUI == 1 ) Keyboard.press(KEY_RIGHT_GUI);

  // releasing old keys
  if(beforeMod.bmLeftCtrl == 1 && afterMod.bmLeftCtrl == 0 ) Keyboard.release(KEY_LEFT_CTRL);
  if(beforeMod.bmLeftShift == 1 && afterMod.bmLeftShift == 0 ) Keyboard.release(KEY_LEFT_SHIFT);
  if(beforeMod.bmLeftAlt == 1 && afterMod.bmLeftAlt == 0 ) Keyboard.release(KEY_LEFT_ALT);
  if(beforeMod.bmLeftGUI == 1 && afterMod.bmLeftGUI == 0 ) Keyboard.release(KEY_LEFT_GUI);
  if(beforeMod.bmRightCtrl == 1 && afterMod.bmRightCtrl == 0 ) Keyboard.release(KEY_RIGHT_CTRL);
  if(beforeMod.bmRightShift == 1 && afterMod.bmRightShift == 0 ) Keyboard.release(KEY_RIGHT_SHIFT);
  if(beforeMod.bmRightAlt == 1 && afterMod.bmRightAlt == 0 ) Keyboard.release(KEY_RIGHT_ALT);
  if(beforeMod.bmRightGUI == 1 && afterMod.bmRightGUI == 0 ) Keyboard.release(KEY_RIGHT_GUI);
}

// On KeyDown relay key down events to PC
void KeyboardWrapper::OnKeyDown(uint8_t mod, uint8_t key)
{
  normalKeyPressed = true;
  
  // For some reason the keys from KeyboardReportParser are offset by 136
  // And OemToAscii does not work for all keys on a keyboard (and we are not interested in lower/uppercase difference
  // uint8_t c = OemToAscii(mod, key);
  uint8_t c = key + 136;
  
  if(c) {
    PressWrapKey(mod, key);
    Keyboard.press(c);
  }
}

// On KeyUp we release all for PC (otherwise we would need to store a list of keys to release)
void KeyboardWrapper::OnKeyUp(uint8_t mod, uint8_t key)
{
  normalKeyPressed = false;
  Keyboard.releaseAll();
}

// Press the assigned modifier/wrap key
// maybe have special handling depending on which Keyboard/Keys pressed
void KeyboardWrapper::PressWrapKey(uint8_t mod, uint8_t key)
{
  // for some reason the keys from KeyboardReportParser are offset by 136
  uint8_t c = key + 136;

  //// If you want special cases for keyboards do it like this:
  //if(wrapKey==KEY_F13) {
  //  // If you want special cases for some keys. E.g.like this:
  //  if(c >= KEY_F1 && c <= KEY_F12) {
  //    Keyboard.press(KEY_LEFT_CTRL);
  //    Keyboard.press(KEY_LEFT_SHIFT);
  //  } else {
  //    Keyboard.press(KEY_LEFT_ALT);
  //    Keyboard.press(KEY_LEFT_CTRL);
  //    Keyboard.press(KEY_LEFT_SHIFT);
  //  }
  //} else {
    Keyboard.press(wrapKey);
  //}
}

//===========================
// Global USB Hub Variables:
//===========================

USB     Usb;
USBHub     Hub1(&Usb);
USBHub     Hub2(&Usb);
USBHub     Hub3(&Usb);
USBHub     Hub4(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard1(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard2(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard3(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard4(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard5(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard6(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard7(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard8(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard9(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard10(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard11(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    Keyboard12(&Usb);
KeyboardWrapper Wrapper1(modifierKeys[0]);
KeyboardWrapper Wrapper2(modifierKeys[1]);
KeyboardWrapper Wrapper3(modifierKeys[2]);
KeyboardWrapper Wrapper4(modifierKeys[3]);
KeyboardWrapper Wrapper5(modifierKeys[4]);
KeyboardWrapper Wrapper6(modifierKeys[5]);
KeyboardWrapper Wrapper7(modifierKeys[6]);
KeyboardWrapper Wrapper8(modifierKeys[7]);
KeyboardWrapper Wrapper9(modifierKeys[8]);
KeyboardWrapper Wrapper10(modifierKeys[9]);
KeyboardWrapper Wrapper11(modifierKeys[10]);
KeyboardWrapper Wrapper12(modifierKeys[11]);

//==================
// setup() & loop()
//==================
void setup()
{
  // safe guard to prevent not beeing able to reprogram arduino
  // on some arduino types that can happen if serial, keyboard and/or mouse input gets sent to pc constantly directly after boot
  pinMode(LED_BUILTIN, OUTPUT);
  for (int i=1; i < 20; i++){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000-(50*i));
    digitalWrite(LED_BUILTIN, LOW);
    delay(50*i);
  }
  // safe guard end
  
  Usb.Init();
  
  Keyboard1.SetReportParser(0, &Wrapper1);
  Keyboard2.SetReportParser(0, &Wrapper2);
  Keyboard3.SetReportParser(0, &Wrapper3);
  Keyboard4.SetReportParser(0, &Wrapper4);
  Keyboard5.SetReportParser(0, &Wrapper5);
  Keyboard6.SetReportParser(0, &Wrapper6);
  Keyboard7.SetReportParser(0, &Wrapper7);
  Keyboard8.SetReportParser(0, &Wrapper8);
  Keyboard9.SetReportParser(0, &Wrapper9);
  Keyboard10.SetReportParser(0, &Wrapper10);
  Keyboard11.SetReportParser(0, &Wrapper11);
  Keyboard12.SetReportParser(0, &Wrapper12);
  Keyboard.begin();
}

void loop()
{
    Usb.Task();
}
