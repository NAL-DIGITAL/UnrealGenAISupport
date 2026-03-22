# Level Design Recipes

Pre-built patterns for common level types. Combine with UEWorldsCore primitives.

## Coordinate Reference

UE units = centimeters. X=forward, Y=right, Z=up.

| Measure | UE units |
|---------|----------|
| 1 meter | 100 |
| Door height | 250 |
| Wall height | 300 |
| Standard room | 800×800 |
| Corridor width | 300 |
| Player height | 180 |

## Recipe 1: Puzzle Room (3 rooms)

### Layout
```
[Entry 0,0]──[Main Room 800,0]──[Treasure 1600,0]
   800×800       800×800             800×800
```

### Actors
```json
[
  {"class": "Cube", "loc": [0,0,-10], "scale": [8,8,0.2], "label": "Floor_Entry"},
  {"class": "Cube", "loc": [800,0,-10], "scale": [8,8,0.2], "label": "Floor_Main"},
  {"class": "Cube", "loc": [1600,0,-10], "scale": [8,8,0.2], "label": "Floor_Treasure"},

  {"class": "UWDoor", "loc": [400,0,0], "props": {"DoorType": "Slide"}},
  {"class": "UWSwitch", "loc": [600,300,100], "props": {"SwitchType": "Toggle"}},
  {"class": "UWSwitch", "loc": [600,-300,100], "props": {"SwitchType": "Toggle"}},
  {"class": "UWLogicGate", "loc": [700,0,100], "props": {"GateType": "AND"}},
  {"class": "UWDoor", "loc": [1200,0,0], "props": {"bLocked": true, "RequiredKeyID": "gold_key"}},
  {"class": "UWPickup", "loc": [800,0,50], "props": {"PickupType": "Key", "ItemID": "gold_key"}}
]
```

### Wiring
```
Switch_1 → LogicGate_1
Switch_2 → LogicGate_1
LogicGate_1 → Door_1
(Door_2 unlocked by gold_key pickup)
```

## Recipe 2: Arena (Deathmatch)

### Layout
```
Circular arena, radius 1500, with 4 pillars for cover.
Elevated ring at edges for sniping.
```

### Actors
```json
[
  {"class": "Cube", "loc": [0,0,-10], "scale": [30,30,0.2], "label": "Floor"},
  {"class": "Cube", "loc": [500,500,0], "scale": [2,2,3], "label": "Pillar_1"},
  {"class": "Cube", "loc": [-500,500,0], "scale": [2,2,3], "label": "Pillar_2"},
  {"class": "Cube", "loc": [500,-500,0], "scale": [2,2,3], "label": "Pillar_3"},
  {"class": "Cube", "loc": [-500,-500,0], "scale": [2,2,3], "label": "Pillar_4"},

  {"class": "UWPickup", "loc": [0,0,50], "props": {"PickupType": "Consumable", "MetricKey": "Health", "MetricValue": 50, "RespawnTime": 15}},
  {"class": "UWPickup", "loc": [300,0,50], "props": {"PickupType": "Equipment", "ItemID": "shotgun", "RespawnTime": 30}},
  {"class": "UWPickup", "loc": [-300,0,50], "props": {"PickupType": "Equipment", "ItemID": "rifle", "RespawnTime": 30}}
]
```

### Game Mode
```
POST /api/game/set-mode
{"mode": "/Script/UEWorldsGameplay.UWDeathmatchGameMode"}
Config: ScoreLimit=25, TimeLimit=600
```

## Recipe 3: Race Track

### Layout
```
Oval track: 4 straight segments + 4 curves
Total length ~4000 units
```

### Actors
```json
[
  {"class": "UWCheckpoint", "loc": [0,0,0], "label": "CP_Start"},
  {"class": "UWCheckpoint", "loc": [1000,0,0], "label": "CP_1"},
  {"class": "UWCheckpoint", "loc": [2000,500,0], "label": "CP_2"},
  {"class": "UWCheckpoint", "loc": [1000,1000,0], "label": "CP_3"},

  {"class": "UWBouncePad", "loc": [500,0,0], "props": {"LaunchVelocity": [500,0,800]}},
  {"class": "UWFallingPlatform", "loc": [1500,250,200], "props": {"FallDelay": 0.5, "ResetDelay": 2.0}},
  {"class": "UWConveyorBelt", "loc": [1800,700,0]},
  {"class": "UWTeleporter", "loc": [2000,1000,0], "props": {"TeleportMode": "Instant"}}
]
```

