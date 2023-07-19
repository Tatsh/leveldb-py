from typing import Any, Iterator

class DB:
    def __init__(self, s: str):
        ...

    def iterator(self) -> Iterator[Any]:
        ...

    def close(self) -> None:
        ...


class Error(Exception):
    ...
