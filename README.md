# PulseFire

## Overview
*PulseFire* is a first-person shooter (FPS) prototype developed using Unreal Engine 5.4, designed to deliver a thrilling multiplayer experience with responsive controller-based gameplay. Inspired by *Tom Clancy's Rainbow Six Vegas* and *Call of Duty*, it blends tactical depth with fast-paced action. This prototype supports 2-4 players over a local area network (LAN) on a compact multiplayer map, serving as a proof-of-concept to showcase core mechanics and attract funding for full development.

The project targets FPS enthusiasts aged 18-34 who prefer controller input, offering intuitive controls, a minimalistic user interface, and a competitive match structure. Built within a $1,000 budget, *PulseFire* leverages Unreal Engine's robust features and existing assets to maximize quality and efficiency.

## Features
- **Multiplayer Gameplay**: Supports 2-4 players in LAN-based matches, with a listen server hosting the game session.
- **Controller-Optimized Mechanics**: Responsive movement (walk, jump, crouch), shooting, and reloading, tailored for Xbox-style controllers.
- **User Interface**: Includes a main menu, multiplayer menu, in-game HUD (health, ammo, timer), pause menu, and results screen.
- **Match Structure**: Timed matches (5 minutes) with kill-based scoring, ending with a results screen displaying final scores.
- **Performance**: Targets 60 FPS on mid-range hardware (e.g., RTX 3060 equivalent) with input latency below 50ms.

## Prerequisites
To run *PulseFire*, ensure you have the following:
- **Operating System**: Windows 10 or 11 (64-bit)
- **Hardware**:
  - CPU: Intel Core i5 or equivalent
  - GPU: NVIDIA RTX 3060 or equivalent
  - RAM: 8 GB minimum
  - Storage: 2 GB free space
- **Input Device**: Xbox-style controller (wired or wireless)
- **Network**: Local Area Network (LAN) for multiplayer
- **Software**: None required beyond the game executable

## Installation
1. **Download the Build**:
   - Obtain the *PulseFire* executable (`PulseFire.exe`) from the provided secure file-sharing link.
2. **Extract Files**:
   - Unzip the downloaded archive to a folder on your computer (e.g., `C:\PulseFire`).
3. **Ensure Controller Setup**:
   - Connect your controller before launching the game.
4. **Configure Network**:
   - Ensure all players are on the same LAN with firewall settings allowing Unreal Engine traffic (UDP ports 7777-7778).

## Usage
Follow these steps to play *PulseFire*:

### Starting the Game
1. Launch `PulseFire.exe` from the installation folder.
2. The game opens to the main menu with two options: *Multiplayer* and *Quit*.

### Hosting a Multiplayer Game
1. Select *Multiplayer* from the main menu.
2. Choose *Host Game* to start a listen server.
3. Note the displayed IP address (e.g., 192.168.x.x).
4. Share the IP with other players on the same LAN.
5. Wait for players to join, then select *Start Game* to begin the match.

### Joining a Multiplayer Game
1. Select *Multiplayer* from the main menu.
2. Choose *Join Game*.
3. Enter the host's IP address provided by the host.
4. Press *Join* to connect to the game session.
5. Wait for the host to start the match.

### Playing the Game
- **Controls** (Xbox-style controller):
  - **Left Analog Stick**: Move character (forward, backward, left, right)
  - **Right Analog Stick**: Look around (rotate camera)
  - **A Button**: Jump
  - **B Button**: Crouch
  - **Right Bumper**: Aim down sights
  - **Right Trigger**: Shoot
  - **X Button**: Reload
  - **Start Button**: Open pause menu
- **Gameplay**:
  - Engage in a 5-minute match, scoring points by eliminating opponents.
  - Monitor health and ammo via the HUD.
  - Respawn after 5 seconds if eliminated.
- **Pause Menu**: Access during gameplay to resume or return to the main menu.
- **Results Screen**: View final scores and the winner at match end.

### Quitting
- From the main menu, select *Quit* to exit the game.
- From the pause menu, select *Main Menu* to leave the match, then *Quit*.

## Development Setup
For developers contributing to *PulseFire*:

### Prerequisites
- **Unreal Engine**: Version 5.4, installed via Epic Games Launcher
- **IDE**: Visual Studio 2022 (Community Edition) with C++ support
- **Version Control**: Git client (e.g., Git Bash, SourceTree)
- **Hardware**: Same as player requirements, plus 16 GB RAM recommended

### Cloning the Repository
1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```
2. Navigate to the project directory:
   ```bash
   cd PulseFire
   ```
3. Open `PulseFire.uproject` in Unreal Engine 5.4.

### Directory Structure
See [Directory Structure](docs/development/directory_structure.md) for details on the project organization.

### Building the Project
1. Open the project in Unreal Engine.
2. Configure build settings for Windows (Development configuration).
3. Build the project in Visual Studio or Unreal Editor.
4. Package the game:
   - In Unreal Editor, go to *File > Package Project > Windows*.
   - Output is saved to `/Build/Windows`.

### Contributing
- Follow the [Frontline Guidelines](docs/development/frontline_guidelines.md) for coding, art, and testing standards.
- Submit pull requests with detailed commit messages.
- Update documentation for major changes.

## Documentation
All project documentation is available in the [docs](docs) directory, organized by category:
- [Planning Documents](docs/planning)
- [Requirements Documents](docs/requirements)
- [Technical Documents](docs/technical)
- [Development Guidelines](docs/development)

## Known Issues
- **Controller Sensitivity**: May require manual adjustment for optimal feel (planned for future updates).
- **Network Stability**: Rare disconnections may occur; reconnect by rejoining the session.
- **Audio**: Placeholder sound effects lack variety (to be enhanced with funding).

## Roadmap
- **Short-Term**: Refine controller sensitivity, add sprint mechanic, improve audio based on playtest feedback.
- **Long-Term** (Post-Funding):
  - Transition to dedicated servers for larger player counts.
  - Add single-player campaign and diverse multiplayer modes.
  - Enhance visuals, audio, and UI for a polished experience.

## License
*PulseFire* is a proprietary prototype developed for evaluation purposes. Distribution and modification are restricted to authorized team members and stakeholders. Contact the project lead for access or inquiries.

## Contact
For feedback, bug reports, or inquiries:
- **Email**: [Insert project lead email]
- **Discord**: [Insert project Discord link]

Thank you for exploring *PulseFire*! We welcome your input to help shape this tactical FPS into a full-fledged game.