# build-current.ps1
# ----------------------------------------------------------------------------
# Compiles "the current opened file" no matter the language, and leaves
# ALWAYS the executable in .run/current.exe so the launch.json (F5) has only
# one fixed objective to point at
#
# It's invocated by .vscode/tasks.json passing 3 data from the opened file in VS Code:
#   powershell -File build-current.ps1  <fullRoute>  <nameWithoutExtension>  <.ext>
#
# To ADD a new language: add another case in the 'switch' bellow. The rule is 
# always the same -> leaving the executable in $target (.run/current.exe).
# ----------------------------------------------------------------------------

# 'param(...)' declares 3 arguments that the script receives (as the parameters of
# a function). Mandatory = if VS Code does not send them it fails.
param(
    [Parameter(Mandatory = $true)][string]$File,   # ${file}                  -> e.g: C:\...\problems\coin_piles.rs
    [Parameter(Mandatory = $true)][string]$Name,   # ${fileBasenameNoExtension} -> e.g: coin_piles
    [Parameter(Mandatory = $true)][string]$Ext     # ${fileExtname}           -> e.g: .rs  o  .cpp
)

# If any command fails, stop the script immediately  
$ErrorActionPreference = "Stop"

# $PSScriptRoot = folder where this script lives (the repo's root)
# Join-Path joins the route in a safe way -> "<repo>\.run"
$out = Join-Path $PSScriptRoot ".run"

# Creates a folder .run if doesn't exits. -Force avoid errors if already exists
# Out-Null discards the text that New-Item will print (to don't dirty the output).
New-Item -ItemType Directory -Force $out | Out-Null

# Fixed route of the final executable. HERE points the F5 (launch.json -> program). 
$target = Join-Path $out "current.exe"

# 'switch' compares $Ext againts every case. Is case-insensitive: ".CPP" also enters.
# Every case has to leave the executable in $target.
switch ($Ext) {

    ".rs" {
        # ---------- Case RUST ----------
        # 1) Generates Cargo.toml for the rust-analyzer to recognize all the problems/*.rs
        #    (& = "executes this script"). That way the autocomplete is never outdated
        & (Join-Path $PSScriptRoot "gen-cargo.ps1")

        # 2) Compiles JUST this file with rustc (as every .cpp is its own executable).
        #    --edition 2021 = version of the language;  -g = simbols to debug with gdb (F5).
        #    -o $target = Where to leave the .exe.   "$File" if there's spaces we put in quotes.
        Write-Host "rustc  -> $File"
        rustc --edition 2021 -g "$File" -o "$target"
    }

    ".cpp" {
        # ---------- Case C++ ----------
        # Uses the pipeline CMake/Nija. compile the target called as the file.
        Write-Host "cmake  -> target $Name"
        cmake -B build
        cmake --build build --target $Name

        # CMake leaves the .exe in build\<nombre>.exe; we copy it to the fixed place .run\current.exe 
        # for the F5 to always point at the same place. -Force = overwrites the previous
        Copy-Item (Join-Path $PSScriptRoot "build/$Name.exe") $target -Force
    }

    # ---------- template for FUTURES languages (Decomment and adjust) ----------
    # COMPILED languages (generates an .exe) fits perfectly in this modele
    #
    # ".c" {
    #     # C con el gcc de MinGW que ya tienes
    #     Write-Host "gcc  -> $File"
    #     gcc -g "$File" -o "$target"
    # }
    #
    # ".go" {
    #     Write-Host "go build -> $File"
    #     go build -o "$target" "$File"
    # }

    default {
        # If the extension is not contemplated, ABORT with a clear message instead of
        # execute secreatly the .exe the previous problem (silence bug).
        throw "Not soported extension: '$Ext'. Add a case for it in build-current.ps1."
    }
}

Write-Host "OK -> $target"
