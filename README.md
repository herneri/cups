# CLI Utilities for Productivity Suite (cups)

A suite of simple and lightweight general-purpose productivity tools
for the command line.

These utilites can be used as seperate programs, or you can use the cups
frontend script, which requires Python, as a centralized solution.

Please note that as of now, all data is written in plain text files.
Refrain from using cups with sensitive data.

## Utilities

* todo: An indexed list of tasks
* wq: A priority queue of assignments
* sch: Weekly schedule of events
* cn: Note manager
* dc: Get definitions, synonyms, and antonyms for words using a web API

## Utility Usage

Refer to the manpages for each specific utility, which are
installed on your system during installation.

`man <UTILITY_NAME>`
Where `<UTILITY_NAME>` is the name of the utility you want to
read about.

## Build/Installation

### To compile a utility:

`make <UTILITY_NAME>`

### To compile the entire suite:

`make`

### Run this command as the root user to install the entire suite:

`make install`
