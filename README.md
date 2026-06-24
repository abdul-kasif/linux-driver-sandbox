# linux-driver-sandbox

A personal workspace for learning and experimenting with custom Linux kernel modules and device drivers.

## Overview

This repository contains educational kernel modules and tools designed to explore Linux driver development concepts. Each module demonstrates specific kernel programming patterns and interfaces.

## Projects

### kern_vault - Procfs-based Message Vault

A simple kernel module that provides a mechanism to store and retrieve messages via the `/proc` filesystem.

**Features:**
- Read/Write access through procfs (`/proc/kern_vault`)
- Thread-safe message buffer protected by mutex
- Read access statistics using atomic counters
- Maximum message size: 127 bytes
- Educational focus on procfs and synchronization primitives

**Limitations:**
- Stores only one message at a time
- Non-persistent (data lost on reboot or module unload)

## Project Structure

```
.
├── drivers/
│   └── kern_vault/
│       ├── kern_vault.c        # Main kernel module implementation
│       └── Makefile            # Module build configuration
├── tools/
│   └── kern_vault.py           # Userspace companion tool
├── Documentation/
│   └── userspace-api/
│       └── kern_vault.rst      # Detailed usage documentation
├── build                        # Build and module insertion script
└── README.md
```

## Quick Start

### Building the Module

1. **Clone the repository:**
   ```bash
   git clone https://github.com/abdul-kasif/linux-driver-sandbox.git
   cd linux-driver-sandbox
   ```

2. **Build the module:**
   ```bash
   cd drivers/kern_vault
   make
   ```

3. **Or use the automated build script:**
   ```bash
   ./build kern_vault
   ```

### Using kern_vault

#### Using the Python Helper Tool

```bash
# Write a message to the vault
sudo ./tools/kern_vault.py -w "My Secret Message"

# Read a message from the vault
sudo ./tools/kern_vault.py -r
```

#### Direct Procfs Access

```bash
# Write a message
echo "My Secret Message" | sudo tee /proc/kern_vault

# Read a message
cat /proc/kern_vault
```

## Installation

### Prerequisites

- Linux kernel headers: `sudo apt-get install linux-headers-$(uname -r)` (Ubuntu/Debian)
- Build tools: `build-essential`
- Python 3 (for userspace tools)

### Loading the Module

```bash
# Build the module
cd drivers/kern_vault && make

# Insert the module into the kernel
sudo insmod kern_vault.ko

# Verify the module is loaded
lsmod | grep kern_vault

# Check kernel logs
sudo dmesg | tail
```

### Removing the Module

```bash
sudo rmmod kern_vault
```

## Module Details

### kern_vault.c

The core kernel module that implements:
- **kern_vault_proc_read()**: Handles read operations with snapshot mechanism
- **kern_vault_proc_write()**: Handles write operations with input validation
- **Synchronization**: Uses mutex for thread-safe access to the shared message buffer
- **Statistics**: Atomic counter for tracking read sessions

**Key Implementation Details:**
- Uses `strscpy()` for safe string copying
- Minimum lock hold time by using temporary buffers
- Proper error handling with `-EFAULT` for userspace copy failures
- Module parameters: permission mode `0644`

### tools/kern_vault.py

A Python 3 utility providing:
- Clean command-line interface
- Error handling for:
  - Permission denied (missing `sudo`)
  - Module not loaded (`/proc/kern_vault` not found)
- `-w, --write <string>`: Write a message
- `-r, --read`: Read a message

## Documentation

Comprehensive documentation is available in:
- `Documentation/userspace-api/kern_vault.rst` - Detailed API reference and usage guide

## Development

### Testing the Module

```bash
# Check if module is properly loaded
lsmod | grep kern_vault

# Monitor kernel logs in real-time
sudo dmesg -w

# Test write operation
echo "Test Message" | sudo tee /proc/kern_vault

# Test read operation
cat /proc/kern_vault
```

### Build Configuration

The module uses the standard Linux kernel build system (Kbuild). The `Makefile` handles:
- Compilation with appropriate kernel headers
- Support for multiple processor cores (`make -j$(nproc)`)
- Clean build artifacts

## Repository Language Composition

- **C**: 62.1% - Kernel module implementation
- **Python**: 20.4% - Userspace tools
- **Shell**: 15.3% - Build and automation scripts
- **Makefile**: 2.2% - Build configuration

## Author

Abdul Kasif <abdulkasif.ra@gmail.com>

## License

GPL

## Contributing

This is a personal learning repository. Contributions and suggestions for improvement are welcome!

## References

- [Linux Kernel Module Programming](https://www.kernel.org/doc/html/latest/admin-guide/security-bugs.html)
- [Procfs Documentation](https://www.kernel.org/doc/html/latest/filesystems/proc.html)
- [Linux Device Drivers Book](https://lwn.net/Kernel/LDD3/)

---

**Note**: This project is for educational purposes. Use with caution in production environments.
