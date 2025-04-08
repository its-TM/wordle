# 🎮 Wordle in C

Do YOU have what it takes to guess a 5 letter word in under 6 guesses in the terminal ⁉️
This project is a simple terminal-based clone of the popular word guessing game Wordle purely written in C, featuring color-coded feedback and 5 letter input validation!

![Game Preview](https://imgur.com/a/XMR5ii9.png)

## Features
- ✅ Exact letter match (GREEN)
- ❓ Letter match but in the wrong position (YELLOW)
- ❌ Letter isn't in the word (RED)
- ⌨️ Input validation for a 5 letter word
- 🎲 Random word selection from a collection of over 1200+ words!

## How to Play
1. Clone the repository
   ```bash
   git clone https://github.com/yourusername/wordle.git
   ```
2. Compile the game
   ```bash
   gcc wordle.c -o wordle
   ```
3. Run the executable
   ```bash
   ./wordle
   ```

## Requirements
- GCC compiler
- Linux/macOS terminal (or Windows with WSL)

## File Structure
```
wordle/
├── wordle.c        # Main game code
├── wordle.txt      # Word list
└──  README.md      # This file
```

## Contributing
Pull requests welcome! Feel free to open an issue to contribute.
