$ErrorActionPreference = "Stop"

# ====== 环境 PATH ======
$cmakeDir = "C:\Qt\Tools\CMake_64\bin"
$mingwDir = "C:\Qt\Tools\mingw1310_64\bin"
if ($env:PATH -notlike "*$cmakeDir*") { $env:PATH = "$cmakeDir;$env:PATH" }
if ($env:PATH -notlike "*$mingwDir*") { $env:PATH = "$mingwDir;$env:PATH" }
$env:CMAKE_PREFIX_PATH = "C:\Qt\6.11.2\mingw_64"

$QtDir       = "C:\Qt\6.11.2\mingw_64"
$windeployqt = "$QtDir\bin\windeployqt.exe"
$AppName     = "GradeWordApp"
$Version     = "1.0.1"                      # ← 版本号，每次发版改这里
$Dist        = "C:\Users\pnm\Desktop\$AppName`_release"
$Zip         = "C:\Users\pnm\Desktop\${AppName}_v${Version}.zip"
$Generator   = "MinGW Makefiles"

if (-not (Test-Path $windeployqt)) { Write-Error "找不到 windeployqt：$windeployqt"; exit 1 }

# ====== 1. 编译 ======
Write-Host "==> 开始编译..." -ForegroundColor Cyan
Remove-Item build -Recurse -Force -ErrorAction SilentlyContinue
cmake -S . -B build -G $Generator -DCMAKE_BUILD_TYPE=Release
cmake --build build

# ====== 2. 准备发布目录 ======
Remove-Item $Dist -Recurse -Force -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force -Path $Dist | Out-Null

$allExe = Get-ChildItem build -Recurse -Filter "*.exe" -ErrorAction SilentlyContinue
$exe = $allExe | Where-Object { $_.Name -like "*$AppName*" } | Select-Object -First 1
if (-not $exe) { Write-Error "找不到 $AppName.exe，编译失败"; exit 1 }
Write-Host "==> 找到 exe：$($exe.FullName)" -ForegroundColor Green
Copy-Item $exe.FullName $Dist -Force

# ====== 3. 部署 Qt + MinGW 运行时 ======
Write-Host "==> 运行 windeployqt..." -ForegroundColor Cyan
& $windeployqt --release --no-translations --compiler-runtime "$Dist\$AppName.exe"

# ====== 4. 清理不需要的文件（可选，减小体积） ======
@(
  "$Dist\*.qmlc", "$Dist\*.qmlcache", "$Dist\qmltooling"
) | ForEach-Object { Remove-Item $_ -Recurse -Force -ErrorAction SilentlyContinue }

# ====== 5. 自动压缩 ======
Write-Host "==> 压缩为 $Zip ..." -ForegroundColor Cyan
Remove-Item $Zip -Force -ErrorAction SilentlyContinue
Compress-Archive -Path "$Dist\*" -DestinationPath $Zip

# ====== 6. 显示结果 ======
$ZipSize = [math]::Round((Get-Item $Zip).Length / 1MB, 2)
Write-Host "========================================" -ForegroundColor Green
Write-Host "? 打包完成" -ForegroundColor Green
Write-Host "   版本：$Version" -ForegroundColor Green
Write-Host "   压缩包：$Zip" -ForegroundColor Green
Write-Host "   大小：$ZipSize MB" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Green