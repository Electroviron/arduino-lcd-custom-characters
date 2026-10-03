# Arduino LCD Custom Characters ❤️

A hands-on Arduino project exploring how to create and display **custom characters** on a 16×2 character LCD.

Instead of displaying only the characters already built into the LCD, this project creates a custom heart symbol using a **5×8 pixel matrix**, stores it in the LCD's **CGRAM**, and displays it alongside normal text.

This project builds on the basic LCD concepts learned in previous projects and introduces the LCD's internal custom-character memory.

---

## 📌 What This Project Does

The Arduino creates a custom heart:

```text
  ██ ██
  █████
  █████
   ███
    █
```

and displays it on the LCD:

```text
┌────────────────┐
│                │
│ ♥ elctronics   │
└────────────────┘
```

The character is created entirely from binary data.

---

# 🧰 Components

### Hardware

* Arduino Uno
* 16×2 character LCD
* Breadboard
* Jumper wires
* 10 kΩ potentiometer for LCD contrast

### Software

* Arduino IDE
* `LiquidCrystal` library

---

# 🔌 LCD Connections

The LCD is operated in **4-bit mode**.

| LCD Pin | Function        |   Arduino Pin |
| ------- | --------------- | ------------: |
| VSS     | Ground          |           GND |
| VDD     | +5 V            |            5V |
| V0      | Contrast        | Potentiometer |
| RS      | Register Select |            D8 |
| RW      | Read/Write      |           GND |
| E       | Enable          |            D9 |
| D4      | Data            |            D4 |
| D5      | Data            |            D5 |
| D6      | Data            |            D6 |
| D7      | Data            |            D7 |
| A       | Backlight +     |           5V* |
| K       | Backlight −     |           GND |

* Backlight current limiting depends on the LCD module being used.

---

# 🧠 How Custom Characters Work

A standard character LCD, such as an HD44780-compatible 16×2 display, normally contains a built-in character set.

For example, it already knows how to display:

```text
A
B
C
1
2
3
!
?
```

But the LCD also provides a small area of RAM called:

**CGRAM — Character Generator RAM**

CGRAM allows us to define our own characters.

---

# 🔲 The 5×8 Character Matrix

A custom character is built using a matrix that is:

```text
5 pixels wide
8 pixels high
```

For example:

```text
00000
00000
01010
11111
11111
01110
00100
00000
```

Each row contains **5 bits**.

Each bit controls one pixel:

```text
0 → OFF
1 → ON
```

For example:

```text
01010
```

can be visualized as:

```text
· █ · █ ·
```

where:

```text
0 = pixel OFF
1 = pixel ON
```

Combining all eight rows produces the complete character.

---

# ❤️ Creating the Heart

The heart is represented in the Arduino program as an array containing eight bytes:

```cpp
byte heart[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};
```

There are exactly **8 rows**, corresponding to the 8-pixel height of the character.

The binary notation makes the pixel pattern easy to visualize.

For example:

```cpp
0b01010
```

represents:

```text
· █ · █ ·
```

---

# 💾 CGRAM Character Slots

The LCD provides space for **8 custom characters** at a time.

The slots are numbered:

```text
0
1
2
3
4
5
6
7
```

The heart is stored in slot `0`:

```cpp
lcd.createChar(0, heart);
```

This tells the LCD:

> Store the pixel pattern contained in `heart` in custom-character location 0.

Other custom characters could be stored in the remaining locations:

```cpp
lcd.createChar(1, character1);
lcd.createChar(2, character2);
lcd.createChar(3, character3);
```

and so on.

---

# 🖥️ Displaying the Character

Once the character has been stored, it can be displayed using:

```cpp
lcd.write(byte(0));
```

The `0` refers to the custom-character slot.

So:

```cpp
lcd.createChar(0, heart);
```

means:

```text
Store heart → slot 0
```

while:

```cpp
lcd.write(byte(0));
```

means:

```text
Display character → slot 0
```

The two operations work together:

```text
heart[8]
   ↓
lcd.createChar(0, heart)
   ↓
CGRAM slot 0
   ↓
lcd.write(byte(0))
   ↓
♥
```

---

# 💻 Complete Code

