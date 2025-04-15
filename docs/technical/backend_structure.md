# Backend Structure Document for PulseFire

## Overview
This document outlines the backend architecture for the PulseFire FPS prototype. As a multiplayer game focused on LAN play, PulseFire uses Unreal Engine's built-in networking capabilities with a listen server model rather than a dedicated backend infrastructure. This approach aligns with the project's scope and budget constraints while providing the necessary functionality for a small-scale multiplayer experience.

## Network Architecture

### Listen Server Model
PulseFire implements a listen server architecture where one player acts as both server and client:

- **Host Player**: Runs both server and client processes
  - Manages game state and authority
  - Handles physics simulation
  - Processes all player inputs
  - Broadcasts state updates to clients

- **Client Players**: Connect to the host
  - Send input to the server
  - Receive state updates
  - Render game state locally
  - Implement client-side prediction for responsive feel

### Network Topology
```
            ┌─────────────┐
            │             │
            │    HOST     │
            │             │
            └──────┬──────┘
                   │
         ┌─────────┼─────────┐
         │         │         │
┌────────▼─┐ ┌─────▼────┐ ┌─▼────────┐
│          │ │          │ │          │
│ CLIENT 1 │ │ CLIENT 2 │ │ CLIENT 3 │
│          │ │          │ │          │
└──────────┘ └──────────┘ └──────────┘
```

## Networking Components

### Core Classes

#### PulseFireGameMode
- Exists only on the server
- Manages match rules and scoring
- Handles player spawning and respawning
- Controls match timer and end conditions

#### PulseFireGameState
- Replicated to all clients
- Tracks match time remaining
- Maintains player scores
- Broadcasts match state changes

#### PulseFirePlayerController
- Handles player input
- Manages client-server communication
- Implements client-side prediction
- Processes server corrections

#### PulseFireCharacter
- Represents the player's physical presence
- Handles movement and collision
- Manages health and damage
- Replicates animations and effects

### Replication Strategy

#### Frequently Updated Properties
- Player positions and rotations
- Weapon states and firing
- Health values
- Ammunition counts

#### Occasionally Updated Properties
- Player scores
- Match timer
- Player connection status

#### Reliable vs. Unreliable Replication
- **Reliable**: Critical game state (health, scores, match status)
- **Unreliable**: Frequent updates (position, rotation, animations)

## Data Flow

### Input Processing
1. Client captures player input
2. Input sent to server
3. Server validates and processes input
4. Server updates game state
5. Updated state replicated to clients

### Client-Side Prediction
1. Client predicts movement locally
2. Server authoritative update received
3. Client reconciles differences
4. Smooth correction applied if needed

### Hit Detection
1. Client performs local hit detection
2. Hit information sent to server
3. Server validates hit (anti-cheat)
4. Server applies damage if valid
5. Health updates replicated to all clients

## Connection Management

### Host Setup
1. Player selects "Host Game"
2. Listen server initialized
3. Network port opened (UDP 7777)
4. Local IP displayed for sharing

### Client Connection
1. Player enters host IP
2. Connection request sent
3. Host validates connection
4. Client downloads initial game state
5. Player joins match

### Disconnection Handling
- **Client Disconnect**: Server removes player, notifies others
- **Host Disconnect**: All clients return to main menu
- **Timeout Handling**: 10-second grace period before disconnect

## Performance Considerations

### Bandwidth Optimization
- Property replication frequency tuned by importance
- Movement updates prioritized
- Relevancy-based replication (only replicate what's needed)
- Compression for network packets

### Latency Mitigation
- Input buffer of 2-3 frames
- Client-side prediction for movement
- Server reconciliation for corrections
- Interpolation for smooth transitions

### Resource Usage
- Maximum 100 KB/s bandwidth per client
- Host requires additional CPU resources (~20% overhead)
- Memory overhead for server: ~100 MB

## Security Considerations

### Server Authority
- Server is authoritative for all game-critical decisions
- Health, damage, and scoring controlled by server
- Position validation to prevent teleporting

### Limitations
- Basic security suitable for LAN play
- No encryption for local network traffic
- No authentication beyond initial connection
- Vulnerable to IP spoofing (acceptable for prototype)

## Testing Tools

### Network Profiling
- Unreal Network Profiler for bandwidth analysis
- Artificial latency simulation (0-100ms)
- Packet loss simulation (0-5%)

### Metrics Monitoring
- Bandwidth usage per client
- Server CPU and memory utilization
- Client-server round trip time
- Replication errors and corrections