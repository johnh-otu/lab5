# SOFE3950U Operating Systems - Lab 5: Banker's Problem

## How to install / run
1. Download code as zip file on a linux system (wsl works fine), then extract/unzip
2. Open the new directory in a linux shell (bash, zsh, etc.)
3. Run the "make" command in the directory
4. Run BANKERS.exe once it's been built

## Code Organization

```bash
.
├── BANKERS.exe
├── Makefile
├── README.md
└── src
    ├── algorithms.c
    ├── algorithms.h
    ├── main.c
    ├── main.h
    ├── resources.c
    └── resources.h
```

| File              | Description                                                                              |
|-------------------|------------------------------------------------------------------------------------------|
| main.c/main.h     | Creates NCUSTOMERS threads that randomly request and release resources                   |
| algorithms.c/algorithms.h | Algorithms to check for safe states and whether resources can be safely allocated  |
| resources.c/resources.h | Uses mutex locks to allow access to vectors and matrices and allocates resources if safe to do so (see algorithms.c) |
