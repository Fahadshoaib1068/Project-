# 🎴 UNO Game (2-Player Console Version)

A simple **2-player UNO game** implemented in C++ that runs in the terminal with colorful ASCII cards and full UNO rules support.  
This project demonstrates **array manipulation, game logic, file handling, and console graphics** using Windows API colors.

---

## 📖 About
This is a **console-based simulation of UNO** for 2 players. The game includes:
- Dealing cards
- Playing turns with valid card matching
- Handling **special cards** (Skip, Reverse, +2, Wild, +4)
- Penalty system for forgetting to call "UNO"
- Automatic score tracking and saving results to a file

---

## ✨ Features
- 🎨 **Colored card display** using `windows.h` console text attributes  
- 🎴 Supports all UNO card types:
  - Number cards (0–9)
  - Skip, Reverse, +2
  - Wild, Wild +4
- ⏩ Skip & Reverse turn mechanics  
- ⚡ Draw penalties (+2, +4, or forgetting UNO)  
- 📝 Game history saved in `game_history.txt`  
- 🎮 Simple text-based menu:
  - Play Game
  - See Score
  - Exit

---

## 🛠 Installation & Run

### Requirements
- Windows OS (uses `windows.h` for colors)
- C++ Compiler (MinGW, MSVC, or g++ with Windows support)

### Steps
1. Clone this repository:
   ```bash
   git clone https://github.com/your-username/uno-game.git
   cd uno-game
  
2. **Compile the source code using a C++ compiler:**
   ```sh
   g++ UNO\ GAME.cpp -o uno.exe
   ```

## 🚀 Usage
From the main menu, press:

P → Play Game

S → See Score

E → Exit

During gameplay:

Play a card by entering its index number.

Enter 0 to draw a card.

When at 2 cards, type U to call UNO — or face a penalty of drawing 2 extra cards!


## 📜 License

This project is licensed under the MIT License.
You’re free to use, modify, and share it.


