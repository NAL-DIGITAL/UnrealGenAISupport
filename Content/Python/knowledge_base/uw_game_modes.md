# UEWorldsCore Game Modes

All modes: `/Script/UEWorldsGameplay.UW<Mode>GameMode`
Set via: `POST /api/game/set-mode {"mode": "<classPath>"}`

## Base (all modes inherit)

| Property | Default | Description |
|----------|---------|-------------|
| NumTeams | 0 (FFA) | 0=FFA, 2+=team game |
| RespawnDelay | 3.0 | Seconds before respawn |
| WarmupTime | 10.0 | Pre-match warmup |
| MinPlayersToStart | 2 | Auto-start threshold |
| bAutoStart | true | Start when min players met |

## Deathmatch
**ClassPath**: `UWDeathmatchGameMode`
**Type**: FFA with continuous respawn

| Property | Default | Description |
|----------|---------|-------------|
| ScoreLimit | 25 | Kills to win |
| TimeLimit | 600 | Max seconds |

**Required actors**: PlayerStart (×4+)
**Optional**: Pickup (health, weapons), Zone (kill zone at edges)

## Capture The Flag
**ClassPath**: `UWCaptureTheFlagGameMode`
**Type**: 2 teams, capture enemy flag

| Property | Default | Description |
|----------|---------|-------------|
| NumTeams | 2 | Always 2 |
| CaptureLimit | 3 | Captures to win |
| FlagAutoReturnTime | 30 | Seconds before dropped flag returns |

**Required actors**: FlagSpawn(×2), FlagBase(×2), PlayerStart per team
**Flag states**: AtBase → Carried → Dropped → (auto-return)

## King of the Hill
**ClassPath**: `UWKingOfTheHillGameMode`
**Type**: Zone control, points per second

| Property | Default | Description |
|----------|---------|-------------|
| ScoreLimitToWin | 250 | Points to win |
| PointsPerSecond | 1 | While controlling |
| BaseCaptureSpeed | 10.0 | Capture rate |
| CaptureSpeedPerPlayer | 5.0 | Bonus per extra player on hill |
| MaxCaptureProgress | 100.0 | Full capture threshold |
| CaptureDecaySpeed | 5.0 | Decay when unoccupied |
| TimeLimit | 600 | Max seconds |

**Required actors**: UWZone (the hill)
**Hill states**: Neutral → Capturing → Captured → Contested

## Elimination
**ClassPath**: `UWEliminationGameMode`
**Type**: Round-based, last standing wins

| Property | Default | Description |
|----------|---------|-------------|
| EliminationType | FFA | FFA or Team |
| RoundsToWin | 5 | Rounds for match win |
| MaxRounds | 0 | 0=no limit |
| LivesPerRound | 1 | Lives before eliminated |
| RoundTimeLimit | 120 | Seconds per round |

**Required actors**: PlayerStart (×N)
**Round states**: PreRound → Active → PostRound → MatchOver

## Race
**ClassPath**: `UWRaceGameMode`
**Type**: Checkpoint racing

| Property | Default | Description |
|----------|---------|-------------|
| LapCount | 3 | Laps to complete |
| CountdownTime | 3 | Pre-race countdown |

**Required actors**: UWCheckpoint (×N, ordered), FinishTrigger
**Track data per player**: CurrentLap, CurrentCheckpoint, LapTimes[], BestLapTime, FinishPosition

## Battle Royale
**ClassPath**: `UWBattleRoyaleGameMode`
**Type**: Shrinking zone, last alive wins

| Property | Default | Description |
|----------|---------|-------------|
| ShrinkInterval | 60 | Seconds between zone shrinks |
| DamageOutsideZone | 5.0 | DPS outside safe zone |
| MinZoneRadius | 500 | Final zone size |

**Required actors**: PlayerStart (×many), SafeZone boundary

## Survival
**ClassPath**: `UWSurvivalGameMode`
**Type**: Wave-based PvE

| Property | Default | Description |
|----------|---------|-------------|
| WaveCount | 10 | Total waves (0=infinite) |
| DifficultyScaling | 1.2 | Multiplier per wave |
| TimeBetweenWaves | 15 | Rest period |

**Required actors**: UWSpawner (Wave mode, ×N)
**Optional**: Pickup (resupply), barriers

## Tower Defense
**ClassPath**: `UWTowerDefenseGameMode`
**Type**: Lane-based tower placement

| Property | Default | Description |
|----------|---------|-------------|
| StartingGold | 500 | Initial currency |
| WaveConfig | [...] | Wave definitions |

**Required actors**: UWTDLane(×N), UWTowerBase(×N), UWSpawner, UWGoalVolume
