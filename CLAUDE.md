# UnrealGenAISupport — MCP Reference

Generative AI plugin for UE 5.7. Two modules: Runtime (LLM API clients) + Editor (Blueprint/Actor/Widget utils + MCP server).

## Architecture

```
Claude Code → stdio → mcp_server.py → TCP :9877 → unreal_socket_server.py (inside UE) → C++ utils
```

**Requires**: Python Editor Script Plugin enabled in UE. Socket server auto-starts via `init_unreal.py`.

## MCP Server Tools (Content/Python/mcp_server.py)

### Scene Objects
| Tool | Args | What it does |
|------|------|-------------|
| `spawn_object` | actor_class, location[3], rotation[3], scale[3], actor_label | Spawn actor. Basic shapes: "Cube", "Sphere", "Cylinder", "Cone". Others: full class path or "/Game/..." asset path |
| `modify_object` | (via edit_component_property) | Change position, rotation, scale, material of scene actors |
| `get_all_scene_objects` | — | List all actors with name, class, location |
| `create_material` | material_name, color[3] (0-1 RGB) | Create material at /Game/Materials/{name} |
| `take_editor_screenshot` | — | OS-level screenshot of primary monitor (returns Image) |

### Blueprint Creation
| Tool | Args | What it does |
|------|------|-------------|
| `create_blueprint` | blueprint_name, parent_class("Actor"), save_path("/Game/Blueprints") | Create new BP class |
| `add_component_to_blueprint` | blueprint_path, component_class, component_name | Add component (StaticMeshComponent, PointLightComponent, etc.) |
| `add_component_with_events` | blueprint_path, component_name, component_class | Add component + auto-create overlap events. Returns begin_overlap_guid, end_overlap_guid |
| `add_variable_to_blueprint` | blueprint_path, variable_name, variable_type("float"/"vector"/"boolean"), default_value, category | Add BP variable |
| `add_function_to_blueprint` | blueprint_path, function_name, inputs[{name,type}], outputs[{name,type}] | Add BP function. Returns function_id |
| `compile_blueprint` | blueprint_path | Compile BP |
| `spawn_blueprint_actor` | blueprint_path, location, rotation, scale, actor_label | Spawn BP instance in level |

### Blueprint Nodes
| Tool | Args | What it does |
|------|------|-------------|
| `add_node_to_blueprint` | blueprint_path, function_id, node_type, node_position[2], node_properties | Add node. Types: "Branch", "Sequence", "PrintString", "Delay", "Multiply", etc. Space 400x, 300y apart |
| `connect_blueprint_nodes` | blueprint_path, function_id, source_node_id, source_pin, target_node_id, target_pin | Connect two node pins |
| `connect_blueprint_nodes_bulk` | blueprint_path, function_id, connections[{source_node_id, source_pin, target_node_id, target_pin}] | Batch connect |
| `delete_node_from_blueprint` | blueprint_path, function_id, node_id | Delete node |
| `get_all_nodes_in_graph` | blueprint_path, function_id | List all nodes with GUIDs, types, positions |
| `get_node_suggestions` | node_type | Get suggestions for partial node names |
| `get_blueprint_node_guid` | blueprint_path, graph_type("EventGraph"/"FunctionGraph"), node_name/function_id | Get GUID of existing node (e.g., "BeginPlay") |

### Component Properties
| Tool | Args | What it does |
|------|------|-------------|
| `edit_component_property` | blueprint_path, component_name, property_name, value, is_scene_actor(false), actor_name | Edit any component property. Works for BP components and scene actor components |

### UI Widgets
| Tool | Args | What it does |
|------|------|-------------|
| `add_widget_to_user_widget` | user_widget_path, widget_type("TextBlock"/"Button"/"Image"/etc.), widget_name, parent_widget_name | Add widget to UMG |
| `edit_widget_property` | user_widget_path, widget_name, property_name, value | Edit widget property. Prefix "Slot." for layout props |

### Diagnostics
| Tool | Args | What it does |
|------|------|-------------|
| `read_unreal_log` | lines(50), filter(null) | Read recent UE log entries. Filter by substring (e.g., "Error", "UWLevelPackager") |

### Other
| Tool | Args | What it does |
|------|------|-------------|
| `execute_python_script` | script (string) | Execute arbitrary Python in UE. **God tool** — fallback for anything |
| `execute_unreal_command` | command (string) | Execute UE console command (stat fps, obj list, etc.) |
| `create_project_folder` | folder_path (relative to /Game) | Create content folder |
| `get_files_in_folder` | folder_path | List assets in folder |
| `create_game_mode` | game_mode_path, pawn_blueprint_path, base_class("GameModeBase") | Create game mode BP + assign pawn |
| `add_input_binding` | action_name, key | Add input action to Project Settings |
| `how_to_use` | — | Load knowledge base guide |

## Common Pitfalls

- **Pin names**: For BeginPlay use "then" for execution pin, NOT "OutputDelegate"
- **Node spacing**: Always 400x, 300y gap between nodes to prevent overlap
- **Input bindings**: First `add_input_binding`, then use "K2Node_InputAction" with `action_name` in properties
- **Materials on actors**: Use `edit_component_property` with property_name "Material" and value as path string
- **Colliders**: Use `add_component_with_events` — it auto-creates overlap event nodes

## C++ Utility Classes (Editor-only)

| Class | Exposed as | Purpose |
|-------|-----------|---------|
| FGenActorUtils | `unreal.GenActorUtils` | Spawn, transform, delete, materials |
| FGenBlueprintUtils | `unreal.GenBlueprintUtils` | BP creation, components, variables, functions, compilation |
| FGenBlueprintNodeCreator | `unreal.GenBlueprintNodeCreator` | Node creation, connection, graph introspection |
| FGenObjectProperties | `unreal.GenObjectProperties` | Property reflection and modification |
| FGenWidgetUtils | `unreal.GenWidgetUtils` | Widget hierarchy manipulation |

## Runtime Module (ships in packaged game)

LLM API clients for in-game AI:
- `UGenOAIChat` — OpenAI
- `UGenClaudeChat` — Anthropic Claude
- `UGenDSeekChat` — DeepSeek
- `UGenXAIChat` — XAI Grok

Use for AI NPC dialogue, in-game AI assistance.
