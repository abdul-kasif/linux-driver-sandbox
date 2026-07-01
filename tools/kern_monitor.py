#!/usr/bin/env python3

import argparse

SUMMARY_FILE = "/proc/kern_monitor/summary"
PROCESS_LIST_FILE = "/proc/kern_monitor/process_list"


def read_summary():
    try:
        with open(SUMMARY_FILE, "r") as f:
            data = f.read()
        print(data.strip())
    except PermissionError:
        print("Error: Permission denied. Try running with sudo.")
    except FileNotFoundError:
        print(f"Error: Kernel module not loaded ({SUMMARY_FILE} not found).")
    except Exception as e:
        print(f"Failed to read from monitor: {e}")


def read_process_list():
    try:
        with open(PROCESS_LIST_FILE, "r") as f:
            data = f.read()
        print(data.strip())
    except PermissionError:
        print("Error: Permission denied. Try running with sudo.")
    except FileNotFoundError:
        print(f"Error: Kernel module not loaded ({PROCESS_LIST_FILE} not found).")
    except Exception as e:
        print(f"Failed to read from monitor: {e}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Display running tasks managed by the CPU"
    )

    parser.add_argument(
        "-s",
        "--summary",
        action="store_true",
        help="Display the summary of the kern_monitor",
    )

    args = parser.parse_args()

    if args.summary:
        read_summary()
    else:
        read_process_list()

