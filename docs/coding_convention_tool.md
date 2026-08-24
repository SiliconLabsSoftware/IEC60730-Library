# Guideline for running check coding convention

> [!NOTE]
> This is a code formatter tool that helps ensure consistent coding style and detects common issues in source code files.
It integrates with pre-commit, allowing you to automate code formatting and checks as part of your development workflow.

## Features

- Automatically fixes end-of-file issues.
- Removes trailing whitespace from lines.
- Identifies and suggests fixes for common spelling errors using codespell.
- Checks code for potential errors and style issues using cppcheck.

## Project Structure
the following outlines the structure of the code formatter project, detailing the purpose of each file and directory.

```sh
├── .pre-commit-config.yaml: Local pre-commit hooks (EOF, trailing
|                            whitespace, codespell, cppcheck).
└── .github/workflows/00-Check-Code-Convention.yml:
    CI job that runs SiliconLabsSoftware/devs-coding-convention-tool.
```

## Installation
###  Ubuntu
Recommended operating system: WSL, Ubuntu 20.04.

Ensure Python is installed on your system. Then install pre-commit and cppcheck by running:

```sh
$ pip install pre-commit
$ sudo apt install cppcheck
```

Recommended version:

- Codespell 2.2.4
- Cppcheck 1.9

## Quick start
### How To Run

Run pre-commit install to install pre-commit into your git hooks. pre-commit will now run on every commit:

```sh
$ pre-commit install
```

Staging files need formatting. For example:

```sh
$ git add data_format_sample.c
```

Run pre-commit hooks on a repository to check coding convention.

```sh
$ pre-commit run --all-files
```

### Exclude Folder
When using this tool, you may want to skip some folders.

You can specify folders to exclude from formatting and checks by replacing the exclude regex pattern with the folders you want to skip.

Here's an example of the exclude regex pattern in `.pre-commit-config.yaml`:

```sh
exclude: ^(docs|site|assets|pictures|simplicity_sdk|test/test_script|log|sample|cmake|components|lib/inc/coding_standard.h|lib/inc/silabs_license_agreement.h|lib/inc/sl_iec60730_library_documentation.h|README.md|.github)
```

Exclude File, Ignore Words for Codespell
When using Codespell, you may encounter false positives or want to exclude specific files or directories from being checked.

Codespell provides configuration options to handle these scenarios.
```sh
-   id: codespell
    args: [-w, --ignore-words-list=teh,foobar]
```

Explanation of Configuration Options

- ignore-words-list: This option defines comma-separated words that Codespell should ignore.
If you encounter a false positive, add the word to `--ignore-words-list`.
- check-filenames: Set this option to true if you want Codespell to check filenames for spelling errors.
By default, this is empty (disabled).
- check-hidden: Set this option to true if you want Codespell to check hidden files for spelling errors.
By default, this is empty (disabled).
- count: Set this option to true if you want Codespell to display the number of occurrences of each misspelled word.
By default, this is empty (disabled).
- skip: This option allows you to specify files or directories that Codespell should skip.
You can list multiple entries separated by commas.
For example, .git,*.a, will skip the .git directory and all files with the .a extension.


