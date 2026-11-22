$ErrorActionPreference = "Stop"

Write-Host "========================================"
Write-Host "        WhyNow Automated Test Suite"
Write-Host "========================================"

$SessionExecutable = "test_session.exe"
$ActivityExecutable = "test_activity.exe"
$IntegrationExecutable = "test_integration.exe"
$InsightsExecutable = "test_insights.exe"
$StorageExecutable = "test_storage.exe"

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
    Write-Host "[BUILD] Compiling integration tests..."

    gcc -Wall -Wextra -Wpedantic -Werror `
        -Iinclude `
        tests\test_integration.c `
        src\session.c `
        src\activity.c `
        -o $IntegrationExecutable

    if ($LASTEXITCODE -ne 0) {
        throw "Integration test compilation failed."
    }

    Write-Host "[PASS] Integration tests compiled."

    Write-Host ""
    Write-Host "[TEST] Running integration tests..."
    & ".\$IntegrationExecutable"

    if ($LASTEXITCODE -ne 0) {
        throw "Integration tests failed."
    }

    Write-Host ""
    Write-Host "[BUILD] Compiling behaviour insights tests..."

    gcc -Wall -Wextra -Wpedantic -Werror `
        -Iinclude `
        tests\test_insights.c `
        src\insights.c `
        src\session.c `
        -o $InsightsExecutable

    if ($LASTEXITCODE -ne 0) {
        throw "Behaviour insights test compilation failed."
    }

    Write-Host "[PASS] Behaviour insights tests compiled."

    Write-Host ""
    Write-Host "[TEST] Running behaviour insights tests..."
    & ".\$InsightsExecutable"

    if ($LASTEXITCODE -ne 0) {
        throw "Behaviour insights tests failed."
    }

    Write-Host ""
    Write-Host "[BUILD] Compiling storage tests..."

    gcc -Wall -Wextra -Wpedantic -Werror `
        -Iinclude `
        tests\test_storage.c `
        src\storage.c `
        src\session.c `
        -o $StorageExecutable

    if ($LASTEXITCODE -ne 0) {
        throw "Storage test compilation failed."
    }

    Write-Host "[PASS] Storage tests compiled."

    Write-Host ""
    Write-Host "[TEST] Running storage tests..."
    & ".\$StorageExecutable"

    if ($LASTEXITCODE -ne 0) {
        throw "Storage tests failed."
    }

    Write-Host ""
    Write-Host "========================================"
    Write-Host "        ALL TEST SUITES PASSED"
    Write-Host "        Total assertions: 103"
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

    if (Test-Path $IntegrationExecutable) {
        Remove-Item $IntegrationExecutable
    }

    if (Test-Path $InsightsExecutable) {
        Remove-Item $InsightsExecutable
    }

    if (Test-Path $StorageExecutable) {
        Remove-Item $StorageExecutable
    }

    if (Test-Path "test_sessions_storage.dat") {
        Remove-Item "test_sessions_storage.dat"
    }
}

