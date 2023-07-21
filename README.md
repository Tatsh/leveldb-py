# LevelDB Viewer and Dumper

This repository contains media to view LevelDB databases in a GUI or dump them to a SQLite database.

## Special Note

To use the either program make sure you make a copy of the levelDB directory that you want to
view/dump.

## leveldb-dump Usage

```shell
leveldb-dump LEVELDB_DIRECTORY [OUTPUT_FILENAME]
```

`LEVELDB_DIRECTORY` is the directory that the LevelDB database resides in.

`OUTPUT_FILENAME` the file name of the SQLite database. If not specified, defaults to the directory
name with `.sqlite` appended.

## leveldb-viewer

```shell
leveldb_viewer [LEVELDB_DIRECTORY]
```

Under the _File_ menu option choose _Open Database_. Navigate to the directory that the LevelDB
data resides in and click _Open_ to view the key/value pairs.

The macOS version is currently not signed so you must [allow it to run](https://support.apple.com/en-gb/guide/mac-help/mh40616/mac).

The Windows version requires [MSVC runtime libraries](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#visual-studio-2015-2017-2019-and-2022)
to be installed.
