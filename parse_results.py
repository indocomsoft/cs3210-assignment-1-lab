import sys
from collections import defaultdict


def parse(filename):
    try:
        with open(filename, "r") as f:
            results = f.readlines()
            print_by_line_by_time(
                list(filter(lambda x: x[0] == "|", results[1:])))
    except Exception as e:
        print(e)


def print_by_line_by_time(results):
    # Taking advantage of the fact that built in sort is stable

    # Sort by time
    results.sort(key=lambda r: int([x.strip()
                                    for x in filter(None, r.split("|"))][0]))

    # Sort by train id
    results.sort(key=lambda r: int([x.strip()
                                    for x in filter(None, r.split("|"))][1][1]))

    # Group by line
    results.sort(key=lambda r: [x.strip()
                                for x in filter(None, r.split("|"))][1][0])

    for r in results:
        print(r.strip())


if __name__ == "__main__":
    filename = sys.argv[1]
    parse(filename)
