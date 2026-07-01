<h1 align="center">
  dotDUDE
<p align="center">
  <img alt="dotDUDE" src="https://github.com/user-attachments/assets/c97e6bdd-0b44-4e93-87a7-b59438c7c7d9">
</p>
</h1>

# PT-BR
### Expressão digital
Inspirado por projetos como Tamagochi, Pixel Chix e MUGEN, dotDUDE é um avatar digital que pode ser transportado para diversos ambientes criados por artistas/desenvolvedores em qualquer plataforma, desde programas de computador, projetos com microcontroladores, websites, etc.

Um arquivo .DUDE representa um caritcha, um personagem digital e analógico, criado do seu jeito, expressando sua arte e capaz de viajar contigo e habitar cenários diversos.

Para a criação de mundos e cenários, o core do projeto é feito em C++ de forma que seja fácil de utilizar como biblioteca em um projeto próprio seu, garantindo que qualquer um com um arquivo .DUDE possa utilizar.

## Criando um carinha
Um 'hominho' é composto de um arquivo data.json com traços dele, e um arquivo de animations.json com as informações sobre as animações
### Traços
Por enquanto, são o nome, a velocidade base e a velocidade de corrida do personagem. 
### Animações
Aqui, você cria uma imagem png com os frames das suas animações um do lado do outro horizontalmente. 
Salve todas as imagens dentro de uma pasta /sprites.
Depois você preenche, para cada animação, o tamanho de cada frame (altura e largura), a quantidade de frames, o nome do arquivo fonte e uma lista de até quatro TAGS que representam essa animação

As TAGS são utilizadas pelos programas que o seu hominho habitar para automaticamente selecionar a animação certa pra cada ocasião.
As TAGS disponíveis são:
| TAG | FUNÇÃO | 
| -------- | -------- | 
| IDLE | Quando seu personagem ficar parado / animação padrão | 
| WALK | Quando seu personagem se movimentar em geral |
| RUN | Quando seu personagem se movimentar em velocidade máxima |
| INTERACT | Quando seu personagem interagir com objetos em geral |
| DANCE | Quando seu personagem dançar, comemorar, curtir |
| SIT | Quando seu personagem sentar |
| SLEEP | Quando seu personagem deitar/dormir/descansar |
| HIGHLIGHT | Quando seu personagem estiver sendo selecionado/selecionando algum objeto interativo/apontando |

Você pode criar mais de uma animação para a mesma situação, o core vai selecionar aleatoriamente entre elas, criando uma diversidade e personalidade e você não precisar criar uma animação para cada cenário, na falta de animação especifica, o core vai selecionar a sua animação IDLE ou a primeira animação da lista.

### O Empacotador
Depois de criar os arquivos e imagens, coloque eles dentro de uma pasta e você pode utilizar a ferramenta de empacotamento em **tools/packager** para criar um único arquivo binário .dude, seu bonequinho pronto para habitar qualquer ambiente.
No seu terminal, rode o script passando a pasta de origem e o arquivo de saida .dude
```
python tools/packager.py ../../assets/sample ../../assets/sample.dude
```
A pasta **assets** contem um exemplo de um personagem desempacotado e a versão empacotada dele.

## Criando ambientes
Você pode utilizar os arquivos dentro da pasta **core** copiando eles pro seu projeto, esses são a arquitetura básica para interpretar e interagir com os personagens inseridos no projeto.
Depois, você precisa do driver certo para seu projeto específico. Os drivers estão dentro da pasta **drivers** e são criados em cima de uma classe base *IPlatform*. Caso algum driver específico de um sistema não tenha sido criado, você pode enviar um pedido ou tentar criar (não é complicado, eu juro) herdando da classe *IPlataform*.
### Como Contribuir (DRIVERS específicos)
- Crie um fork do projeto.
- Crie um novo driver para seu sistema, herdando da classe *IPlatform*.
- Abra um **pull request** para ser adicionado ao projeto principal.

A partir dai, você tem uma classe *World* que guarda os personagens inseridos e objetos que eles possam encontrar e interagir, é só programar a vontade.

