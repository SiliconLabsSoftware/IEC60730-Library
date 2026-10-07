# Guideline for Running Coding Convention Checks

> [!NOTE]
> This project uses pre-commit hooks to keep formatting consistent and to catch common issues in source files. CI also runs the Silicon Labs coding-convention workflow.

## Features

- Fixes missing final newlines (end-of-file)
- Removes trailing whitespace
- Detects common spelling mistakes with codespell
- Runs static analysis with cppcheck

## Project structure

```text
├── .pre-commit-config.yaml
│     Local pre-commit hooks (EOF, trailing whitespace, codespell, cppcheck)
└── .github/workflows/00-Check-Code-Convention.yml
      CI job that runs SiliconLabsSoftware/devs-coding-convention-tool
```

## Installation (Ubuntu)

Recommended environment: WSL or Ubuntu 20.04+.

Install Python, then install pre-commit and cppcheck:

```sh
pip install pre-commit
sudo apt install cppcheck
```

Recommended versions:

- Codespell 2.2.4
- Cppcheck 1.9 (or newer from your distribution)

## Quick start

Install the Git hooks once:

```sh
pre-commit install
```

Stage files, then run checks:

```sh
git add <your_files>
pre-commit run --all-files
```

Hooks also run automatically on each `git commit` after `pre-commit install`.

### Exclude folders

To skip directories or files, update the `exclude` regex in `.pre-commit-config.yaml`. Example pattern used by this repository:

```yaml
exclude: ^(docs|site|assets|pictures|simplicity_sdk|test/test_script|log|sample|cmake|components|lib/inc/coding_standard.h|lib/inc/silabs_license_agreement.h|lib/inc/sl_iec60730_library_documentation.h|README.md|.github)
```

### Codespell options

Example hook configuration:

```yaml
- id: codespell
  args: [-w, --ignore-words-list=teh,foobar]
```

Useful options:

| Option | Purpose |
| --- | --- |
| `--ignore-words-list` | Comma-separated words Codespell should ignore |
| `check-filenames` | Also check file names for spelling errors |
| `check-hidden` | Also check hidden files |
| `count` | Show occurrence counts for each misspelling |
| `skip` | Comma-separated files/directories to skip (for example `.git,*.a`) |
