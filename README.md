# C++ Hash Table Implementation

A simple C++ implementation of a hash table that stores string data indexed by integer keys. This project uses **double hashing** for collision resolution, providing efficient insertions, deletions, and search operations.

## Features

- 🔑 **Insertions, deletions, and search operations** for managing the hash table
- 🏷️ **Double hashing** as a collision resolution strategy
- 🚀 Efficient performance for common hash table operations

## How It Works

The hash table uses an array to store values, and each string is mapped to an integer key. When two keys hash to the same index (collision), **double hashing** is used to calculate a second hash function that provides a step size (or offset) to find the next available spot in the table. This minimizes clustering and helps distribute data more evenly across the table.

### Double Hashing Process:
1. **First hash function** (`h1`): Maps the key to an initial index.
2. **Second hash function** (`h2`): Computes the step size to be used in case of a collision, reducing clustering issues.
3. **Probe sequence**: In case of a collision, the probe sequence is calculated as:  
   `new_index = (h1(key) + i * h2(key)) % table_size`,  
   where `i` is the number of collision attempts.

## Functions

- **Insert**: Adds a key-value pair to the table.
- **Delete**: Removes a key-value pair from the table.
- **Search**: Finds the value associated with a given key.

## Performance

This implementation is optimized for average-case constant time complexity **O(1)** for insert, delete, and search operations. However, in the worst case (with many collisions), the time complexity may degrade to **O(n)**.

Additionally, the project includes **performance evaluation** based on the selected hashing technique. The effectiveness of the chosen hash function and collision resolution strategy (double hashing) is analyzed to assess how well it handles different datasets and collision scenarios. The evaluation helps in understanding the trade-offs between speed and memory usage for various types of inputs.
