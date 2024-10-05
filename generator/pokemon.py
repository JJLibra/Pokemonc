from dataclasses import dataclass
from typing import List


@dataclass
class Pokemon:
    name: str
    forms: List[str]
