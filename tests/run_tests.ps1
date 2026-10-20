$ErrorActionPreference = "Stop"

Write-Host "========================================"
Write-Host "        WhyNow Automated Test Suite"
Write-Host "========================================"

$SessionExecutable = "test_session.exe"
$ActivityExecutable = "test_activity.exe"

try {
    Write-Host ""
    Write-Host "[BUILD] Compiling session tests..."

    gcc -Wall -Wextra -Wpedantic -Werror `
        -Iinclude `
        tests\test_session.c `
        src\session.c `
        -o $SessionExecutable

    if ($LASTEXITCODE -ne 0) {
        throw "Session test compilation failed."
    }

    Write-Host "[PASS] Session tests compiled."

    Write-Host ""
    Write-Host "[TEST] Running session tests..."

    & ".\$SessionExecutable"

    if ($LASTEXITCODE -ne 0) {
        throw "Session tests failed."
    }

    Write-Host ""
    Write-Host "[BUILD] Compiling activity tests..."

    gcc -Wall -Wextra -Wpedantic -Werror `
        -Iinclude `
        tests\test_activity.c `
        src\activity.c `
        src\session.c `
        -o $ActivityExecutable

    if ($LASTEXITCODE -ne 0) {
        throw "Activity test compilation failed."
    }

    Write-Host "[PASS] Activity tests compiled."

    Write-Host ""
    Write-Host "[TEST] Running activity tests..."

    & ".\$ActivityExecutable"

    if ($LASTEXITCODE -ne 0) {
        throw "Activity tests failed."
    }

    Write-Host ""
    Write-Host "========================================"
    Write-Host "        ALL TEST SUITES PASSED"
    Write-Host "        Total assertions: 40"
    Write-Host "========================================"

    exit 0
}
catch {
    Write-Host ""
    Write-Host "========================================"
    Write-Host "        TEST SUITE FAILED"
    Write-Host "========================================"
    Write-Host $_

    exit 1
}
finally {
    if (Test-Path $SessionExecutable) {
        Remove-Item $SessionExecutable
    }

    if (Test-Path $ActivityExecutable) {
        Remove-Item $ActivityExecutable
    }
}