### Game Mode
```
POST /api/game/set-mode
{"mode": "/Script/UEWorldsGameplay.UWRaceGameMode"}
Config: LapCount=3
```

## Recipe 4: Platformer (Vertical)

### Layout
```
5 floors, each 500 units above previous.
Each floor = challenge to reach next.
```

### Actors per floor
```json
[
  {"class": "UWPlatform", "props": {"MovementMode": "PingPong", "MoveSpeed": 150, "Waypoints": [{"Location": [0,0,0]}, {"Location": [400,0,0]}]}},
  {"class": "UWFallingPlatform", "props": {"FallDelay": 1.0, "bResetAfterFall": true}},
  {"class": "UWBouncePad", "props": {"LaunchVelocity": [0,0,1200]}},
  {"class": "UWPickup", "props": {"PickupType": "Collectible", "ItemID": "star"}},
  {"class": "UWCounter", "props": {"TargetCount": 5}}
]
```

### Wiring
```
Pickup(star) ×5 → Counter_1(target=5)
Counter_1.OnTargetReached → Door_Exit
```

## Recipe 5: Tower Defense

### Layout
```
3 lanes from north to south, tower spots between lanes.
```

### Actors
```json
[
  {"class": "UWTDLane", "loc": [-400,0,0], "label": "Lane_Left"},
  {"class": "UWTDLane", "loc": [0,0,0], "label": "Lane_Center"},
  {"class": "UWTDLane", "loc": [400,0,0], "label": "Lane_Right"},

  {"class": "UWTowerBase", "loc": [-200,500,0]},
  {"class": "UWTowerBase", "loc": [200,500,0]},
  {"class": "UWTowerBase", "loc": [-200,1000,0]},
  {"class": "UWTowerBase", "loc": [200,1000,0]},

  {"class": "UWSpawner", "loc": [-400,0,0], "props": {"SpawnMode": "Wave", "ActorsPerWave": 5, "WaveDelay": 15, "bScaleWaves": true}},
  {"class": "UWSpawner", "loc": [0,0,0], "props": {"SpawnMode": "Wave"}},
  {"class": "UWSpawner", "loc": [400,0,0], "props": {"SpawnMode": "Wave"}},

  {"class": "UWGoalVolume", "loc": [-400,2000,0]},
  {"class": "UWGoalVolume", "loc": [0,2000,0]},
  {"class": "UWGoalVolume", "loc": [400,2000,0]}
]
```

## Recipe 6: Survival Arena

### Layout
```
Central platform with 4 spawn edges, supply drops in center.
```

### Actors
```json
[
  {"class": "UWSpawner", "loc": [1000,0,0], "props": {"SpawnMode": "Wave", "ActorsPerWave": 5, "bScaleWaves": true, "WaveScaleAmount": 2}},
  {"class": "UWSpawner", "loc": [-1000,0,0], "props": {"SpawnMode": "Wave"}},
  {"class": "UWSpawner", "loc": [0,1000,0], "props": {"SpawnMode": "Wave"}},
  {"class": "UWSpawner", "loc": [0,-1000,0], "props": {"SpawnMode": "Wave"}},

  {"class": "UWPickup", "loc": [0,0,50], "props": {"PickupType": "Consumable", "MetricKey": "Health", "MetricValue": 25, "bRespawns": true, "RespawnTime": 20}},
  {"class": "UWPickup", "loc": [200,0,50], "props": {"PickupType": "Consumable", "MetricKey": "Ammo", "MetricValue": 10, "bRespawns": true, "RespawnTime": 15}},

  {"class": "UWGameTimer", "loc": [0,0,200], "props": {"TimerMode": "Repeating", "Duration": 30, "bAutoStart": true}},
  {"class": "UWCounter", "loc": [0,0,200], "props": {"TargetCount": 50}}
]
```

### Wiring
```
GameTimer_1(30s repeating) → Spawner_1..4 (start next wave)
Kills → Counter_1 (tracks total kills for victory)
```
