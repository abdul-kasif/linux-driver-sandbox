# linux-driver-sandbox

A personal workspace for learning and experimenting with custom Linux kernel modules and device drivers.

## Overview

This repository contains educational kernel modules and tools designed to explore Linux driver development concepts. Each module demonstrates specific kernel programming patterns and interfaces.

## Projects

### 1. [kern_vault](Documentation/userspace-api/kern_vault.rst) - Procfs-based Message Vault
A simple kernel module that provides a mechanism to store and retrieve messages via the `/proc` filesystem.
 
## Project Structure

```
.
├── drivers/                    # Main kernel module implementations
│   └── kern_vault/
│       ├── kern_vault.c        
│       └── Makefile            
├── tools/                      # Userspace companion tools
│   └── kern_vault.py           
├── Documentation/              # Detailed Usage documentation
│   └── userspace-api/
│       └── kern_vault.rst      
├── build                       # Build and module insertion script
└── README.md
```

## Author

Abdul Kasif <abdulkasif.ra@gmail.com>

## References

- [Linux Kernel Module Programming Guide](https://sysprog21.github.io/lkmpg/)
- [Linux Device Drivers Book](https://lwn.net/Kernel/LDD3/)

---

**Note**: This project is for educational purposes.
