# PulseFire

## Overview

*PulseFire* is a first-person shooter (FPS) prototype developed using Unreal Engine 5.4, designed to deliver a thrilling multiplayer experience with responsive controller-based gameplay. Inspired by *Tom Clancy's Rainbow Six Vegas* and *Call of Duty*, it blends tactical depth with fast-paced action. This prototype supports 2-4 players over a local area network (LAN) on a compact multiplayer map, serving as a proof-of-concept to showcase core mechanics and attract funding for full development.

## Features

- **Enhanced Movement System**: Fluid movement with sprint, slide, and mantling mechanics for dynamic gameplay.
- **Weapon Variety**: Multiple weapon types including assault rifles, shotguns, and sniper rifles, each with unique characteristics.
- **Game Modes**: Multiple game modes including Deathmatch, Team Deathmatch, and Capture the Flag.
- **Multiplayer Gameplay**: Supports 2-4 players in LAN-based matches, with a listen server hosting the game session.
- **Controller-Optimized Mechanics**: Responsive movement, shooting, and reloading, tailored for Xbox-style controllers.
- **User Interface**: Includes a main menu, multiplayer menu, in-game HUD, pause menu, and results screen.
- **Audio System**: Dynamic sound system with spatial audio, footsteps, and adaptive music.
- **Visual Effects**: High-quality particle effects, post-processing, and visual feedback.
- **Performance Optimization**: Automatic performance scaling, LOD management, and profiling tools.
- **Analytics and Feedback**: Built-in analytics and feedback systems for tracking usage and collecting user input.

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
  - **/PulseFire** - Main game code
    - **/Components** - Game components (health, movement, etc.)
    - **/Weapons** - Weapon system implementation
    - **/Multiplayer** - Multiplayer functionality
    - **/UI** - User interface implementation
    - **/Performance** - Performance optimization tools
    - **/Audio** - Audio system implementation
    - **/VFX** - Visual effects implementation
    - **/Settings** - Game settings management
    - **/Analytics** - Analytics and feedback systems
- **/Documentation** - Project documentation
  - **/UserManual** - Instructions for players
  - **/DeveloperGuide** - Guide for developers
  - **/API** - API reference

## Documentation

For detailed documentation, please refer to the following resources:

- [User Manual](Documentation/UserManual/README.md) - Instructions for players
- [Developer Guide](Documentation/DeveloperGuide/README.md) - Guide for developers
- [API Reference](Documentation/API/README.md) - Detailed API documentation

## Building the Project

The project includes build scripts for both Windows and Unix-based systems:

### Windows

```batch
# Build development version
Build.bat -c Development

# Build shipping version
Build.bat -c Shipping -a

# Generate documentation
Build.bat --docs
```

### Unix (macOS/Linux)

```bash
# Build development version
./Build.sh -c Development

# Build shipping version
./Build.sh -c Shipping -a

# Generate documentation
./Build.sh --docs
```

## Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Commit your changes: `git commit -m 'Add some feature'`
4. Push to the branch: `git push origin feature/your-feature-name`
5. Submit a pull request
