==================================================
kern_vault - Simple procfs-based message vault
==================================================

The ``kern_vault`` driver provides a simple mechanism to store and retrieve
a single message via the procfs filesystem. It is intended as an educational
workspace driver.

Interface
=========

The driver exposes a single interface file:

``/proc/kern_vault``
    Read and write access to the message vault.

Usage via Sysfs/Procfs
======================

Writing to the vault
--------------------

You can store a message by writing directly to the procfs entry:

.. code-block:: shell

    echo "My Secret Message" > /proc/kern_vault

Reading from the vault
----------------------

You can retrieve the stored message by reading the procfs entry:

.. code-block:: shell

    cat /proc/kern_vault

Userspace Tool
==============

A helper script is provided in the ``tools/`` directory to simplify interaction
and handle common errors.

``tools/kern_vault.py``
    Automates validation, reading, and writing operations.

Command Line Arguments
----------------------

The script requires one of the following mutually exclusive arguments:

``-w, --write <string>``
    Secret message to write to the vault.
``-r, --read``
    Read the current secret message from the vault.

Examples
--------

**Writing a secret:**

.. code-block:: shell

    sudo ./tools/kern_vault.py -w "SecretMessage"

**Reading a secret:**

.. code-block:: shell

    sudo ./tools/kern_vault.py -r

Error Handling
--------------

The tool actively catches and reports the following issues:

- **Permission Denied**: Prompted if executed without sufficient privileges (e.g., missing ``sudo``).
- **File Not Found**: Occurs if the ``kern_vault`` kernel module is not loaded.

Limitations
===========

- Stores only one message at a time.
- Messages are non-persistent and lost on reboot or module unload.
- Maximum message size is 127 bytes.

