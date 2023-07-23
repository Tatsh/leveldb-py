# LevelDB viewer and dumper

[![Build](https://github.com/Tatsh/leveldb-py/actions/workflows/cmake.yml/badge.svg)](https://github.com/Tatsh/leveldb-py/actions/workflows/cmake.yml)
[![QA](https://github.com/Tatsh/leveldb-py/actions/workflows/qa.yml/badge.svg)](https://github.com/Tatsh/leveldb-py/actions/workflows/qa.yml)

This repository contains software to view LevelDB databases in a GUI and dump them to a SQLite database.

## Notes

To use either program it is best to make a copy of the levelDB directory that you want to view or dump.

If a `LOCK` file is present in the copied directory, delete it.

Not all databases will open with this utility.

## leveldb-dump usage

```shell
leveldb-dump LEVELDB_DIRECTORY [OUTPUT_FILENAME]
```

`LEVELDB_DIRECTORY` is the directory that the LevelDB database resides in.

`OUTPUT_FILENAME` the file name of the SQLite database. If not specified, defaults to the directory
name with `.sqlite` appended.

## leveldb-viewer usage

```shell
leveldb-viewer [LEVELDB_DIRECTORY]
```

`LEVELDB_DIRECTORY` is the directory that the LevelDB database resides in.

Under the _File_ menu option choose _Open Database_. Navigate to the directory that the LevelDB
data resides in and click _Open_ to view the key/value pairs.

### Notes on native versions

The native macOS version is currently not signed so you must [allow it to run](https://support.apple.com/en-gb/guide/mac-help/mh40616/mac).

The native Windows version requires [MSVC runtime libraries](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#visual-studio-2015-2017-2019-and-2022)
to be installed.
