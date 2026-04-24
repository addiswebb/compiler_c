# pyright: reportGeneralTypeIssues=false, reportUndefinedVariable=false,reportAttributeAccessIssue=false ,reportMissingTypeStubs=false ,reportUnknownMemberType=false
# ruff: noqa: F821

import os
import sys

import lit.formats

config.name = "compiler_c"

config.test_format = lit.formats.ShTest()

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = config.test_source_root

config.suffixes = [".c"]
config.excludes = ["input"]

# Detect OS
is_windows = sys.platform.startswith("win")

# Determine build directory relative to project root
# Adjust if your structure differs
project_root = os.path.abspath(os.path.join(config.test_source_root, ".."))

compiler_exe = lit_config.params.get("compiler_exe")

if is_windows:
    if not compiler_exe:
        compiler_exe = os.path.join(project_root, "build", "compiler_c.exe")
    python_cmd = "python"
else:
    if not compiler_exe:
        compiler_exe = os.path.join(project_root, "build", "compiler_c")
    python_cmd = "python3"

# Substitutions
config.substitutions.append(("%cc", compiler_exe))

config.substitutions.append(
    (
        "%check_exit",
        f"{python_cmd} {os.path.join(config.test_source_root, 'check_exit.py')}",
    )
)

config.substitutions.append(
    (
        "%check_output",
        f"{python_cmd} {os.path.join(config.test_source_root, 'check_output.py')}",
    )
)
