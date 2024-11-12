# C++ Hash Table Implementation

A simple C++ implementation of a hash table that stores string data indexed by integer keys. This project uses **open addressing** for collision resolution, providing efficient insertions, deletions, and search operations.

## Features

- 🔑 **Insertions, deletions, and search operations** for managing the hash table
- 🏷️ **Open addressing** as a collision resolution strategy
- 🚀 Efficient performance for common hash table operations

## How It Works

The hash table uses an array to store values, and each string is mapped to an integer key. When two keys hash to the same index (collision), **open addressing** is used to find the next available spot in the table. This allows for quick access and modification of data.

## Functions

- **Insert**: Adds a key-value pair to the table.
- **Delete**: Removes a key-value pair from the table.
- **Search**: Finds the value associated with a given key.

## Performance

This implementation is optimized for average-case constant time complexity **O(1)** for insert, delete, and search operations. However, in the worst case (with many collisions), the time complexity may degrade to **O(n)**.

Additionally, the project includes **performance evaluation** based on the selected hashing technique. The effectiveness of the chosen hash function and collision resolution strategy (open addressing) is analyzed to assess how well it handles different datasets and collision scenarios. The evaluation helps in understanding the trade-offs between speed and memory usage for various types of inputs.
