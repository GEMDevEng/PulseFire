# PulseFire Directory Structure

```
/PulseFire
├── /Config
│   ├── DefaultEngine.ini
│   ├── DefaultGame.ini
│   ├── DefaultInput.ini
│   └── ProjectSettings.ini
├── /Content
│   ├── /Animations
│   │   ├── PlayerIdle.anim
│   │   ├── PlayerRun.anim
│   │   ├── PlayerJump.anim
│   │   ├── PlayerCrouch.anim
│   │   ├── WeaponFire.anim
│   │   └── WeaponReload.anim
│   ├── /Blueprints
│   │   ├── /Characters
│   │   │   ├── BP_PlayerCharacter.uasset
│   │   │   └── BP_Weapon.uasset
│   │   ├── /GameModes
│   │   │   ├── BP_GameMode.uasset
│   │   │   └── BP_GameState.uasset
│   │   ├── /HUD
│   │   │   ├── BP_HUD.uasset
│   │   │   ├── BP_MainMenu.uasset
│   │   │   ├── BP_MultiplayerMenu.uasset
│   │   │   ├── BP_PauseMenu.uasset
│   │   │   └── BP_ResultsScreen.uasset
│   │   └── /Utils
│   │       └── BP_NetworkManager.uasset
│   ├── /Maps
│   │   ├── MultiplayerMap.umap
│   │   └── MainMenu.umap
│   ├── /Materials
│   │   ├── M_PlayerSkin.uasset
│   │   ├── M_Weapon.uasset
│   │   ├── M_Wall.uasset
│   │   ├── M_Floor.uasset
│   │   └── M_Crate.uasset
│   ├── /Meshes
│   │   ├── SM_PlayerModel.uasset
│   │   ├── SM_Weapon.uasset
│   │   ├── SM_Wall.uasset
│   │   ├── SM_Floor.uasset
│   │   └── SM_Crate.uasset
│   ├── /Sounds
│   │   ├── Footstep.wav
│   │   ├── Gunshot.wav
│   │   ├── Reload.wav
│   │   └── AmbientBackground.wav
│   └── /Textures
│       ├── T_PlayerSkin.uasset
│       ├── T_Weapon.uasset
│       ├── T_Wall.uasset
│       ├── T_Floor.uasset
│       └── T_Crate.uasset
├── /Source
│   ├── /PulseFire
│   │   ├── PulseFire.Build.cs
│   │   ├── /Characters
│   │   │   ├── PlayerCharacter.h
│   │   │   ├── PlayerCharacter.cpp
│   │   │   ├── Weapon.h
│   │   │   └── Weapon.cpp
│   │   ├── /GameModes
│   │   │   ├── PulseFireGameMode.h
│   │   │   ├── PulseFireGameMode.cpp
│   │   │   ├── PulseFireGameState.h
│   │   │   └── PulseFireGameState.cpp
│   │   └── /Networking
│   │       ├── NetworkManager.h
│   │       └── NetworkManager.cpp
├── /Plugins
│   └── (Empty or third-party plugins if added later)
├── /Documentation
│   ├── SetupGuide.md
│   ├── PlaytestGuide.md
│   ├── TechnicalNotes.md
│   └── InvestorPresentation.pptx
├── /Build
│   ├── /Windows
│   │   └── PulseFire.exe
├── /.gitignore
├── /README.md
└── /PulseFire.uproject
```
