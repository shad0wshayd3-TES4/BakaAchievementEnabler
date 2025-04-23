#Requires -Version 7.1

try {
    $repo = Resolve-Path $PSScriptRoot/../
    Push-Location $repo

    # test-path
    $manifests = @(
        "$repo/xmake.lua",
        "$repo/.github/TEMPLATE",
        "$repo/.github/workflows/bootstrap.yml",
        # duh
        "$($MyInvocation.MyCommand.Path)"
    )

    Write-Host "Checking files..."
    foreach ($file in $manifests) {
        if (!(Test-Path $file)) {
            throw "File not found : {$file}"
        }
    }
    Write-Host "...Ok, all files present"


    # parse repo
    $author = $($env:REPO).Split('/')[0]
    $project = $($env:REPO).Split('/')[1]
    Write-Host "AUTHOR: $author"
    Write-Host "PROJECT: $project"

    
    # update CMakeLists.txt
    Write-Host "Updating $($manifests[0])"
    $xmake = [IO.File]::ReadAllText($manifests[0])
    $xmake = $xmake -replace "@ProjectName@", $project
    $xmake = $xmake -replace "@PluginAuthor@", $author
    [IO.File]::WriteAllText($manifests[0], $xmake)
    Write-Host "...Ok"


    # remove bootstrappers
    Write-Host "Removing bootstrap files..."
    foreach ($file in $manifests[1..3]) {
        Remove-Item $file -Force -ErrorAction:SilentlyContinue -Confirm:$false | Out-Null
    }
    Write-Host "...Ok"

    Write-Output "SETUP_SUCCESS=true" >> $env:GITHUB_OUTPUT
}
catch {
    Write-Error "...Failed: $_"
    Write-Output "SETUP_SUCCESS=false" >> $env:GITHUB_OUTPUT
}
finally {
    Pop-Location
    exit
}
