# Software Requirements Specification for PulseFire

## 1. Introduction

### 1.1 Purpose
This Software Requirements Specification (SRS) document outlines the requirements for the PulseFire first-person shooter prototype. It defines the functional and non-functional requirements necessary to deliver a responsive controller-based FPS experience that will serve as a proof-of-concept for potential investors.

### 1.2 Scope
PulseFire is a multiplayer FPS prototype developed in Unreal Engine 5.4, focusing on responsive controller-based gameplay. The prototype will include a single multiplayer map supporting 2-4 players over LAN, with core shooting mechanics optimized for Xbox-style controllers.

### 1.3 Definitions and Acronyms
- **FPS**: First-Person Shooter
- **LAN**: Local Area Network
- **UI**: User Interface
- **HUD**: Heads-Up Display
- **MVP**: Minimum Viable Product

## 2. Functional Requirements

### 2.1 Player Controls

#### 2.1.1 Movement System
- The system shall allow player movement using the left analog stick.
- The system shall support jumping via the A button.
- The system shall enable crouching via the B button.
- The system shall provide smooth 360-degree camera control via the right analog stick.

#### 2.1.2 Combat Controls
- The system shall implement aiming down sights via the right bumper.
- The system shall execute shooting via the right trigger.
- The system shall perform reloading via the X button.
- The system shall display appropriate animations for all combat actions.

### 2.2 Multiplayer Functionality

#### 2.2.1 Network Setup
- The system shall support hosting a game session as a listen server.
- The system shall allow other players to join via IP address input.
- The system shall support 2-4 players in a single match.
- The system shall operate over a Local Area Network (LAN).

#### 2.2.2 Match Structure
- The system shall implement 5-minute timed matches.
- The system shall track and display kill counts for all players.
- The system shall implement a 5-second respawn timer after player elimination.
- The system shall display match results at the end of the time limit.

### 2.3 User Interface

#### 2.3.1 Main Menu
- The system shall provide a main menu with Multiplayer and Quit options.
- The system shall support controller navigation throughout all menus.

#### 2.3.2 Multiplayer Menu
- The system shall provide options to host or join a game.
- The system shall display the host's IP address when hosting.
- The system shall provide an input field for entering an IP address when joining.

#### 2.3.3 In-Game HUD
- The system shall display current health status.
- The system shall show ammunition count (current/total).
- The system shall present a match timer counting down from 5:00.
- The system shall display current score (kill count) for all players.

## 3. Non-Functional Requirements

### 3.1 Performance

#### 3.1.1 Frame Rate
- The system shall maintain 60 FPS on recommended hardware (RTX 3060 equivalent).
- The system shall achieve a minimum of 30 FPS on minimum hardware (GTX 1060 equivalent).

#### 3.1.2 Input Responsiveness
- The system shall limit input latency to a maximum of 50ms from button press to on-screen action.
- The system shall prioritize responsive controls over visual fidelity when necessary.

#### 3.1.3 Network Performance
- The system shall synchronize player positions and actions with minimal perceived lag.
- The system shall handle up to 5% packet loss gracefully.
- The system shall optimize for LAN play with <50ms ping.

### 3.2 Usability

#### 3.2.1 Control Scheme
- The system shall implement an intuitive control scheme familiar to FPS players.
- The system shall provide consistent button mapping across all game states.

#### 3.2.2 User Interface
- The system shall display clear, readable text at 1080p and 1440p resolutions.
- The system shall provide visual and audio feedback for all player actions.
- The system shall implement controller-friendly menu navigation.

### 3.3 Reliability

#### 3.3.1 Stability
- The system shall operate without crashes for the duration of a match.
- The system shall handle unexpected player disconnections gracefully.

#### 3.3.2 Data Integrity
- The system shall accurately track and display player scores.
- The system shall maintain consistent hit detection and damage calculation.

## 4. System Requirements

### 4.1 Hardware Requirements

#### 4.1.1 Minimum Specifications
- Operating System: Windows 10/11 (64-bit)
- CPU: Intel Core i5 or equivalent
- GPU: NVIDIA GTX 1060 or equivalent
- RAM: 8 GB
- Storage: 2 GB free space
- Input: Xbox-style controller (wired or wireless)
- Network: Local Area Network connection

#### 4.1.2 Recommended Specifications
- CPU: Intel Core i7 or equivalent
- GPU: NVIDIA RTX 3060 or equivalent
- RAM: 16 GB
- Storage: 5 GB free space

### 4.2 Software Requirements

#### 4.2.1 Development Environment
- Unreal Engine 5.4
- Visual Studio 2022 with C++ support
- Git version control

#### 4.2.2 Runtime Environment
- DirectX 12 compatible system
- Windows 10/11 (64-bit)
- Xbox controller drivers

## 5. Constraints and Limitations

### 5.1 Budget Constraints
- Development must be completed within the $1,000 fixed budget.
- Asset usage should prioritize existing Unreal Engine assets or low-cost alternatives.

### 5.2 Scope Limitations
- The prototype will include only one multiplayer map.
- The prototype will focus on a single weapon type.
- The prototype will not include progression systems or unlockables.
- The prototype will not support online matchmaking (LAN only).
- The prototype will use basic sound effects only.