# PulseFire

## Overview
*PulseFire* is a first-person shooter (FPS) prototype developed using Unreal Engine 5.4, designed to deliver a thrilling multiplayer experience with responsive controller-based gameplay. Inspired by *Tom Clancy's Rainbow Six Vegas* and *Call of Duty*, it blends tactical depth with fast-paced action. This prototype supports 2-4 players over a local area network (LAN) on a compact multiplayer map, serving as a proof-of-concept to showcase core mechanics and attract funding for full development.

## Features
- **Multiplayer Gameplay**: Supports 2-4 players in LAN-based matches, with a listen server hosting the game session.
- **Controller-Optimized Mechanics**: Responsive movement (walk, jump, crouch), shooting, and reloading, tailored for Xbox-style controllers.
- **User Interface**: Includes a main menu, multiplayer menu, in-game HUD (health, ammo, timer), pause menu, and results screen.
- **Match Structure**: Timed matches (5 minutes) with kill-based scoring, ending with a results screen displaying final scores.
- **Performance**: Targets 60 FPS on mid-range hardware (e.g., RTX 3060 equivalent) with input latency below 50ms.

## Development Setup
1. **Prerequisites**:
   - Unreal Engine 5.4
   - Visual Studio 2022 (Windows) or Xcode (Mac)
   - Git

2. **Getting Started**:
   ```bash
   # Clone the repository
   git clone https://github.com/GEMDevEng/PulseFire.git
   
   # Navigate to the project directory
   cd PulseFire
   
   # Open the project in Unreal Engine
   # Double-click PulseFire.uproject
   ```

## Project Structure
- **/Config** - Engine configuration files
- **/Content** - Game assets and Blueprints
  - **/Animations** - Character and weapon animations
  - **/Blueprints** - Game logic in Blueprint format
  - **/Maps** - Game levels and test environments
  - **/Materials** - Material assets for visual appearance
  - **/Meshes** - 3D models
  - **/Sounds** - Audio assets
  - **/Textures** - Texture assets
- **/Source** - C++ source code

## Documentation
For detailed documentation, please refer to the [docs](../docs) directory.
