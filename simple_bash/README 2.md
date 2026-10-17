# s21_grep

A custom implementation of the Unix `grep` command-line utility written in C.

The project was originally developed as part of the **School 21 Simple Bash Utils** assignment. The goal was to reproduce the core behavior of the standard Unix `grep` utility, including regular expression matching and commonly used command-line options.

## Features

The program can:

- search for patterns in files;
- process multiple files;
- read from `stdin`;
- use POSIX regular expressions;
- perform case-insensitive searches;
- invert search results;
- count matching lines;
- print names of matching files;
- display line numbers;
- suppress filenames in output;
- suppress file error messages;
- load patterns from a file;
- print only matching parts of lines;
- process multiple search patterns;
- combine command-line options.

## Supported Options

| Option | Description |
|---|---|
| `-e PATTERN` | Use PATTERN for matching |
| `-i` | Ignore case distinctions |
| `-v` | Invert the sense of matching |
| `-c` | Print only the number of matching lines |
| `-l` | Print only names of files containing matches |
| `-n` | Prefix each output line with its line number |
| `-h` | Suppress filename prefixes when processing multiple files |
| `-s` | Suppress error messages about nonexistent or unreadable files |
| `-f FILE` | Read patterns from FILE |
| `-o` | Print only the matched parts of matching lines |

Options can be combined:

```bash
./s21_grep -in "hello" file.txt
```

## Build

Compile the program with GCC:

```bash
gcc -std=c11 -Wall -Wextra -Werror s21_grep.c -o s21_grep
```

## Usage

General syntax:

```bash
./s21_grep [OPTION...] PATTERN [FILE...]
```

Search for a pattern:

```bash
./s21_grep "hello" file.txt
```

Ignore case:

```bash
./s21_grep -i "hello" file.txt
```

Show line numbers:

```bash
./s21_grep -n "hello" file.txt
```

Invert matching:

```bash
./s21_grep -v "hello" file.txt
```

Count matching lines:

```bash
./s21_grep -c "hello" file.txt
```

Print names of files containing matches:

```bash
./s21_grep -l "hello" file1.txt file2.txt
```

Use multiple patterns:

```bash
./s21_grep -e "hello" -e "world" file.txt
```

Read patterns from a file:

```bash
./s21_grep -f patterns.txt file.txt
```

Print only matching parts:

```bash
./s21_grep -o "hello" file.txt
```

Process multiple files:

```bash
./s21_grep "hello" file1.txt file2.txt
```

Read from standard input:

```bash
echo "Hello, world!" | ./s21_grep -i "hello"
```

## Implementation

The utility is implemented from scratch in C using the POSIX regular expression library provided by `<regex.h>`.

The program is divided into several main parts:

- command-line argument parsing;
- regular expression compilation;
- file and standard input processing;
- pattern matching;
- multiple-pattern handling;
- output formatting;
- processing of individual and combined command-line flags.

A `Flags` structure is used to store the state of the command-line options:

```c
typedef struct {
  int i;
  int v;
  int n;
  int h;
  int c;
  int l;
  int s;
  int f;
  int o;
} Flags;
```

Search patterns are compiled using `regcomp()` and matched against input lines using `regexec()`.

## Project Structure

```text
.
├── README.md
└── s21_grep.c
```

## What I Learned

This project helped me practice:

- C programming;
- POSIX regular expressions;
- command-line argument parsing;
- structs and pointers;
- file and stream processing;
- standard input and output;
- handling multiple files and patterns;
- reproducing the behavior of Unix command-line utilities.

## About the Project

This is an educational implementation of `grep` created to better understand regular expressions, text processing, and the internal behavior of Unix command-line utilities.