# App Flow Document for PulseFire

## Overview
This document outlines the user flow through the PulseFire application, detailing the screens, interactions, and transitions that users will experience. The flow is designed to be intuitive for controller input, with minimal complexity to focus on the core gameplay experience.

## Application States

### 1. Launch State
- **Entry Point**: User launches the PulseFire executable
- **Processing**: Loading screen displays with PulseFire logo
- **Transition**: Automatically proceeds to Main Menu once loading completes
- **Estimated Duration**: 5-15 seconds depending on hardware

### 2. Main Menu
- **Layout**: Minimalist design with game logo and two options
- **Options**:
  - Multiplayer (highlighted by default)
  - Quit
- **Navigation**: Up/down on controller D-pad or left stick
- **Selection**: A button
- **Transitions**:
  - Selecting "Multiplayer" → Multiplayer Menu
  - Selecting "Quit" → Exit Application

### 3. Multiplayer Menu
- **Layout**: Two primary options with back button
- **Options**:
  - Host Game (highlighted by default)
  - Join Game
  - Back
- **Navigation**: Up/down on controller D-pad or left stick
- **Selection**: A button
- **Transitions**:
  - Selecting "Host Game" → Host Setup Screen
  - Selecting "Join Game" → Join Game Screen
  - Selecting "Back" → Main Menu

### 4. Host Setup Screen
- **Layout**: Server information display with start and back options
- **Information Displayed**:
  - Local IP Address (for sharing with other players)
  - Connected Players (0/4 initially)
  - Player list (updates as players join)
- **Options**:
  - Start Game (enabled when at least one other player has joined)
  - Back
- **Navigation**: Up/down on controller D-pad or left stick
- **Selection**: A button
- **Transitions**:
  - Selecting "Start Game" → Game Session (Multiplayer Map)
  - Selecting "Back" → Multiplayer Menu

### 5. Join Game Screen
- **Layout**: IP input field with connect and back options
- **Input Method**:
  - On-screen keyboard navigated with controller
  - IP address format validation (xxx.xxx.xxx.xxx)
- **Options**:
  - Connect (enabled when valid IP is entered)
  - Back
- **Navigation**: Controller navigation of on-screen keyboard
- **Selection**: A button
- **Transitions**:
  - Selecting "Connect" with valid IP → Connecting Screen → Game Session
  - Selecting "Back" → Multiplayer Menu

### 6. Connecting Screen
- **Layout**: Simple loading indicator with status message
- **States**:
  - Attempting Connection
  - Connection Successful
  - Connection Failed (with error message)
- **Options**: Retry or Back (if connection fails)
- **Transitions**:
  - Connection Successful → Game Session (Waiting for Host)
  - Selecting "Back" → Join Game Screen

### 7. Game Session
- **Entry Points**:
  - From Host Setup Screen (as host)
  - From Connecting Screen (as client)
- **HUD Elements**:
  - Health indicator (top left)
  - Ammo counter (bottom right)
  - Match timer (top center)
  - Score display (top right)
- **Pause Menu Access**: Start button
- **Match End Condition**: Timer reaches 0:00
- **Transition**: Automatically to Results Screen when match ends

### 8. Pause Menu
- **Trigger**: Player presses Start button during Game Session
- **Layout**: Overlay menu with options
- **Options**:
  - Resume Game (highlighted by default)
  - Main Menu (with confirmation prompt)
- **Navigation**: Up/down on controller D-pad or left stick
- **Selection**: A button
- **Transitions**:
  - Selecting "Resume Game" → Return to Game Session
  - Selecting "Main Menu" and confirming → Main Menu

### 9. Results Screen
- **Entry Point**: Automatically at the end of a match
- **Layout**: Player rankings with scores
- **Information Displayed**:
  - Final scores for all players
  - Winner highlighted
  - Match statistics (kills, deaths, K/D ratio)
- **Options**:
  - Play Again (host only)
  - Main Menu
- **Navigation**: Up/down on controller D-pad or left stick
- **Selection**: A button
- **Transitions**:
  - Selecting "Play Again" → New Game Session with same players
  - Selecting "Main Menu" → Main Menu

## User Flow Diagram
```
Launch → Main Menu → Multiplayer Menu → Host Setup/Join Game → Game Session → Results Screen
                                                                ↑
                                                                |
                                                          Pause Menu
```

## Error Handling

### Connection Failures
- **Scenario**: Unable to connect to host
- **User Feedback**: Error message with specific reason
- **Options**: Retry or Back

### Host Disconnection
- **Scenario**: Host leaves during gameplay
- **User Feedback**: "Host has disconnected" message
- **Automatic Transition**: Main Menu after 5 seconds

### Player Disconnection
- **Scenario**: Client player disconnects
- **Host Feedback**: "[Player] has disconnected" message
- **Game Continuation**: Match continues with remaining players

## Controller Mapping

### Menu Navigation
- **D-Pad/Left Stick**: Navigate options
- **A Button**: Select option
- **B Button**: Back/Cancel
- **Start Button**: Access Pause Menu (in-game)

### Gameplay Controls
- **Left Stick**: Move character
- **Right Stick**: Look/Aim
- **A Button**: Jump
- **B Button**: Crouch
- **Right Bumper**: Aim down sights
- **Right Trigger**: Shoot
- **X Button**: Reload
- **Start Button**: Pause Menu