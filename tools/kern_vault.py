#!/usr/bin/env python3

import argparse

PROC_FILE = "/proc/kern_vault"


def write_vault(secret):
    try:
        with open(PROC_FILE, "w") as f:
            f.write(secret)
        print(f"Successfully locked message in vault: '{secret}'")
    except PermissionError:
        print("Error: Permission denied. Run this with sudo.")
    except FileNotFoundError:
        print(f"Error: {PROC_FILE} not found.")
    except Exception as e:
        print(f"Failed to write to vault: {e}")


def read_vault():
    try:
        with open(PROC_FILE, "r") as f:
            data = f.read()
        print(f"Vault contents: '{data}'")
    except PermissionError:
        print("Error: Permission denied. Run this with sudo.")
    except FileNotFoundError:
        print(f"Error: {PROC_FILE} not found.")
    except Exception as e:
        print(f"Failed to read from vault: {e}")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(
        description="Interact with the Kernel Secret Vault."
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-w", "--write", type=str, help="Secret message to write to the vault"
    )
    group.add_argument(
        "-r",
        "--read",
        action="store_true",
        help="Read the secret message from the vault",
    )

    args = parser.parse_args()

    if args.write:
        write_vault(args.write)
    elif args.read:
        read_vault()
