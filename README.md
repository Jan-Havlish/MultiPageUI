# MultiPageUI

MultiPageUI is a lightweight UI library for the **Wio Terminal** that provides building blocks for creating multi-page user interfaces with scrolling, color themes, and 5-way navigation input.

### Features
- Widgets: `Label`, `Button`, `CheckBox`, `RadioButton`, `Link`
- `Page` and `PageManager` for multi-page navigation
- Built-in **color themes** (Default, Red, Blue, Green)
- Handles 5-way joystick/button navigation (up, down, left, right, press)

Includes a working **demo** (`examples/DemoUI`) showing how to assemble a full interactive interface.

---

## Installation

1. Download the latest release as a ZIP.
2. In Arduino IDE: `Sketch → Include Library → Add .ZIP Library…`
3. Select the ZIP file.
4. Open `File → Examples → MultiPageUI → DemoUI`.

---

## Dependencies
- [Wio Terminal TFT_eSPI](https://wiki.seeedstudio.com/Wio-Terminal-LCD-Overview/) (must be installed)

---

## About
This library was prototyped and generated with the help of **free-tier LLMs** (OpenAI, Anthropic, Google) to quickly provide a PySimpleGUI-like interface for the Wio Terminal.

---

## License
Distributed under the MIT License. See [LICENSE](LICENSE) for details.
