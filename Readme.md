# Wisdom Archive 🌍📜

A minimalist quote sharing platform with a C-based backend server and SQLite database

## Features
- Submit wisdom quotes with name and country
- Browse all submitted quotes in a grid layout
- Responsive navigation with mobile toggle
- Simple RESTful API endpoints
- Persistent SQLite storage
- Lightweight multi-page HTML interface

## Tech Stack
**Frontend:** Vanilla HTML/CSS/JS  
**Backend:** C11 with POSIX sockets  
**Database:** SQLite3

## Requirements
- C compiler (GCC recommended)
- SQLite3 development libraries
- POSIX-compliant OS (Linux/macOS/WSL)

## Installation
```bash
# Initialize database
sqlite3 quotes.db < quotes.sql

# Compile
gcc -o server server.c dbmanager.c -lsqlite3

# Start server
./server

```
