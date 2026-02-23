param (
    [Parameter(Mandatory = $true)]
    [string]$File1,

    [Parameter(Mandatory = $true)]
    [string]$File2
)

$ErrorActionPreference = "Stop"

if (!(Test-Path $File1))
{
    Write-Error "File not found: $File1"
}

if (!(Test-Path $File2))
{
    Write-Error "File not found: $File2"
}

$Base1 = [System.IO.Path]::GetFileNameWithoutExtension($File1)
$Base2 = [System.IO.Path]::GetFileNameWithoutExtension($File2)

Write-Host "[1/4] Compiling $File1 with your compiler → $Base1.s"
& compiler_c $File1 -a

Write-Host "[2/4] Assembling $Base1.s → $Base1.o"
& gcc -c "$Base1.s" -o "$Base1.o"

Write-Host "[3/4] Compiling $File2 with gcc → $Base2.o"
& gcc -c $File2 -o "$Base2.o"

Write-Host "[4/4] Linking → a.exe (no libc)"
& gcc "$Base1.o" "$Base2.o" -o "a.exe"

Write-Host "Done."
