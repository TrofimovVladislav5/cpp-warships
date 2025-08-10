# Disclaimer

This project was created as a learning project, which means it includes all the cauveats of the learning project, such as: imperfect architecture, possible memory leaks, and lack of optimisation. Here is the justifications of not fixing those aspects:
- Rethinking the architecture will require recreating the project from scratch which will take unreasonably long time;
- I wanted to keep all those downsights to make the progress more trackable over pet-projects;
- Even though supporting project to the production-ready state will teach me, the progress will be less rapid relative to if I was creating new project for learning new concepts. For pet-project, this is the main metric.
Considering all the above, authors of this project understand most of its downsides, but it is not planned to be maintained in the future.

# WarShips game
This project is based on a laboratory work from a second course of a university. The purpose was to create the Warships game with following integrated parts:
1) Main game. It includes possibility to attack an enemy, place ships, and all the main features inside the game cycle;
2) Skills. The task was to improve game variety by implementing the skills system;
3) Saves. This part includes saving game progress, loading from save files;
4) Configs. Initially, it was about the possibility of creating files with 'commands', and the game will run all those commands in order;

## Usage 
Final user can find the list of commands in Makefile in the root of the project.

| command | description           | details                    |
|---------|-----------------------|----------------------------|
| compile   | build the project using the previous builds to optimise build time  | [read more](#game-process) |
| rebuild-debug   | rebuild the project in 'debug' mode   | [read more](#game-process) |
| rebuild-release   | rebuild the project in 'release' mode  | [read more](#game-process) |
| test   | run all tests in all libraries  | [read more](#game-process) |

### `compile` Command
Builds the project using the previous builds to optimise build time. It supports the following flags:

| flag | value | description |
|------|------|--------|
| CLEAN | 0/1 | If set to 1, previous build artifacts will be cleaned up before building the project |
| BUILD_DIR | string | Specifies the directory of the build |
| CPP_COMPILER | string | Specifies the c++ compiler that will be used in the build process. This value is passed to the -DCMAKE_CXX_COMPILER cmake flag |
| C_COMPILER | string | Specifies the c-language compiler that will be used in the build process. This value is passed to the -DCMAKE_C_COMPILER cmake flag |
| BUILD_TYPE | Debug/Release | Choose which build to use (see more details on each build further |

### `rebuild` Command
Builds the project from scratch, cleaning the previous build artifacts if present. It supports the same flags as [compile](#compile-command) command, overriding the `CLEAN` flag to 1:

| flag | value | description |
|------|------|--------|
| BUILD_DIR | string | Specifies the directory of the build |
| CPP_COMPILER | string | Specifies the c++ compiler that will be used in the build process. This value is passed to the -DCMAKE_CXX_COMPILER cmake flag |
| C_COMPILER | string | Specifies the c-language compiler that will be used in the build process. This value is passed to the -DCMAKE_C_COMPILER cmake flag |
| BUILD_TYPE | Debug/Release | Choose which build to use (see more details on each build further |

### `rebuild-debug` Command
Builds the project from scratch in Debug mode. It supports the same flags as [rebuild](#rebuild-command) command, overriding the `BUILD_TYPE` flag to 'Debug':

| flag | value | description |
|------|------|--------|
| BUILD_DIR | string | Specifies the directory of the build |
| CPP_COMPILER | string | Specifies the c++ compiler that will be used in the build process. This value is passed to the -DCMAKE_CXX_COMPILER cmake flag |
| C_COMPILER | string | Specifies the c-language compiler that will be used in the build process. This value is passed to the -DCMAKE_C_COMPILER cmake flag |

### `rebuild-release` Command
Builds the project from scratch in Release mode. It supports the same flags as [rebuild](#rebuild-command) command, overriding the `BUILD_TYPE` flag to 'Release':

| flag | value | description |
|------|------|--------|
| BUILD_DIR | string | Specifies the directory of the build |
| CPP_COMPILER | string | Specifies the c++ compiler that will be used in the build process. This value is passed to the -DCMAKE_CXX_COMPILER cmake flag |
| C_COMPILER | string | Specifies the c-language compiler that will be used in the build process. This value is passed to the -DCMAKE_C_COMPILER cmake flag |

## Build Types
Project currently supports two build modes: Release and Debug. The difference between those two is that the tests are completely omitted on the 'Release' mode, which means that the tests will not be found if you try to run `make test` after `make rebuild-release`. It is used to achieve the minimum bundle size in the release mode.

| criteria | debug | release |
|------|------|--------|
| GTests | Included and can be run with `make test` | Omitted from the build and will not be found with `make test` |
