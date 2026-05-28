param(
    [string[]]$SkipFiles = @("sysv")
)

$ErrorActionPreference = "Continue"

$Passed = @()
$Failed = @()

$SourceFiles = Get-ChildItem -Path ./src/ -Filter "*.c" -Recurse -File

foreach ($SrcFile in $SourceFiles)
{
    & cmake --build build
    $BaseName = $SrcFile.BaseName
    $RelPath  = (Resolve-Path -Relative $SrcFile.FullName).Replace("\", "/").TrimStart("./")

    if ($SkipFiles -contains $BaseName)
    {
        Write-Host "Skipping: $RelPath"
        continue
    }

    Write-Host "=========================================="
    Write-Host "Testing: $RelPath"
    Write-Host "=========================================="

    & ./tests/build.ps1 $SrcFile.FullName
    if ($LASTEXITCODE -eq 0)
    {
        Write-Host "✓ Build succeeded: $RelPath"
        Write-Host "Running lit tests..."

        & lit -s ./tests/basic/ ./tests/complex/
        if ($LASTEXITCODE -eq 0)
        {
            Write-Host "✓ Tests passed for: $RelPath"
            $Passed += $RelPath
        } else
        {
            Write-Host "✗ Tests failed for: $RelPath"
            $Failed += $RelPath
        }
    } else
    {
        Write-Host "✗ Build failed: $RelPath"
        $Failed += $RelPath
    }

    Write-Host ""
}

Write-Host "=========================================="
Write-Host "SUMMARY"
Write-Host "=========================================="
Write-Host "Passed: $($Passed.Count)"
foreach ($f in $Passed)
{ Write-Host "  ✓ $f"
}
Write-Host ""
Write-Host "Failed: $($Failed.Count)"
foreach ($f in $Failed)
{ Write-Host "  ✗ $f"
}
