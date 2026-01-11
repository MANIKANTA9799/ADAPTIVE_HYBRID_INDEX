Adaptive Hybrid Index (AHI)
Overview

The Adaptive Hybrid Index (AHI) is an experimental data structure that automatically adapts its internal representation based on how it is used. Instead of the programmer manually choosing between a hash table, balanced tree, or other data structures, AHI observes runtime behavior and switches to the most suitable structure when needed.

This project is a prototype and represents my initial step into applying advanced Object-Oriented Programming (OOP) concepts together with Data Structures and Algorithms (DSA) to solve more modern, system-level problems.

While solving many DSA problems, a recurring pattern becomes clear:

Hash tables are excellent for fast lookups.

Balanced trees are better for range queries.

No single data structure is optimal for all workloads.

However, most programs select a data structure once and never change it, even if usage patterns change over time.

The idea behind AHI is simple:

If workloads change, the data structure should be able to change too.

This project explores that idea in a practical and educational way.




Future Work and Planned Extensions

This prototype is designed to be extensible. Planned improvements include:

1. Reinforcement Learning (RL)

Replace rule-based switching with a learning-based policy

Allow the system to learn optimal decisions from experience

Adapt better to complex and changing workloads

2. Additional Data Structures

Trie (for prefix queries)

Array-based indexes (for cache-friendly scans)

Hybrid or compressed structures

3. Better Cost Models

Cache behavior awareness

Branch prediction effects

Hardware performance counters

4. System-Level Improvements

Incremental (non-blocking) migration

Thread-safe / lock-free variants

Persistent and versioned indexes

