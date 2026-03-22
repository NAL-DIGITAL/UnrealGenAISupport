# UEWorldsCore Primitives Reference

All primitives inherit AUWPrimitive. Spawn via ue-bridge `POST /api/actors` with classPath.

## ClassPath Format
`/Script/UEWorldsGameplay.UW<Name>`

## Primitive Catalog

### Switch
- **ClassPath**: `/Script/UEWorldsGameplay.UWSwitch`
- **Props**: SwitchType(Toggle|Momentary|OneShot), Cooldown(0.5), bIsOn
- **Signal**: IN: any→Toggle | OUT: OnSwitched to connected

### Door
- **ClassPath**: `/Script/UEWorldsGameplay.UWDoor`
- **Props**: DoorType(Swing|Slide|Double|Animated), OpenTime(1.0), bAutoClose(true), OpenDuration(3.0), bLocked(false), RequiredKeyID, bConsumeKey(false), SlideOffset(0,0,200), SwingAngle(90)
- **Signal**: IN: any→Open/Close/Toggle | OUT: OnDoorStateChanged
- **States**: Closed, Opening, Open, Closing

### PressurePlate
- **ClassPath**: `/Script/UEWorldsGameplay.UWPressurePlate`
- **Props**: RequiredWeight(1.0), ActivationDelay(0.0), DeactivationDelay(0.0), RequiredActorClass, PressDepth(5.0)
- **Signal**: IN: weight-based auto | OUT: OnPressed/OnReleased

### Platform
- **ClassPath**: `/Script/UEWorldsGameplay.UWPlatform`
- **Props**: MovementMode(Loop|PingPong|OneShot|Triggered|Spline), MoveSpeed(200), Waypoints[{Location,Rotation,WaitTime,SpeedOverride}], bCarryPassengers(true), MaxPassengers(0=unlimited), bSmoothMovement(true)
- **Signal**: IN: any→StartMoving/StopMoving | OUT: OnWaypointReached, OnPassengerChanged

### FallingPlatform
- **ClassPath**: `/Script/UEWorldsGameplay.UWFallingPlatform`
- **Props**: FallDelay(1.0), FallSpeed(800), ResetDelay(3.0), bResetAfterFall(true), MaxFallDistance(2000), ShakeAmplitude(2.0)
- **Signal**: IN: overlap triggers fall | OUT: OnPlatformTriggered/OnPlatformFell/OnPlatformReset
- **States**: Stable→Shaking→Falling→Resetting

### Teleporter
- **ClassPath**: `/Script/UEWorldsGameplay.UWTeleporter`
- **Props**: LinkedTeleporter(soft ref), TeleportMode(Instant|Fade|Portal|Custom), FadeDuration(0.5), Cooldown(1.0), bTeleportOnOverlap(true), bRequireInteraction(false), bPlayersOnly(true), RequiredTags
- **Signal**: OUT: OnPreTeleport/OnPostTeleport

### BouncePad
- **ClassPath**: `/Script/UEWorldsGameplay.UWBouncePad`
- **Props**: LaunchVelocity(0,0,1500), bOverrideVelocity(false), CooldownTime(0.2), LaunchCurve(optional)
- **Signal**: OUT: OnActorLaunched

### Spawner
- **ClassPath**: `/Script/UEWorldsGameplay.UWSpawner`
- **Props**: SpawnMode(Once|Continuous|Wave|Triggered|Respawn), SpawnEntries[{ActorClass,Weight,RelativeTransform}], MaxAlive(10), TotalSpawnLimit(0=unlimited), SpawnInterval(5.0), RespawnDelay(3.0), ActorsPerWave(5), WaveDelay(10), TotalWaves(0), bScaleWaves(false), SpawnRadius(200), bSnapToGround(true)
- **Signal**: IN: any→SpawnOne/StartSpawning | OUT: OnActorSpawned/OnWaveStarted/OnWaveCleared

### Pickup
- **ClassPath**: `/Script/UEWorldsGameplay.UWPickup`
- **Props**: PickupType(Consumable|Key|Collectible|Equipment|Quest|Custom), ItemID, ItemDisplayName, Amount(1), bOneTimePerPlayer(false), bModifyMetric(false), MetricKey, MetricValue(1.0), bRespawns(true), RespawnTime(10), RotationSpeed(90), BobAmplitude(10)
- **Signal**: OUT: OnPickupCollected

### LogicGate
- **ClassPath**: `/Script/UEWorldsGameplay.UWLogicGate`
- **Props**: GateType(AND|OR|NOT|XOR|NAND|NOR), InputPrimitives[]
- **Signal**: IN: updates input states | OUT: OnOutputChanged (broadcasts bool result)

### Counter
- **ClassPath**: `/Script/UEWorldsGameplay.UWCounter`
- **Props**: TargetCount(5), bResetOnTarget(false), bCountDown(false), MinCount(0), MaxCount(999)
- **Signal**: IN: any→Increment/Decrement | OUT: OnCountChanged/OnTargetReached

### GameTimer
- **ClassPath**: `/Script/UEWorldsGameplay.UWGameTimer`
- **Props**: TimerMode(Countdown|Repeating|Stopwatch), Duration(30), bAutoStart(false), bLoop(false), TickInterval(1.0)
- **Signal**: IN: any→Start/Stop/Pause | OUT: OnTimerStarted/OnTimerFired/OnTimerStopped

### LaserEmitter
- **ClassPath**: `/Script/UEWorldsGameplay.UWLaserEmitter`
- **Props**: BeamDirection(1,0,0), MaxBeamLength(5000), BeamColor(Red), BeamWidth(2.0), DamagePerSecond(0)
- **Signal**: IN: any→SetEmitterActive | OUT: OnBeamHit/OnBeamBlocked

### LaserMirror
- **ClassPath**: `/Script/UEWorldsGameplay.UWLaserMirror`
- Reflects laser beams. Max 16 reflections per beam.

### Zone
- **ClassPath**: `/Script/UEWorldsGameplay.UWZone`
- Generic trigger volume. Used by KotH for hill zone.
- **Signal**: OUT: OnActorEnter/OnActorExit

### Checkpoint
- **ClassPath**: `/Script/UEWorldsGameplay.UWCheckpoint`
- Race checkpoint. Auto-wires to RaceGameMode.

### GoalVolume
- **ClassPath**: `/Script/UEWorldsGameplay.UWGoalVolume`
- Win condition / destination zone.

### DestructibleObject
- **ClassPath**: `/Script/UEWorldsGameplay.UWDestructibleObject`
- Damageable, destroys at threshold.

### ConveyorBelt
- **ClassPath**: `/Script/UEWorldsGameplay.UWConveyorBelt`
- Moves actors across surface continuously.

### MovableBlock
- **ClassPath**: `/Script/UEWorldsGameplay.UWMovableBlock`
- Physics-based pushable block for puzzles.

### SequenceLock
- **ClassPath**: `/Script/UEWorldsGameplay.UWSequenceLock`
- Puzzle lock requiring correct activation sequence.

### Ball
- **ClassPath**: `/Script/UEWorldsGameplay.UWBall`
- Physics ball for puzzles/sports.

### TDLane / TowerBase
- **ClassPath**: `/Script/UEWorldsGameplay.UWTDLane`, `UWTowerBase`
- Tower defense lane spawner and tower placement base.
