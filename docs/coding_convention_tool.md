# Guideline for running check coding convention

> [!NOTE]
> Coding convention checks are enforced in CI by the
> [SiliconLabsSoftware/devs-coding-convention-tool](https://github.com/SiliconLabsSoftware/devs-coding-convention-tool)
> GitHub Action (see `.github/workflows/00-Check-Code-Convention.yml`).
> This repository also ships a root `.pre-commit-config.yaml` for lightweight
> local checks that do not require project-local Uncrustify, clang-tidy, or
> codespell configuration files under `tools/`.

## Features

- Automatically fixes end-of-file issues.
- Removes trailing whitespace from lines.
- Identifies and suggests fixes for common spelling errors using codespell
  (ignore list is passed as hook arguments; no `tools/.codespell` config).
- Checks code for potential errors and style issues using cppcheck.
- Full Silabs formatting / identifier checks (Uncrustify, clang-tidy, and related
  rules) run in CI via `devs-coding-convention-tool`, not via local `tools/` configs.

## Project Structure

```sh
├── .pre-commit-config.yaml: Local pre-commit hooks (EOF, trailing
|                            whitespace, codespell, cppcheck).
└── .github/workflows/00-Check-Code-Convention.yml:
    CI job that runs SiliconLabsSoftware/devs-coding-convention-tool.
```

This project does **not** ship `tools/.clang-tidy`, `tools/uncrustify.cfg`, or
`tools/.codespell/.codespellrc`. Do not add pre-commit args that point at those
paths unless you also restore the corresponding files.

## Installation

### Ubuntu

Recommended operating system: WSL, Ubuntu 20.04.

Ensure Python is installed on your system. Then install pre-commit and cppcheck:

```sh
$ pip install pre-commit
$ sudo apt install cppcheck
```

Recommended versions for local hooks:

- Codespell 2.2.4
- Cppcheck 1.9

Uncrustify and clang-tidy are not required for the local `.pre-commit-config.yaml`
hooks; CI applies those checks through `devs-coding-convention-tool`.

## Quick start

### How To Run (local pre-commit)

Install pre-commit into your git hooks. pre-commit will now run on every commit:

```sh
$ pre-commit install
```

Staging files need formatting. For example:

```sh
$ git add data_format_sample.c
```

Run pre-commit hooks on a repository:

```sh
$ pre-commit run --all-files
```

### How To Run (CI)

Push a pull request (or use workflow_dispatch) so
`.github/workflows/00-Check-Code-Convention.yml` runs. That workflow calls
`SiliconLabsSoftware/devs-coding-convention-tool@master` with the repository
exclude regex and codespell ignore words.

### Exclude Folder

When using local pre-commit, you may want to skip some folders.

You can specify folders to exclude from formatting and checks by replacing the
exclude regex pattern with the folders you want to skip.

Here's how you can exclude folders `build` and `gecko_sdk_xxx` using regex
patterns in `.pre-commit-config.yaml`:

```sh
exclude: .*/build/.*|.*/gecko_sdk.*/.*
```

### Codespell

The codespell hook uses `--ignore-words-list` in `.pre-commit-config.yaml`
(currently `teh,foobar`), matching the CI workflow inputs. Add more words there
if you hit false positives. There is no local `.codespellrc` under `tools/`.

### Cppcheck

The cppcheck hook language/standard and suppressions are set in
`.pre-commit-config.yaml`. Adjust those arguments if your build defines
additional board headers or macros.

### Clang-Tidy / Uncrustify

Identifier naming and Silabs formatting rules are applied by
`devs-coding-convention-tool` in CI. They are not configured through local
`tools/.clang-tidy` or `tools/uncrustify.cfg` in this repository.

If you need a local clang-tidy compile database for other tooling:

```sh
$ make prepare
$ cd build
$ cmake --toolchain ../cmake/toolchain.cmake .. -DCHECK_CODING_CONVETION_ENABLE=ON -DBOARD_NAME=brd4187c
```

That creates `compile_commands.json` under `build/`, but it is not required by
the current `.pre-commit-config.yaml`.
