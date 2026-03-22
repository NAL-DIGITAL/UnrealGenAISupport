# Content/Python/init_unreal.py - Using UE settings integration
import unreal
import importlib.util
import os
import sys
import subprocess
import atexit
from utils import logging as log

# Global process handle for MCP server
mcp_server_process = None

def shutdown_mcp_server():
    """Shutdown the MCP server process when Unreal Editor closes"""
    global mcp_server_process
    if mcp_server_process:
        log.log_info("Shutting down MCP server process...")
        try:
            mcp_server_process.terminate()
            mcp_server_process = None
            log.log_info("MCP server process terminated successfully")
        except Exception as e:
            log.log_error(f"Error terminating MCP server: {e}")

def start_mcp_server():
    """Start the external MCP server process"""
    global mcp_server_process
    try:
        plugin_python_path = _find_plugin_python_path()
        if not plugin_python_path:
            log.log_error("Could not find plugin Python path")
            return False

        # Get the mcp_server.py path
        mcp_server_path = os.path.join(plugin_python_path, "mcp_server.py")

        if not os.path.exists(mcp_server_path):
            log.log_error(f"MCP server script not found at: {mcp_server_path}")
            return False

        # Start the MCP server as a separate process
        python_exe = sys.executable
        log.log_info(f"Starting MCP server using Python: {python_exe}")
        log.log_info(f"MCP server script path: {mcp_server_path}")

        # Create a detached process that will continue running
        # even if Unreal crashes (we'll handle proper shutdown with atexit)
        creationflags = 0
        if sys.platform == 'win32':
            creationflags = subprocess.CREATE_NEW_PROCESS_GROUP

        mcp_server_process = subprocess.Popen(
            [python_exe, mcp_server_path],
            creationflags=creationflags,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        log.log_info(f"MCP server started with PID: {mcp_server_process.pid}")

        # Register cleanup handler to ensure process is terminated when Unreal exits
        atexit.register(shutdown_mcp_server)

        return True
    except Exception as e:
        log.log_error(f"Error starting MCP server: {e}")
        return False
def _find_plugin_python_path():
    """Find the plugin's Python directory in sys.path."""
    needle = os.path.normpath("UnrealGenAISupport/Content/Python")
    for path in sys.path:
        if needle in os.path.normpath(path):
            log.log_info(f"Found plugin Python path: {path}")
            return path
    log.log_error(f"Plugin path not found. Needle='{needle}', sys.path={sys.path}")
    return None


def _read_json_config():
    """Read auto_start from socket_server_config.json as fallback."""
    import json
    plugin_path = _find_plugin_python_path()
    if plugin_path:
        cfg = os.path.join(plugin_path, "socket_server_config.json")
        if os.path.exists(cfg):
            try:
                with open(cfg) as f:
                    return json.load(f).get("auto_start_socket_server", True)
            except Exception:
                pass
    return True


def initialize_socket_server():
    """
    Initialize the socket server if auto-start is enabled in UE settings
    """
    auto_start = True  # Default to True for MCP connectivity

    # Get settings from UE settings system
    try:
        settings_class = unreal.load_class(None, '/Script/GenerativeAISupportEditor.GenerativeAISupportSettings')
        if settings_class:
            settings = unreal.get_default_object(settings_class)

            if hasattr(settings, 'auto_start_socket_server'):
                auto_start = settings.auto_start_socket_server
                log.log_info(f"Socket server auto-start setting (UE): {auto_start}")
            else:
                # Fallback to JSON config
                auto_start = _read_json_config()
                log.log_info(f"Socket server auto-start setting (JSON fallback): {auto_start}")
        else:
            auto_start = _read_json_config()
            log.log_warning("GenerativeAISupportSettings not found, using JSON config")
    except Exception as e:
        log.log_error(f"Error reading UE settings: {e}")
        auto_start = _read_json_config()
        log.log_info(f"Falling back to JSON config: auto_start={auto_start}")

    # Auto-start if configured
    if auto_start:
        log.log_info("Auto-starting Unreal Socket Server...")

        # Start Unreal Socket Server
        try:
            plugin_python_path = _find_plugin_python_path()

            if plugin_python_path:
                server_path = os.path.join(plugin_python_path, "unreal_socket_server.py")
    
                if os.path.exists(server_path):
                    # Import and execute the server module
                    spec = importlib.util.spec_from_file_location("unreal_socket_server", server_path)
                    server_module = importlib.util.module_from_spec(spec)
                    spec.loader.exec_module(server_module)
                    log.log_info("Unreal Socket Server started successfully")

                    # Now start the MCP server
                    if start_mcp_server():
                        log.log_info("Both servers started successfully")
                    else:
                        log.log_error("Failed to start MCP server")
                else:
                    log.log_error(f"Server file not found at: {server_path}")
            else:
                log.log_error("Could not find plugin Python path")
        except Exception as e:
            log.log_error(f"Error starting socket server: {e}")
    else:
        log.log_info("Unreal Socket Server auto-start is disabled")

# Run initialization when this script is loaded
initialize_socket_server()
