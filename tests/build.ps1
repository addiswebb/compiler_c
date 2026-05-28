param(
    [Parameter(Mandatory=$true, ValueFromRemainingArguments=$true)]
    [string[]]$Args
)

$ErrorActionPreference = "Stop"

# Expand directories to .c files recursively
$Inputs = @()
foreach ($Arg in $Args)
{
    if (Test-Path $Arg -PathType Container)
    {
        $Found = Get-ChildItem -Path $Arg -Filter "*.c" -Recurse -File | Select-Object -ExpandProperty FullName
        if ($Found.Count -eq 0)
        {
            Write-Error "No .c files found in directory: $Arg"
            exit 1
        }
        $Inputs += $Found
    } else
    {
        $Inputs += $Arg
    }
}

# Files to compile with GCC instead of compiler_c
$GCC_COMPILE = @()

# Files to skip completely
$SKIP = @("win64")

$GCC_Compiled       = @()
$CompilerC_Compiled = @()
$Skipped            = @()

foreach ($InputFile in $Inputs)
{
    $Basename   = [System.IO.Path]::GetFileNameWithoutExtension($InputFile)
    # Normalize to forward slashes to match CMake's dir structure
    $Relative   = (Resolve-Path -Relative $InputFile).Replace("\", "/").TrimStart("./")
    $OutputPath = "./build/CMakeFiles/compiler_c.dir/$Relative.obj"

    # Ensure output directory exists
    $OutputDir = Split-Path $OutputPath -Parent
    New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

    if ($SKIP -contains $Basename)
    {
        $Skipped += $Basename
        continue
    }

    if ($GCC_COMPILE -contains $Basename)
    {
        & gcc -c $InputFile -I./include/ -o $OutputPath
        if ($LASTEXITCODE -ne 0)
        { Write-Error "GCC compilation failed on $InputFile"; exit 1
        }
        $GCC_Compiled += $Basename
    } else
    {
        & gcc -E -P -nostdinc -D__COMPILER_C__ -I./libc -std=c11 $InputFile -o ./tests/test.c -I./include/
        if ($LASTEXITCODE -ne 0)
        { Write-Error "GCC preprocessor failed on $InputFile"; exit 1
        }

        & compiler_c ./tests/test.c -o ./tests/$Basename.obj -c
        if ($LASTEXITCODE -ne 0)
        { Write-Error "compiler_c failed on $InputFile"; exit 1
        }

        Move-Item -Force "./tests/$Basename.obj" $OutputPath
        $CompilerC_Compiled += $Basename
    }
}

Write-Host ""
Write-Host "======== Compilation Summary ========="
Write-Host ""
Write-Host "Files compiled with GCC: $($GCC_Compiled.Count)"
Write-Host ""
Write-Host "Files skipped (platform-specific): $($Skipped.Count)"
foreach ($f in $Skipped)
{ Write-Host "  - $f"
}
Write-Host ""
Write-Host "Files compiled with compiler_c: $($CompilerC_Compiled.Count)"
foreach ($f in $CompilerC_Compiled)
{ Write-Host "  - $f"
}
Write-Host ""
Write-Host "======================================"
Write-Host ""

Push-Location "./build/CMakeFiles/compiler_c.dir/"
try
{
    & gcc `
        src/main.c.obj `
        src/compiler.c.obj `
        src/core/node.c.obj `
        src/core/util.c.obj `
        src/core/array.c.obj `
        src/core/type.c.obj `
        src/core/arena.c.obj `
        src/tokenize/tokenizer.c.obj `
        src/parse/parser.c.obj `
        src/x86/x86_gen.c.obj `
        src/x86/x86_emit.c.obj `
        src/ir/ir_gen.c.obj `
        src/ir/ir_util.c.obj `
        src/ir/ir_builder.c.obj `
        src/ir/ir_module.c.obj `
        src/analyse/sema.c.obj `
        src/analyse/analysis.c.obj `
        src/analyse/const_expr.c.obj `
        src/abi/win64.c.obj `
        src/log/logger.c.obj `
        -o ../../compiler_c.exe -lm
    if ($LASTEXITCODE -ne 0)
    { Write-Error "Linking failed"; exit 1
    }
} finally
{
    Pop-Location
}

Write-Host "Finished Linking"
