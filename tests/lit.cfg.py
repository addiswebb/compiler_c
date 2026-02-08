# pyright: reportGeneralTypeIssues=false, reportUndefinedVariable=false,reportAttributeAccessIssue=false ,reportMissingTypeStubs=false ,reportUnknownMemberType=false
# ruff: noqa: F821

import os

import lit.formats

config.name = "compiler_c"

config.test_format = lit.formats.ShTest()

config.test_source_root = os.path.dirname(__file__)
config.test_exec_root = config.test_source_root

config.suffixes = [".test"]

config.substitutions.append(("%cc", r"E:\dev\compiler_c\build\compiler_c.exe"))

config.substitutions.append(
    ("%check_exit", r"python E:\dev\compiler_c\tests\check_exit.py")
)
