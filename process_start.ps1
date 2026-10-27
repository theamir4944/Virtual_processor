g++ .\generation_file.cpp -o generation_file.exe
if ($LASTEXITCODE -ne 0) {
    exit 1
}
Write-Output "generation_file.cpp - compiled\n"


.\generation_file.exe .\general.cpp .\vm_general.cpp
if ($LASTEXITCODE -ne 0) {
    exit 1
}
Write-Output "generation_file.exe - done\n"


g++ .\asm.cpp -o asm.exe
if ($LASTEXITCODE -ne 0) {
    exit 1
}
Write-Output "asm.cpp - compiled\n"


.\asm.exe .\fib_numbers.txt
if ($LASTEXITCODE -ne 0) {
    exit 1
}
Write-Output "asm.exe - done\n"


g++ .\spu.cpp -o spu.exe -lraylib -lopengl32 -lgdi32 -lwinmm -lm
if ($LASTEXITCODE -ne 0) {
    exit 1
}
Write-Output "spu.cpp - compiled\n"


.\spu.exe out.bin
if ($LASTEXITCODE -ne 0) {
    exit 1
}
Write-Output "spu.exe - done\n"