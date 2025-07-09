# Road Safety Enhancer 🚦🏍️

An **affordable blind spot detection system** for motorcyclists, designed to enhance road safety and rider awareness using simple and accessible components.

## 🔧 Components (≈ 53€)

- ESP32 Devkit – ≈ 12€
- Adafruit Neopixel Strip (min. 6 LEDs) – ≈ 16€
- Powerbank – ≈ 12€
- 3× HC-SR04 sensors *(can be replaced by waterproof JSN-SR04T)*
- 3× 1kΩ resistors
- 3× 2.2kΩ resistors
- 1× 470Ω resistor
- 1× 1000µF 6V capacitor
- Jumper wires – ≈ 3€
- 1× Breadboard – ≈ 2€

## 🧰 Materials

For soldering:
- Solder wire, soldering paste, desoldering mesh
- Perforated plate, screws, heat shrink sleeve

## 🛠️ Tools

- Soldering iron
- Small star screwdriver
- 3D printer (for casing)

## 📦 Alternatives

- Use batteries with a voltage regulator instead of a powerbank.
- Use waterproof JSN-SR04T sensors for better durability.
- Power the system from the bike with proper voltage regulation.

## ⚙️ Circuit Assembly

Prototyping is recommended using a breadboard first.

- Each sensor:
  - VCC → ESP32 Vin
  - GND → ESP32 GND
  - TRIG → Digital pin on ESP32
  - ECHO → 1kΩ resistor → ESP32 pin and → 2.2kΩ to GND
- LED strip:
  - DIN → 470Ω resistor → ESP32 pin
  - 5V → capacitor anode → ESP32 Vin
  - GND → capacitor cathode → ESP32 GND

🔗 Online Prototype (Tinkercad, using Arduino Uno for simulation):  
[Road Safety Enhancer – Tinkercad](https://www.tinkercad.com/things/dLVjJe70Qei-road-safety-enhancer?sharecode=Z7BBWLo4VtbwdmQoFHWKGO1k9tpFJqyPfxQFXZNC0VQ)

## 💻 Programming

- Use the `.ino` file in the `/code` folder.
- Upload via [Arduino IDE](https://www.arduino.cc/en/software/).
- Install ESP32 board drivers:  
  [Windows CP210x Drivers](https://randomnerdtutorials.com/install-esp32-esp8266-usb-drivers-cp210x-windows/)

## 🖨️ 3D Printing

- `.obj` files located in `/3d printing` folder.
- Compatible with most materials.
- Designed to fit 3mm diameter, 8mm length screws.
- Optional: Add vibration-resistant material or insulation tape inside.

## 🔩 Final Assembly

- Solder most components for stability.
- Use heat-shrink sleeves to insulate.
- Housing has:
  - Access hole for powerbank button.
  - Cable exit for LED strip.
  - Optional top reinforcements with wire/thread.

## 📱 Android App (for audio alerts)

- Find the `.apk` and `.aia` files in `/android app`.
- Customize via [MIT App Inventor](https://appinventor.mit.edu/)
- View in the gallery:  
  [App Gallery Link](https://gallery.appinventor.mit.edu/?galleryid=15356150-cc52-45ef-ad10-0015a3798d69)
