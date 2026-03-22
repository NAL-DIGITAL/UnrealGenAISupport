# UEWorldsCore HTTP API Reference

Base URL: `http://localhost:8080` (or 8090, configurable via UE_PORT)

## World & Actor CRUD

### GET /api/world
Returns complete FUWWorldDocument JSON: metadata, actors[], connections[].

### POST /api/world
Replace entire world. Body: FUWWorldDocument JSON.

### GET /api/schema
Full JSON schema of all spawnable classes with their properties, types, defaults.

### GET /api/catalog
Brief class list: name, classPath, category.

### POST /api/actors
Spawn actor. Body:
```json
{
  "classPath": "/Script/UEWorldsGameplay.UWDoor",
  "displayName": "MainDoor",
  "location": [500, 0, 0],
  "rotation": [0, 90, 0],
  "scale": [1, 1, 1],
  "properties": {
    "DoorType": {"type": "string", "value": "Swing"},
    "bLocked": {"type": "bool", "value": "true"}
  },
  "tags": ["UW.State.Primitive.Active"]
}
```
Returns: `{"actorID": "Door_1", "success": true}`

### GET /api/actors/{id}
Single actor details.

### PATCH /api/actors/{id}
Update properties/transform. Body: partial FUWActorRecord.

### DELETE /api/actors/{id}
Delete actor.

### POST /api/connect
Wire two primitives. Body:
```json
{"source_id": "Switch_1", "target_id": "Door_1"}
```

### POST /api/batch
Multiple commands in one request. Body:
```json
{
  "commands": [
    {"action": "spawn", "data": {...}},
    {"action": "connect", "data": {"source_id": "...", "target_id": "..."}},
    {"action": "delete", "data": {"actorID": "..."}}
  ]
}
```

### POST /api/undo / POST /api/redo
History navigation.

### GET /api/history
Command history.

## Taxonomy

### GET /api/taxonomy/classify?mesh=SM_Barrel01
Returns: recommended class, tags, confidence.

### POST /api/taxonomy/wrap
Auto-wrap mesh into tagged actor. Body: `{"mesh": "SM_Barrel01"}`

### GET /api/taxonomy/rules
List all classification rules.

### GET /api/taxonomy/mappings
Tag → property mappings.

## Game State

### GET /api/game/state
```json
{
  "time": 125.4,
  "fps": 60,
  "playerCount": 4,
  "matchState": "InProgress",
  "mapName": "TestMap"
}
```

### GET /api/game/mode
Current game mode class and configuration.

### POST /api/game/set-mode
```json
{"mode": "/Script/UEWorldsGameplay.UWDeathmatchGameMode"}
```
Takes effect on next map load.

### GET /api/game/scores
Player and team scores.

### GET /api/game/quests
Active quests and objective progress.

## Player Control

### GET /api/player/status
Location, health, movement state, active abilities.

### POST /api/player/move-to
```json
{"location": [500, 200, 0]}
```
or
```json
{"actorId": "Door_1"}
```
Blocks/polls until arrival. Timeout configurable.

### POST /api/player/look-at
Point camera at location or actor.

### POST /api/player/interact
```json
{"actorId": "Switch_1"}
```
Interact with nearest or specific actor.

### POST /api/player/use-ability
```json
{"tag": "UW.Ability.Attack"}
```

### POST /api/player/use-item
```json
{"itemId": "health_potion"}
```

### POST /api/player/attack
```json
{"targetId": "Enemy_1"}
```

## Observer Camera

### POST /api/observer/spawn
```json
{"location": [0, 0, 500]}
```
Spawn free-flying spectator.

### POST /api/observer/move-to
Fly to location or actor.

### POST /api/observer/look-at
Point camera.

### POST /api/observer/orbit
```json
{"actorId": "Door_1", "radius": 500, "speed": 1.0}
```

### GET /api/observer/screenshot
Returns base64 PNG. Optional: `?width=1920&height=1080`

### GET /api/observer/status
Position, rotation, movement state.

### DELETE /api/observer
Destroy observer pawn.

## AI Editor (via UWAIEditor plugin)

### GET /api/editor/sessions
List active AI editor sessions.

### GET /api/editor/session?session_id=X
Session details.

Note: Session creation/commands use WebSocket on port 8081, not HTTP.
