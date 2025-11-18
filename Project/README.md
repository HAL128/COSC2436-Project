## Typing Game (COSC2436-H01 Project)

### Project Overview

This is a console-based typing game built in C++ that challenges players to type sentences as accurately as possible within a 2-minute time limit. The game features a real-time countdown with a visual progress bar and a scoring system that rewards accuracy with points and sentence completion bonuses while penalizing mistakes with both point deductions and time penalties.

The project implements several key features to enhance gameplay experience. A stack-based duplicate prevention algorithm ensures that no sentence repeats until all available sentences have been used, providing varied content throughout the game session. Players can view their performance on a top 5 leaderboard that tracks personal bests, and they have the ability to add their own custom sentences to expand the game's content. Throughout gameplay, color-coded visual feedback displays typing progress with completed text in green, the current character in yellow, and remaining text in white.

### Class Structure
```
Game (Main Controller)
„¥„Ÿ Player (User information management)
„¥„Ÿ Sentence (Typing progress tracking)
„¤„Ÿ JSONManager (Data persistence)
   „¤„Ÿ PlayerScore (Ranking data structure)
```

### Project Files
```
„¥„Ÿ Game.h / Game.cpp
„¥„Ÿ Player.h / Player.cpp
„¥„Ÿ Sentence.h / Sentence.cpp
„¥„Ÿ JSONManager.h / JSONManager.cpp
„¥„Ÿ main.cpp
„¥„Ÿ sentences.json
„¤„Ÿ scores.json
```
