# Guideline for running check coding convetion

> [!NOTE]
> This is a code formatter tool that helps ensure consistent coding style and detects common issues in source code files.
It integrates with pre-commit, allowing you to automate code formatting and checks as part of your development workflow.

- See details rule check in folder tools: [tools]().

## Features

- Automatically fixes end-of-file issues.
- Removes trailing whitespace from lines.
- Identifies and suggests fixes for common spelling errors using codespell.
- Formats code according to specified Uncrustify rules base on Silabs's coding standard.
- Checks for identifiers naming style mismatch of function, definition, struct, variable using clang-tidy.
- Checks code for potential errors and style issues using cppcheck.

## Project Structure
the following outlines the structure of the code formatter project, detailing the purpose of each file and directory.

```sh
├── tools: Contains all config file of codespell, clang-format, clang-tidy.
|    ├── codespell
|    |    ├── exclude-file.txt: Contains lines to be
|    |    |      excluded from Codespell checks.
|    |    └── cignore-words.txt: Contains words to be
|    |          ignored by Codespell to avoid false
|    |          positives.
|    |
|    ├── uncrustify.cfg: Configuration file for Uncrustify,
|    |   specifying the coding style and formatting rules.
|    └── .clang-tidy: Configuration file for clang-tidy,
|         specifying checks and options for static analysis.
└── .pre-commit-config.yaml: Configuration file for
    pre-commit, defining the hooks and their settings
    to be run before commits.
```

## Installation
###  Ubuntu
Recommended operating system: WSL, Ubuntu 20.04.

Ensure Python is installed on your system. Then, install pre-commit clang-format clang-tidy cppcheck by running:

```sh
$ pip install pre-commit
$ sudo apt install uncrustify clang-tidy cppcheck
```

Recommended version:

- Codespell 2.2.4
- Uncrustify 0.72.0
- Clang tidy 14.0.0
- Cppcheck 1.9

## Quick start
### How To Run

Before running the coding convention test, to create a database file for clang-tidy to test, run the command below

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DCHECK_CODING_CONVETION_ENABLE=ON -DBOARD_NAME=brd4187c
```
After running the above command, the file `compile_commands.json` data base will be created in the folder [build]().

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

Here's how you can exclude folders build and gecko_sdk_xxx using regex patterns in .pre-commit-config.yaml file:

```sh
exclude: .*/build/.*|.*/gecko_sdk.*/.*
```

Exclude File, Ignore Words for Codespell
When using Codespell, you may encounter false positives or want to exclude specific files or directories from being checked.

Codespell provides configuration options to handle these scenarios.

Here's an example of a codespell configuration file .codespellrc

```sh
ignore-words = ./tools/.codespell/ignore-words.txt
exclude-file = ./tools/.codespell/exclude-file.txt
check-filenames =
check-hidden =
count =
skip = .git,*.a,
```

Explanation of Configuration Options

- ignore-words : This option points to a file containing words that should be ignored by Codespell.
If you encounter a false positive, add the problematic word to ignore-words.txt.
Ensure each word is in lowercase and on a new line.
- exclude-file : This option points to a file containing lines that should be excluded from spell-checking.
If a specific line in your code is causing a false positive, copy and paste the entire line into exclude-file.txt.
- check-filenames: Set this option to true if you want Codespell to check filenames for spelling errors.
By default, this is empty (disabled).
- check-hidden: Set this option to true if you want Codespell to check hidden files for spelling errors.
By default, this is empty (disabled).
- count: Set this option to true if you want Codespell to display the number of occurrences of each misspelled word.
By default, this is empty (disabled).
- skip: This option allows you to specify files or directories that Codespell should skip.
You can list multiple entries separated by commas.
For example, .git,*.a, will skip the .git directory and all files with the .a extension.

### Uncrustify
Using automatic source code formatting follows Silabs's coding standard.
The Uncrustify configuration file is located at /tools/uncrustify.cfg. Base on the repo /projects/WMN_TOOLS/repos/scripts/browse/uncrustify

### Clang-Tidy
- Checks for identifiers naming style mismatch
- Clang-Tidy supports checking casing types (UPPER_CASE/lower_case) and prefixes of
- Function, Struct, Enum, Global Constant. You can modify the prefix in the file /tools/.clang-tidy.

For example, config function with lower_case and prefix 'sl_'

```sh
- key: readability-identifier-naming.FunctionCase
  value: lower_case
- key: readability-identifier-naming.FunctionPrefix
  value: 'sl_'
```

#### Adding Extra Arguments
You may need to provide additional paths to header files or define macros for Clang-Tidy to ensure it correctly analyzes your code.

This can be done using the --extra-arg option in the hook configuration.

To add extra arguments to the clang-tidy hook, modify the args section of the hook configuration in your .pre-commit-config.yaml file.

Here's an example of how to add extra arguments:

```sh
-   repo: https://github.com/pocc/pre-commit-hooks
    rev: v1.3.5
    hooks:
    -   id: clang-tidy
        args:
            - --config-file=./tools/.clang-tidy
            - --use-color
            - --extra-arg=-I/inc/
            - --extra-arg=-DMY_MACRO=1
```

In this example, we've added two extra arguments to Clang-Tidy:

- `I/inc/`: Specifies an include directory where Clang-Tidy will look for header files.

- `DMY_MACRO=1`: Defines a macro named MY_MACRO with the value 1 for use in the code analysis.


