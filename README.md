# Bomberman

**Author:** Kyan Van den Eynde
**Student number:** s0242566
**Repository:** https://github.com/kyanvde/bomberman

Advanced Programming project (2025-2026, University of Antwerp): an interactive game inspired by the classic
Bomberman battle mode, written in C++ using SFML.

Bomberman is a game in which the Player competes against three computer-controlled characters to be the last man
standing. They do this by using bombs to break blocks, gather power-ups and trap the other players!

## Project layout

- `core/` — the game logic library (static, no SFML dependency). Entities, world simulation, collision detection,
  and the design-pattern abstractions (Observer, Abstract Factory, Singleton) all live here.
- `game/` — the SFML-based representation layer: the window/main loop, states (menu, gameplay), and the concrete
  views/renderer/factory that bring `core`'s entities to the screen.
- `tests/` — a small, dependency-free test suite (`core_tests`) exercising `core` in isolation, run via CTest.
- `assets/` — sprites, fonts, and world layout files used by the game.

## Building

Requires CMake 3.28+ and a C++17 compiler (developed against G++ 13.2.0 / Clang 18.0.0 on Ubuntu 24.04.3, the
reference grading platform; also builds on MSVC). SFML 2.6.1 is fetched automatically via CMake's `FetchContent` —
no manual SFML installation is required.

```bash
cmake -S . -B build
cmake --build build
```

The `bomberman` executable is produced under `build/game/` (or `build/game/<Config>/` with a multi-config
generator, e.g. Visual Studio), alongside a copy of the `assets/` directory and the required SFML shared libraries.

## Running

```bash
./build/game/bomberman
```

## Testing

```bash
ctest --test-dir build --output-on-failure
```

## Continuous Integration

Every push is built (and tested) on CircleCI against an Ubuntu 24.04 image matching the reference platform; see
`.circleci/config.yml`.
