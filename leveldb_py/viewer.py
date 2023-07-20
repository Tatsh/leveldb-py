# pylint: disable=no-name-in-module
from os.path import dirname
from pathlib import Path
import sys

from PyQt5 import uic
from PyQt5.QtWidgets import QApplication, QFileDialog, QMainWindow, QTableWidgetItem
import click
import plyvel

__all__ = ('main',)


def to_str(ba: bytearray, container: list[str]) -> bool:
    try:
        container[0] = ba.decode()
        return False
    except UnicodeDecodeError:
        return True


def bin_to_hex(ba: bytearray) -> str:
    return f'0x{str(ba.hex(" "))}'


Ui_MainWindow, _ = uic.loadUiType(str(Path(
    dirname(__file__)).joinpath('mainwindow.ui')))  # type: ignore[no-untyped-call]


class MyApp(QMainWindow, Ui_MainWindow):  # type: ignore[valid-type, misc]
    def __init__(self, initial_dir: str | None = None):
        QMainWindow.__init__(self)
        super().__init__(self)
        self.setupUi(self)
        self.setUnifiedTitleAndToolBarOnMac(True)
        self.actionOpenDatabase.triggered.connect(self.open_database)
        if initial_dir:
            self.add_leveldb(initial_dir)

    def open_database(self) -> None:
        if (file_dir := QFileDialog.getExistingDirectory(self, self.tr('Select directory'))):
            self.add_leveldb(file_dir)

    def add_leveldb(self, level_db_dir: str) -> None:
        self.levelDbTableWidget.clearContents()
        self.levelDbTableWidget.setRowCount(0)
        self.levelDbTableWidget.setDisabled(True)
        ldb = plyvel.DB(level_db_dir)
        row_num = 0
        for key, value in ldb.iterator():
            self.levelDbTableWidget.insertRow(row_num)
            key_container = [key]
            key_is_binary = to_str(key, key_container)
            self.levelDbTableWidget.setItem(
                row_num, 0,
                QTableWidgetItem(key_container[0] if not key_is_binary else bin_to_hex(key)))
            value_container = [value]
            value_is_binary = to_str(value, value_container)
            self.levelDbTableWidget.setItem(
                row_num, 1,
                QTableWidgetItem(value_container[0] if not value_is_binary else bin_to_hex(key)))
            self.levelDbTableWidget.setItem(
                row_num, 2,
                QTableWidgetItem(
                    self.tr('Yes') if value_is_binary or key_is_binary else self.tr('No')))
            row_num += 1
        self.levelDbTableWidget.setDisabled(False)
        ldb.close()


@click.command()
@click.argument('database', required=False, nargs=1)
def main(database: str | None = None) -> None:
    app = QApplication(sys.argv)
    MyApp(database).show()
    if app.exec_() != 0:
        raise click.Abort()
