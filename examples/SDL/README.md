# PT-BR
# Instalação
## WINDOWS
### Requisitos
Toolchain GPP
- Instale MSYS2 pra windows (https://www.msys2.org/).
Dentro do terminal dele, rode
`pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain` 
Adicione a pasta do UCRT nas variaveis de ambiente.

### Compilação
Na raiz do projeto, digite
`Make`
o Makefile vai compilar o projeto em um executavel main.exe

# EN-US
# Installation
## WINDOWS
### Requirements
Toolchain GPP
- Install MSYS2 for windows (https://www.msys2.org/).
Inside its terminal, run
`pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain` 
Add the UCRT folder to your system variables.

### Compilling
In the project root folder, type
`Make`
The makefile will compille the project in a executable main.exe
