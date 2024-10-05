def get_color_escape_code(r: int, g: int, b: int, background=False) -> str:
    """
    Given rgb values give the ANSI escape sequence for printing out the
    color to the terminal
    """
    return "\033[{};2;{};{};{}m".format(48 if background else 38, r, g, b)


def get_reset_escape_code() -> str:
    """
    ANSI code for reseting colors and styles
    """
    return "\033[0m"
