from os import unlink
import sqlite3

import click
import plyvel

CREATE_TABLE_SQL = '''CREATE TABLE leveldb (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    key BLOB NOT NULL,
    value BLOB NOT NULL
)'''


@click.command()
@click.argument('database',
                type=click.Path(dir_okay=True, file_okay=False, resolve_path=True),
                nargs=1)
@click.argument('output_path', type=click.Path(dir_okay=False), required=False, nargs=1)
def main(database: str, output_path: str | None = None) -> None:
    if not output_path:
        output_path = f'{database}.sqlite'
    try:
        with open(output_path, 'rb'):
            pass
        unlink(output_path)
    except FileNotFoundError:
        pass
    try:
        ldb = plyvel.DB(database)
    except plyvel.Error as e:
        raise click.Abort() from e
    with sqlite3.connect(output_path) as conn:
        conn.execute(CREATE_TABLE_SQL)
        c = conn.cursor()
        try:
            for key, value in ldb.iterator():
                c.execute('INSERT INTO leveldb (key, value) VALUES (?, ?)', (key, value))
        finally:
            c.close()
            ldb.close()
