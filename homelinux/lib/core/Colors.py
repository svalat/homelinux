class Colors:
    """Define colors for shell"""

    COLOR_NORMAL  = "\033[0m"
    COLOR_RED     = "\033[31m"
    COLOR_GREEN   = "\033[32m"
    COLOR_YELLOW  = "\033[33m"
    COLOR_BLUE    = "\033[34m"
    COLOR_MAGENTA = "\033[35m"
    COLOR_CYAN    = "\033[36m"
    COLOR_WHITE   = "\033[37m"

    @staticmethod
    def colored(value,color):
        return color + value + Colors.COLOR_NORMAL

    @staticmethod
    def red(value):
        return Colors.colored(value,Colors.COLOR_RED)

    @staticmethod
    def green(value):
        return Colors.colored(value,Colors.COLOR_GREEN)

    @staticmethod
    def yellow(value):
        return Colors.colored(value,Colors.COLOR_YELLOW)

    @staticmethod
    def blue(value):
        return Colors.colored(value,Colors.COLOR_BLUE)

    @staticmethod
    def magenta(value):
        return Colors.colored(value,Colors.COLOR_MAGENTA)

    @staticmethod
    def cyan(value):
        return Colors.colored(value,Colors.COLOR_CYAN)

    @staticmethod
    def white(value):
        return Colors.colored(value,Colors.COLOR_WHITE)
