Hardware-Accelerated CHIP-8 Virtual Machine
A cycle-accurate, hardware-accelerated CHIP-8 emulator engineered in C++ with SDL2.

<img width="1100" height="683" alt="Screenshot 2026-01-26 084207" src="https://github.com/user-attachments/assets/a0ba2431-2c0e-4624-9e02-a63d306b3669" />
<img width="1171" height="611" alt="Screenshot 2026-01-26 084403" src="https://github.com/user-attachments/assets/91c1d979-3398-4864-ab85-0170d7af217a" />


This project moves beyond simple interpretation by employing a heterogeneous computing architecture. It offloads the render loop from the CPU to the GPU via SDL_Texture streaming and VRAM manipulation, 
ensuring locked 60FPS performance and eliminating the CPU bottlenecks found in traditional software-rendered emulators.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Key Technical Achievements:

Heterogeneous Rendering: Replaced the legacy software surface blitting logic with modern SDL_Renderer logic. 
The emulator streams the framebuffer to a GPU texture to enable hardware-accelerated scaling and rendering.

Decoupled Architecture: 
Implemented a non-blocking event loop that decouples Input Polling from the Fetch-Decode-Execute cycle to prevent event starvation and keep the interface responsive under load.

Timing is Cycle-Accurate: Added an accumulator that relies on fixed time steps. 
This will decouple the CPU's clock speed-instructions per frame-from the refresh rate of 60Hz display/timers and keep the execution speed consistent across hardware.


DSP Audio Synthesis: Created a custom audio callback system using SDL_AudioSpec. 
Instead of using pre-recorded assets, the emulator will generate sine wave audio data in real time using Digital Signal Processing directly into the sound buffer.
--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
System Architecture:
The emulator follows the Von Neumann architecture, where instructions and data share the same address space.
Memory Map 
0x000 - 0x1FF: Reserved for System Interpreter & Font Data (Sprites 0-F loaded here).
0x200 - 0xFFF: Program ROM / Application Memory (Where games are loaded).
Components
CPU: Implements the Fetch-Decode-Execute cycle for the 35 opcodes.
Display: 64x32 Monochrome display, scaled 20x via GPU for modern resolutions.
Stack: 16-level deep LIFO stack for subroutine management.
Timers: Two 8-bit timers (Delay & Sound) operating at 60Hz.

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Controls:
The emulator maps the original 4x4 hexadecimal keypad to the left side of a modern QWERTY keyboard
<img width="299" height="407" alt="image" src="https://github.com/user-attachments/assets/3e370545-3b94-4e2e-8d3c-b50bd96f3269" />

--

Prerequisites:
C++ Compiler (g++, clang, or MSVC) supporting C++17.
SDL2 Development Libraries.

--
Compilation
Windows (MinGW):

g++ -o chip8 main.cpp chip8.cpp cpu.cpp logger.cpp displaySDL.cpp keyboardSDL.cpp soundSDL.cpp cmdLineParser.cpp romLoader.cpp -lmingw32 -lSDL2main -lSDL2

Linux:
g++ -o chip8 main.cpp chip8.cpp cpu.cpp logger.cpp displaySDL.cpp keyboardSDL.cpp soundSDL.cpp cmdLineParser.cpp romLoader.cpp -lSDL2

--
Usage
Run the emulator via the command line, passing the ROM file path as an argument.
./chip8 --romFileName "Roms/Pong.ch8"

--

Command-Line Arguments:

-r or --romFileName : (Required) Path to the .ch8 ROM file.

-l or --logLevel : Set logging verbosity (0=NONE, 1=ERROR, 2=WARN, 3=INFO, 4=DEBUG).

-h or --help : Show help message.

<img width="1100" height="683" alt="Screenshot 2026-01-26 084207" src="https://github.com/user-attachments/assets/fbf1cdb9-6245-4b0b-91e4-c8268b694045" />

Project Structure:
cpu.cpp/h: The key interpreter, containing the Fetch-Decode-Execute cycle and opcode logic.

displaySDL.cpp/h: SDL2 D2 uses textures to implement hardware acceleration for graphics.

soundSDL.cpp/h: Implements real-time audio synthesis via DSP callbacks.

keyboardSDL.cpp/h: Responsible for input handling and key mapping.
romLoader.cpp/h: Binary file I/O for loading programs into memory at offset 0x200.