## Arquivo .DUDE
O arquivo principal do projeto é um arquivo binário customizado, para a performance em qualquer dispositivo, seja mobile, mcu, desktop, etc.
As informações na sequencia correta em binário evitam a necessidade de bibliotecas para tratar strings e json.
As imagens png são convertidas em uma sequência de pixels RGB565, já que é o formato padrão para microcontroladores, e bibliotecas gráficas gerais suportam esse formato, evitando conversão de imagens em tempo de processamento.
Essas decisões são essenciais para um menor uso de RAM já que não é necessário processamento pesado dos dados e conversão de formatos, permitindo que mais dispositivos limitados possam rodar ambientes para os nossos personagens habitarem.

## ROADMAP
- Desenvolvimento de uma ferramenta para criar personagens.
- Driver para web (js, webassembly).
- Driver para microcontroladores (TFT e LCD mono).
- Facilitar o uso e criação de objetos utilizando sprites armazenados localmente.

# EN-US
### Digital expression
Inspired by projects like Tamagotchi, Pixel Chix, and MUGEN, dotDUDE is a digital avatar that can be transported to various environments created by artists/developers on any platform, from computer software to microcontroller projects, websites, etc.
A .DUDE file represents a little guy, a digital and analog character created your way, expressing your art, capable of traveling with you and inhabiting diverse scenarios.

To create worlds and scenarios, the core of the project is written in C++ so it's easy to use as a library in your own project, ensuring anyone with a .DUDE file can jump right in.

## Creating a character
A little dude consists of a data.json file with its traits, and an animations.json file containing animation data.
### Traits
For now, these include the character's name, base speed, and run speed.
### Animations
Here, you create a PNG image with your animation frames side-by-side horizontally.
Save all images inside a /sprites folder. 
Then, for each animation, fill in the frame size (width and height), frame count, source file name, and a list of up to four TAGS that represent this animation.

The TAGS are used by the programs your little dude inhabits to automatically select the right animation for every occasion. 
Available TAGS are:
| TAG | FUNCTION | 
| -------- | -------- | 
| IDLE | When your character stands still / default animation | 
| WALK | When your character moves around |
| RUN | When your character moves at max speed |
| INTERACT | When your character interacts with objects in general |
| DANCE | When your character dances, celebrates, or enjoys themselves |
| SIT | When your character sits down |
| SLEEP | When your character lies down/sleeps/rests |
| HIGHLIGHT | When your character is selected, is selecting an interactive object, or pointing |

You can create more than one animation for the same situation, the core will randomly select between them, adding diversity and personality. Also, you don't need to create a specific animation for every single scenario, if a specific animation is missing, the core will fall back to your IDLE animation or the first one on the list.

### The Packager
After creating your files and images, put them inside a folder, and you can use the packaging tool in **tools/packager** to create a single .dude binary file, your little guy ready to inhabit any environment.
In your terminal, run the script passing the source folder and the output .dude file:
```
python tools/packager.py ../../assets/sample ../../assets/sample.dude
```
The **assets** folder contains an example of an unpacked character and its packaged version.

## Creating enviroments
You can use the files inside the **core** folder by copying them into your project, these provide the basic architecture to parse and interact with characters inserted into the project.
Next, you need the right driver for your specific project. Drivers are located in the **drivers** folder and are built on top of an *IPlatform* base class. If a specific driver for a system hasn't been created yet, you can submit a request or try making one yourself (it's not complicated, I swear) by inheriting from the *IPlatform* class.
### How to Contribute (specific DRIVERS)
- Create a fork of the project.
- Create a new driver for your system, inheriting from the *IPlatform* class.
- Open a **pull request** to be added to the main project.

From there, you have a *World* class that holds the inserted characters and objects they can find and interact with, just code away.

## .DUDE File
The main file of the project is a custom binary file, designed for performance on any device, whether mobile, MCU, desktop, etc.
Having the information in the correct binary sequence removes the need for string and JSON parsing libraries.
PNG images are converted into a sequence of RGB565 pixels, as this is the standard format for microcontrollers, and general graphics libraries support it natively, avoiding image conversion at runtime.
These decisions are essential for lower RAM usage, as heavy data processing and format conversion aren't needed. This allows more hardware limited devices to run environments for our characters to inhabit.

## ROADMAP
- Development of a character creation tool.
- Web Driver (js, webassembly).
- Microcontroller Driver (TFT and LCD mono).
- Facilitate the use and creation of objects using locally stored sprites.

