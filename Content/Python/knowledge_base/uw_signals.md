# UEWorldsCore Signal System

Primitives communicate via FUWSignalPayload. Source sends, connected targets receive.

## Signal Payload Structure
```
SignalName: FName       — e.g., "Activated", "Deactivated", "Triggered"
Value: float            — numeric data (counter value, weight)
bBoolValue: bool        — on/off
Location: FVector       — spatial context
RelatedActor: AActor*   — who triggered
Tags: FGameplayTagContainer
CustomFloats: TMap<FName, float>
CustomStrings: TMap<FName, FString>
```

## How Wiring Works

1. Each primitive has `ConnectedPrimitives: TArray<TSoftObjectPtr<AUWPrimitive>>`
2. When activated, primitive calls `SendSignal(SignalName, Payload)`
3. Payload delivered to all connected primitives' `ReceiveSignal()`
4. Max propagation depth: 64 (prevents infinite loops)

## Wiring via API

```
POST /api/connect
Body: {"source_id": "Switch_1", "target_id": "Door_1"}
```

## Common Signal Names

| Signal | Sent by | Meaning |
|--------|---------|---------|
| Activated | Switch, PressurePlate | Turned on |
| Deactivated | Switch, PressurePlate | Turned off |
| Triggered | Generic trigger | One-time event |
| TargetReached | Counter | Count hit target |
| TimerFired | GameTimer | Timer completed |
| WaveStarted | Spawner | New wave began |
| WaveCleared | Spawner | All wave enemies dead |

## Common Wiring Patterns

### Basic: Switch → Door
Player interacts with switch → door toggles open/closed.
```
Switch_1 → Door_1
```

### Gated: AND gate
Both switches must be on for door to open.
```
Switch_1 → LogicGate_1(AND)
Switch_2 → LogicGate_1(AND)
LogicGate_1 → Door_1
```

### Timed: Timer → Spawner
Waves spawn on timer intervals.
```
GameTimer_1(Repeating, 30s) → Spawner_1
```

### Counter: Collectibles → Unlock
Collect N items to open path.
```
Pickup_1 → Counter_1(target=3)
Pickup_2 → Counter_1
Pickup_3 → Counter_1
Counter_1 → Door_1  (fires on TargetReached)
```

### Laser Puzzle
Redirect laser beam through mirrors to hit target.
```
LaserEmitter_1 → (beam traces through) → LaserMirror_1 → LaserMirror_2 → PressurePlate_1
PressurePlate_1 → Door_1
```
Note: Laser beam routing is physics-based (line trace), not signal-based. PressurePlate detects beam hit.

### Sequence Lock
Switches must be activated in correct order.
```
Switch_1 → SequenceLock_1
Switch_2 → SequenceLock_1
Switch_3 → SequenceLock_1
SequenceLock_1 → Door_1 (fires when sequence correct)
```

### Wave Survival
```
GameTimer_1(Repeating) → Spawner_1(Wave)
Spawner_1.OnWaveCleared → GameTimer_1(restart)
Counter_1 ← kills
Counter_1.OnTargetReached → victory condition
```

### Platform Puzzle
Pressure plate activates moving platform.
```
PressurePlate_1 → Platform_1(Triggered)
```
Platform moves only while plate is pressed (Momentary behavior).

## Design Tips

- Use LogicGate(OR) when ANY input should trigger (multiple paths to same door)
- Use LogicGate(AND) when ALL inputs required (puzzle completion)
- Counter with bResetOnTarget=true creates repeating thresholds
- GameTimer(Countdown) + Door creates timed challenges
- Chain: Switch → Counter → LogicGate → Door for complex puzzles
- Signal propagation is instant — no delay unless Timer primitive is used
