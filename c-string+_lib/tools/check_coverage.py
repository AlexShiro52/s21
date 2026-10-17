#!/usr/bin/env python3
"""Check executable-line coverage separately for every library function."""

import argparse
import html
from pathlib import Path
import re
import sys


def read_lcov(path):
    records = {}
    current = None
    for line in path.read_text().splitlines():
        if line.startswith("SF:"):
            current = Path(line[3:]).resolve()
            records.setdefault(current, {})
        elif line.startswith("DA:") and current is not None:
            number, count, *_ = line[3:].split(",")
            records[current][int(number)] = int(count)
        elif line == "end_of_record":
            current = None
    return records


def function_ranges(source):
    # Remove comments and literals while preserving line numbers and braces.
    tokens = r'/\*.*?\*/|//[^\n]*|"(?:\\.|[^"\\])*"|\'(?:\\.|[^\'\\])*\''
    clean = re.sub(tokens, lambda match: re.sub(r"[^\n]", " ", match[0]),
                   source, flags=re.S)
    pattern = r"(?m)^[A-Za-z_][\w \t*]*\b(s21_\w+)\s*\([^;{}]*\)\s*\{"
    for match in re.finditer(pattern, clean):
        depth = 1
        end = match.end()
        while depth and end < len(clean):
            depth += (clean[end] == "{") - (clean[end] == "}")
            end += 1
        yield (match[1], clean.count("\n", 0, match.start()) + 1,
               clean.count("\n", 0, end) + 1)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("trace", type=Path)
    parser.add_argument("--minimum", type=float, default=80)
    parser.add_argument("--html", type=Path, required=True)
    args = parser.parse_args()
    records = read_lcov(args.trace)
    source_root = Path(__file__).resolve().parent.parent
    rows = []
    failures = []
    for path in sorted(source_root.glob("s21_*.c")):
        counts = records.get(path.resolve(), {})
        functions = list(function_ranges(path.read_text()))
        if not functions:
            failures.append(f"No function definitions found in {path.name}")
        for name, first, last in functions:
            lines = [count for number, count in counts.items()
                     if first <= number <= last]
            hit = sum(count > 0 for count in lines)
            percent = 100 * hit / len(lines) if lines else 0
            rows.append((path.name, name, hit, len(lines), percent))
            if percent < args.minimum:
                failures.append(f"{path.name}: {name}: {percent:.1f}% "
                                f"({hit}/{len(lines)} lines)")
    if not rows:
        failures.append("No library coverage was found")
    content = ["<!doctype html><html lang='en'><meta charset='utf-8'>",
               "<title>Coverage of each s21_string function</title>",
               "<style>body{font:16px system-ui;margin:2rem}"
               "table{border-collapse:collapse}td,th{padding:.5rem 1rem;"
               "border:1px solid #ddd;text-align:left}"
               ".fail{background:#ffe2e2}</style>",
               "<h1>Executable-line coverage of each library function</h1>",
               f"<p>Required minimum: {args.minimum:g}% for every function. "
               "Test sources are excluded. "
               "<a href='index.html'>LCOV report</a></p>",
               "<table><tr><th>Source</th><th>Function</th>"
               "<th>Covered / executable lines</th><th>Coverage</th></tr>"]
    for file, name, hit, total, percent in rows:
        css = " class='fail'" if percent < args.minimum else ""
        content.append(f"<tr{css}><td>{html.escape(file)}</td>"
                       f"<td>{html.escape(name)}</td><td>{hit} / {total}</td>"
                       f"<td>{percent:.1f}%</td></tr>")
    content.append("</table></html>")
    args.html.parent.mkdir(parents=True, exist_ok=True)
    args.html.write_text("\n".join(content))
    for failure in failures:
        print(f"FAIL: {failure}", file=sys.stderr)
    if failures:
        return 1
    print(f"PASS: all {len(rows)} library functions have at least "
          f"{args.minimum:g}% executable-line coverage.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
