# s21_cat

A custom implementation of the Unix `cat` command-line utility written in C.

The project was originally developed as part of the **School 21 Simple Bash Utils** assignment. The goal was to reproduce the core behavior of the standard Unix `cat` utility and implement commonly used command-line options.

## Features

The program can:

- read and print files to standard output;
- process multiple files;
- read from `stdin` when no file is provided;
- number output lines;
- number only non-empty lines;
- squeeze repeated blank lines;
- display end-of-line markers;
- display tab characters;
- display non-printing characters;
- parse both short and selected GNU-style long options.

## Supported Options

| Option | Long option | Description |
|---|---|---|
| `-b` | `--number-nonblank` | Number non-empty output lines |
| `-n` | `--number` | Number all output lines |
| `-s` | `--squeeze-blank` | Suppress repeated empty output lines |
| `-e` | — | Equivalent to `-vE` |
| `-E` | — | Display `$` at the end of each line |
| `-t` | — | Equivalent to `-vT` |
| `-T` | — | Display tab characters as `^I` |
| `-v` | — | Display non-printing characters |

Options can be combined:

```bash
./s21_cat -ns file.txt
```

## Build

Compile the program with GCC:

```bash
gcc -std=c11 -Wall -Wextra -Werror s21_cat.c -o s21_cat
```

## Usage

General syntax:

```bash
./s21_cat [OPTION] [FILE...]
```

Print a file:

```bash
./s21_cat file.txt
```

Number all lines:

```bash
./s21_cat -n file.txt
```

Number only non-empty lines:

```bash
./s21_cat -b file.txt
```

Remove repeated blank lines:

```bash
./s21_cat -s file.txt
```

Show `$` at the end of each line:

```bash
./s21_cat -E file.txt
```

Show tab characters:

```bash
./s21_cat -T file.txt
```

Process multiple files:

```bash
./s21_cat file1.txt file2.txt
```

Read from standard input:

```bash
echo "Hello, world!" | ./s21_cat
```

GNU-style long options are also supported:

```bash
./s21_cat --number file.txt
./s21_cat --number-nonblank file.txt
./s21_cat --squeeze-blank file.txt
```

## Implementation

The utility is implemented from scratch in C using standard file I/O.

The program is divided into several main parts:

- command-line option parsing;
- file handling;
- line numbering;
- blank-line processing;
- non-printing character conversion;
- formatted output.

A `flags` structure is used to store the state of the command-line options:

```c
typedef struct {
  int b;
  int n;
  int s;
  int E;
  int T;
  int v;
} flags;
```

The implementation works with files character by character using `fgetc()` and writes the processed output to `stdout`.

## Project Structure

```text
.
├── README.md
└── s21_cat.c
```

## What I Learned

This project helped me practice:

- C programming;
- working with files and streams;
- command-line argument parsing;
- structs;
- pointers;
- standard input and output;
- Unix command-line utilities;
- reproducing the behavior of an existing system utility.

## About the Project

This is an educational implementation of `cat` created to better understand how Unix command-line utilities work internally.