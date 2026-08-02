# Compiler Construction Lab Project

### Design and Implement a Mini Programming Language Compiler using Flex and Bison

Department of Computer Science and Engineering
Metropolitan University, Bangladesh

---

Welcome to the official repository for the **Compiler Construction Lab Project**!

This repository serves as the **official project template and instruction repository** for the Compiler Construction Laboratory course.

Every project group **must fork this repository** and complete the entire project inside their own GitHub repository. This repository is provided only as a project template to help you organize your work. It does not contain the implementation of any required compiler phase. Every group is responsible for designing and implementing their own solution.



# Important Notice

This repository **does not contain the project solution**.

It only contains:

* Project instructions
* Project manual
* Repository guidelines
* GitHub workflow
* Required directory structure
* Submission instructions

The complete project specification is provided in:

> **Compiler Construction Lab Project Manual.pdf**

Read the project manual carefully before starting the implementation.



# Project Objective

The objective of this project is to design and implement a compiler front-end for a custom programming language using:

* Flex
* Bison
* C/C++
* Linux
* Make
* Git
* GitHub

The project integrates the major phases of compiler construction into one complete software system.

Your compiler must implement:

* Lexical Analysis
* Syntax Analysis
* Abstract Syntax Tree (AST)
* Symbol Table
* Semantic Analysis
* Intermediate Code Generation (Three Address Code)

Refer to the Project Manual for the complete language specification and implementation requirements.



# Repository Workflow

Every project group must strictly follow the workflow below.

```
Instructor Repository
        │
        ▼
Fork Repository
        │
        ▼
Student Group Repository
        │
        ▼
Regular Development
        │
        ▼
GitHub Repository Submission
```

Students **must not develop directly inside this repository.**



# Step 1: Fork This Repository

Click the **Fork** button located at the top-right corner of this repository.

This will create your own copy of the repository under your GitHub account.

Each group must maintain its own repository.

Do **NOT** request write access to the instructor repository.



# Step 2: Clone Your Own Repository

Clone **your fork**, not the instructor repository.

Example:

```bash
git clone https://github.com/your-username/your-repository.git
cd your-repository
```



# Step 3: Rename Your Repository

Rename your repository using the following format:

```
CC-Lab-Project-GroupName
```



# Step 4: Add Team Members

Every member of the project group must be added as a collaborator.

GitHub

Settings

↓

Collaborators

↓

Add Collaborator

Every member should contribute through their own GitHub account.



# Step 5: Repository Visibility

Keep your repository **Public** until the evaluation process has been completed.

Do not delete your repository after submission.



# Project Directory Structure

Your repository should approximately follow the following structure.

```
project-root/

├── docs/
│
├── src/
│   ├── lexer/
│   ├── parser/
│   ├── ast/
│   ├── semantic/
│   └── symbol_table/
│
├── tests/
│
├── examples/
│
├── Makefile
│
├── README.md
│
└── Project Report.pdf
```

You may organize your source code further if necessary, but the overall structure should remain clean and professional.



# Required Compiler Modules

Your compiler must include the following components.

## Lexical Analyzer

* Token recognition
* Keywords
* Identifiers
* Constants
* Operators
* Delimiters
* Comment handling
* Lexical error reporting



## Syntax Analyzer

* Complete CFG implementation
* Parsing using Bison
* Syntax error detection
* Basic error recovery



## Abstract Syntax Tree

* Build AST during parsing
* Meaningful node hierarchy
* AST visualization or printing


## Symbol Table

* Variable declarations
* Nested scopes
* Identifier lookup
* Scope management


## Semantic Analyzer

Your compiler must detect semantic errors including:

* Undeclared variables
* Redeclaration
* Scope violations
* Type mismatch
* Invalid assignments
* Invalid expressions


## Intermediate Code Generation

Generate Three Address Code (TAC).

Support:

* Arithmetic expressions
* Relational expressions
* Logical expressions
* Assignment statements
* if
* if-else
* while
* print


# Git Commit Policy

GitHub activity will be considered during evaluation.

Every student is expected to contribute throughout the semester.

Avoid uploading the entire project at the end.

Commit regularly.

Good commit messages:

```
Add lexer rules for keywords

Implement parser grammar

Create AST node hierarchy

Implement symbol table lookup

Add semantic type checking

Generate TAC for arithmetic expressions

Fix parser conflicts

Improve syntax error recovery
```

Avoid commit messages like:

```
update

new

code

final

fix

project
```

Meaningful commit history reflects professional software development practices.


# Branching (Recommended)

You may either:

* work directly on the main branch

or

* create feature branches

Example

```
feature/lexer

feature/parser

feature/semantic

feature/tac
```

Merge feature branches into `main` after testing.


# .gitignore

A `.gitignore` file is included to prevent generated and temporary files from being tracked by Git.

Examples include:

* Flex generated files
* Bison generated files
* Object files
* Executables
* IDE configuration files
* Temporary files
* Log files

Do not remove the `.gitignore` file.


# Coding Guidelines

Write clean and readable code.

Use

* meaningful variable names
* proper indentation
* modular design
* comments where appropriate

Avoid

* unnecessary global variables
* duplicated code
* magic numbers
* excessively long functions


# Test Suite

This directory contains sample test programs for the compiler.

- `valid/` contains programs that should compile successfully and produce valid output.
- `invalid/` contains programs that intentionally violate lexical, syntactic, or semantic rules. Your compiler should detect and report these errors with clear diagnostic messages where possible.

These files are provided as examples only. You are expected to develop additional test cases to thoroughly validate your compiler.


# AI Usage Policy

Artificial Intelligence tools are permitted.

Examples include:

* ChatGPT
* GitHub Copilot
* Claude
* Gemini

However,

Every student is expected to fully understand every submitted line of code.

During demonstration and viva, any group member may be asked to explain any part of the implementation.

Failure to explain the implementation may result in mark deductions regardless of whether the compiler functions correctly.


# Academic Integrity

The submitted work must be original.

Do not

* copy another group's implementation
* reuse previous semesters' projects
* submit downloaded compiler implementations

External resources may be used for learning purposes, but all references must be properly acknowledged where appropriate.


# Submission

Submit your:

* GitHub Repository Link

<!-- No ZIP archive should be submitted unless explicitly requested. -->

The submitted repository must include:

* Source Code
* README
* Project Report
* Test Programs
* Example Programs
* Build Instructions
* Execution Instructions


# Evaluation

Your project may be evaluated based on:

* Correctness
* Code quality
* Documentation
* GitHub activity
* Demonstration
* Presentation
* Group Viva

A working project alone does not guarantee full marks.

Understanding your implementation is equally important.


# Deadline

Refer to the **Compiler Construction Lab Project Manual.pdf** for the official submission deadline.

Late submissions may receive penalties according to the course policy.

---

If you have any questions regarding the project specification, contact the instructor **before** the submission deadline.

Please avoid waiting until the last moment to seek clarification.

---

This project is intended to integrate everything you have learned throughout the Compiler Construction Laboratory course.

Plan your work, collaborate effectively, commit regularly, and write clean, maintainable code.

Happy Coding!
