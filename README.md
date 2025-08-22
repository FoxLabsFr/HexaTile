# 🤖 HexaTile - Modules with Delta motion and led display

A module with 3-DOF and led display to create smooth motions, visual animation.
The end goal is to be able to link multiple modules to create big patterns

Watch the creation process and demo video:

[![HexaTile Creation Process](https://img.shields.io/badge/YouTube-Watch%20Demo-red.svg)](https://www.youtube.com/watch?v=a0t3uPxUgBI)

## 🚀 Features

- [x] **Delta 3-DOF Motion** IK for height, roll, and pitch control using 3 servo motors
- [x] **LED Display**: Animations, images, color, etc.
- [ ] **Chain Modules** Communication through i2c
- [ ] **Neighbor detection** Automatic detection of modules around to adapt animations
- [ ] **Add module to FoxLabs store**

**Note:** If you are aware this is still a work in progress, but want to get a prototype PCB as in the video, feel free to contact us.

## 🏗️ Hardware & Specifications

- **Arduino Nano** - The brain of the platform (ATmega328P)
- **3 Servo Motors** - Micro servos (pins D3, D5, D7)
- **LED Display**: [FoxLabs hexagonal led matrix](https://github.com/FoxLabsFr/Hexagonal_leds_matrix_WS2812B) (pin D6)

## 📁 Project Structure

```
HexaTile/
├── 3D/                    # 3D printed parts
│   ├── arm1.STL           # Servo arm component 1
│   ├── arm2.STL           # Servo arm component 2
│   ├── base.STL           # Platform base
│   └── top.STL            # Platform top plate
├── code/                  # Arduino source code
│   ├── platformio.ini     # PlatformIO configuration
│   └── src/
│       ├── main.ino       # Main entry point with demo motion
│       ├── config/config.h # Hardware configuration
│       └── Delta/         # Delta class
│           ├── Delta.cpp  # Delta class with IK and demo display
│           └── Delta.h    # Delta class definition
└── README.md
```

## ⚙️ Setup

The configuration is in `code/src/config/config.h`:

```cpp
// LED Matrix configuration
#define LED_PIN 6
#define LED_COUNT 144

// Servo configuration
#define S1_PIN 7
#define S2_PIN 5
#define S3_PIN 3

// Servo parameters
#define SERVO_MIN_PULSE 500
#define SERVO_MAX_PULSE 2500
#define SERVO_MIN_ANGLE 1000
#define SERVO_MAX_ANGLE 1750

// Delta Maximum roll/pitch angle in degrees
#define DELTA_RANGE_ANGLE 30.0
```

## 🛠️ Getting Started

### What You Need

- [VS Code](https://code.visualstudio.com/)
- [PlatformIO Extension](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) for VS Code

### Steps

1. **Get the code**

   ```bash
   git clone https://github.com/FoxLabsFr/HexaTile.git
   cd HexaTile
   ```

2. **Upload the code**

   ```bash
   cd Code
   pio run --target upload
   ```

Libraries (PlatformIO installs them automatically from platformio.ini):

- [ServoGroup](https://github.com/FoxLabsFr/ServoGroup.git) - Synchronized servo motion control
- [MatrixController](https://github.com/FoxLabsFr/MatrixController.git) - LED matrix visualization
- [Adafruit NeoPixel](https://github.com/adafruit/Adafruit_NeoPixel) - Dependency of MatrixController
- [Arduino Servo Library](https://github.com/arduino-libraries/Servo) - Dependency of ServoGroup

## 📄 License

This project is licensed under Creative Commons Attribution-NonCommercial (CC BY-NC)<br>
with a special clause reserving all commercial rights to the original author. See [LICENSE](LICENSE) for details.

**Built with ❤️ by FoxLabs**

_This README was generated with AI assistance._