```cpp
#include <LiquidCrystal.h>

// LCD pins
#define RS 8
#define EN 9

#define D4 4
#define D5 5
#define D6 6
#define D7 7

// Create LCD object
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Define custom character
byte heart[8] = {
  0b00000,
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

void setup() {

  lcd.begin(16, 2);

  // Store custom character in CGRAM slot 0
  lcd.createChar(0, heart);

  // Move cursor to column 0, row 1
  lcd.setCursor(0, 1);

  // Display custom character
  lcd.write(byte(0));

  // Display normal text
  lcd.print("elctronics");
}

void loop() {

}
```

---

# 🧩 Important Functions

## `lcd.createChar()`

```cpp
lcd.createChar(0, heart);
```

Creates a custom character and stores it in one of the LCD's custom-character locations.

Syntax:

```cpp
lcd.createChar(location, character);
```

Where:

* `location` = custom character slot (`0–7`)
* `character` = 8-byte character array

---

## `lcd.write()`

```cpp
lcd.write(byte(0));
```

Writes a character directly to the LCD.

For this project, the value refers to the custom-character location.

This is different from:

```cpp
lcd.print("A");
```

`print()` is generally used to print readable text or formatted values, while `write()` can send a specific character code directly.

---

# 🧪 Experimenting With Characters

Once the basic heart works, the interesting part is creating your own designs.

For example:

```text
00000
00100
01110
11111
00100
00100
00100
00000
```

could produce an arrow-like symbol.

You can design characters by thinking of them as an 8-row drawing:

```text
Row 1 → 00000
Row 2 → 00100
Row 3 → 01110
Row 4 → 11111
Row 5 → 00100
Row 6 → 00100
Row 7 → 00100
Row 8 → 00000
```

This makes the LCD behave almost like a tiny **5×8 monochrome display**.

---

# 🐛 Problem Encountered

## `'lcd' does not name a type`

While developing the project, the following line initially produced an error:

```cpp
lcd.createChar(0, heart);
```

The problem was that the function call had been placed **outside of a function**.

For example:

```cpp
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

byte heart[8] = {
  ...
};

lcd.createChar(0, heart); // ❌
```

The Arduino/C++ compiler interpreted this as something that didn't belong in the global declaration area.

The solution was to place the instruction inside `setup()`:

```cpp
void setup() {

  lcd.begin(16, 2);

  lcd.createChar(0, heart);

}
```

This introduced an important C++ concept:

### Declarations vs executable statements

Global declarations can define things such as:

```cpp
byte heart[8];
LiquidCrystal lcd(...);
```

while instructions that need to execute should be placed inside functions:

```cpp
lcd.begin(...);
lcd.createChar(...);
lcd.print(...);
```

---

# 📚 Concepts Learned

### LCD

* CGRAM
* Custom character storage
* 5×8 pixel matrix
* LCD character memory
* Custom-character slots
* `lcd.createChar()`
* `lcd.write()`

### Binary

* Binary representation
* Individual bits representing pixels
* 5-bit pixel rows
* Byte arrays

### C/C++

* Arrays
* `byte`
* Function calls
* Global declarations
* Executable statements
* Scope and program structure

### Embedded Systems

* Working with limited memory
* Creating data structures for hardware
* Translating software data into physical output
* Understanding what a library function is actually configuring

---

# 🔬 What Is Happening Underneath?

The Arduino code looks simple:

```cpp
lcd.createChar(0, heart);
```

But conceptually, several things are happening:

```text
Arduino
   │
   │  8-byte character pattern
   ↓
LiquidCrystal library
   │
   │  LCD commands/data
   ↓
HD44780 LCD controller
   │
   ↓
CGRAM
   │
   └── Custom character slot 0
             │
             ↓
          LCD display
```

This is an important step toward understanding that Arduino libraries are **abstractions over hardware communication**.

Later, this project can be revisited without `LiquidCrystal` to understand exactly how the Arduino communicates with the HD44780 controller.

---

# 🚧 Future Experiments

Possible extensions to this project:

* [ ] Create all 8 custom-character slots
* [ ] Create arrows for a menu system
* [ ] Create battery icons
* [ ] Create signal-strength icons
* [ ] Create temperature symbols
* [ ] Create animation frames
* [ ] Create a loading animation
* [ ] Build a custom LCD menu using custom characters
* [ ] Explore CGRAM limitations
* [ ] Control the LCD directly without `LiquidCrystal`

---

# 🎯 Project Goal

The goal of this project was not simply to create a heart.

The goal was to understand that a character LCD can be treated as a small programmable display where custom pixel patterns can be stored in memory and later displayed.

This project is the foundation for the next stage:

> **Building an LCD menu system using buttons, states, and custom characters.**
