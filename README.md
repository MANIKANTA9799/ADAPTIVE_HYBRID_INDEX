# Adaptive Hybrid Index (AHI)

> An experimental self-adaptive indexing data structure that dynamically switches between internal representations based on observed workload patterns.

---

## Overview

The **Adaptive Hybrid Index (AHI)** is a prototype indexing system that automatically adapts its underlying data structure according to runtime access patterns.

Traditional software systems require developers to choose a data structure upfront:

* **Hash Tables** → Excellent for point lookups
* **Balanced Trees** → Efficient for range queries
* **Arrays** → Fast scans and cache-friendly traversal

The problem is that workloads often change over time, while the selected data structure remains fixed.

AHI explores a different approach:

> Instead of forcing the programmer to choose one data structure permanently, the index continuously monitors usage patterns and automatically migrates to the most suitable representation.

This project combines concepts from:

* Object-Oriented Programming (OOP)
* Data Structures & Algorithms (DSA)
* Cost Modeling
* Adaptive Systems
* Self-Optimizing Software

---

## Motivation

While solving DSA and system design problems, a common observation emerges:

| Data Structure | Strength                |
| -------------- | ----------------------- |
| Hash Table     | Fast key lookups        |
| Balanced BST   | Efficient range queries |
| Trie           | Prefix searches         |
| Arrays         | Sequential scans        |

No single structure performs best across all workloads.

For example:

### Lookup-Heavy Workload

```text
find(10)
find(25)
find(40)
find(90)
```

A hash table is ideal.

### Range-Heavy Workload

```text
range(100, 200)
range(250, 500)
range(1000, 2000)
```

A balanced tree becomes significantly more efficient.

Most systems choose one structure and never revisit that decision.

AHI investigates whether a data structure can:

1. Observe workload behavior
2. Estimate execution cost
3. Adapt automatically
4. Migrate its contents when beneficial

---

# Architecture

## 1. Abstract Backend Interface

All index implementations inherit from a common interface:

```cpp
struct IndexBackend
```

Supported operations:

| Operation    | Description                       |
| ------------ | --------------------------------- |
| insert(k, v) | Insert or update a key-value pair |
| find(k, v)   | Point lookup                      |
| range(l, r)  | Range query                       |
| dump_all()   | Export all entries for migration  |

This abstraction allows the adaptive layer to switch implementations without changing external behavior.

---

## 2. HashIndex

Implementation:

```cpp
unordered_map<int, int>
```

### Characteristics

| Operation   | Complexity   |
| ----------- | ------------ |
| Insert      | O(1) average |
| Find        | O(1) average |
| Range Query | O(n)         |

### Best For

* Lookup-heavy workloads
* Random access patterns
* Fast key retrieval

---

## 3. BSTIndex

Implementation:

```cpp
map<int, int>
```

Internally implemented as a balanced Red-Black Tree.

### Characteristics

| Operation   | Complexity   |
| ----------- | ------------ |
| Insert      | O(log n)     |
| Find        | O(log n)     |
| Range Query | O(log n + k) |

Where:

```text
k = number of returned elements
```

### Best For

* Ordered data
* Frequent range queries
* Sorted traversals

---

## 4. Workload Monitoring

AHI continuously tracks runtime behavior using:

```cpp
struct WorkloadStats
```

Metrics:

```cpp
point  -> find() operations
range  -> range() operations
```

### Exponential Decay

To prevent historical workloads from permanently influencing decisions:

```cpp
point *= 0.95;
range *= 0.95;
```

Recent operations therefore have a greater impact on future adaptation decisions.

---

## 5. Cost Model

AHI estimates the expected cost of each backend.

### Hash Table Cost

```text
Cost(Hash)
=
PointQueries × 1
+
RangeQueries × n
```

Reasoning:

* Point lookup ≈ O(1)
* Range query ≈ O(n)

### BST Cost

```text
Cost(BST)
=
PointQueries × log₂(n)
+
RangeQueries × log₂(n)
```

Reasoning:

* Point lookup ≈ O(log n)
* Range query start ≈ O(log n)

---

## 6. Adaptive Decision Engine

The adaptive layer evaluates costs after every operation:

```cpp
maybe_switch();
```

Current switching logic:

```cpp
if (cost_bst + migration_cost < cost_hash)
{
    switch_to_bst();
}
```

Migration cost is approximated as:

```text
O(n)
```

representing the cost of rebuilding the alternative structure.

---

## Data Migration

When a switch becomes beneficial:

1. Export all entries from the current backend.
2. Create a new backend.
3. Reinsert all elements.
4. Replace the old backend.

```cpp
dump_all()
```

is used to make migration backend-independent.

---

## Current Features

* Runtime workload monitoring
* Adaptive backend selection
* Hash Table implementation
* Balanced BST implementation
* Cost-based switching
* Exponential decay workload tracking
* Clean OOP architecture
* Backend abstraction through polymorphism
* Automatic data migration

---

## Example Workflow

### Initial State

```text
HashIndex
```

### Workload

```text
find()
find()
find()
find()
```

AHI remains on the Hash Table because lookups dominate.

### Workload Changes

```text
range()
range()
range()
range()
```

The estimated hash-table cost increases.

### Decision

```text
[COST] Hash=120
[COST] BST=35
[DECISION] Switching to BST
```

### New State

```text
BSTIndex
```

Future range queries now execute more efficiently.

---

## Project Structure

```text
AdaptiveHybridIndex
│
├── IndexBackend
│   ├── insert()
│   ├── find()
│   ├── range()
│   └── dump_all()
│
├── HashIndex
│   └── unordered_map
│
├── BSTIndex
│   └── std::map (Red-Black Tree)
│
├── WorkloadStats
│   └── Exponential Decay Tracking
│
└── AdaptiveHybridIndex
    ├── Cost Estimation
    ├── Decision Engine
    └── Data Migration
```

---

## Future Work

This prototype is intentionally designed to be extensible.

### Reinforcement Learning

* Replace rule-based switching
* Learn optimal policies from experience
* Adapt to more complex workloads

### Additional Data Structures

* Trie (prefix queries)
* B+ Trees
* Skip Lists
* Cache-friendly array indexes
* Compressed indexes

### Better Cost Models

* Cache behavior awareness
* Branch prediction effects
* Memory consumption
* Hardware performance counters

### System-Level Improvements

* Incremental migration
* Non-blocking switching
* Thread-safe implementations
* Lock-free variants
* Persistent indexes
* Versioned indexes

---

## Educational Value

This project serves as a practical demonstration of:

* Runtime Adaptation
* Strategy Pattern
* Polymorphism
* Dynamic Dispatch
* Cost Modeling
* Self-Tuning Systems
* Hybrid Data Structures

It is intended as both a learning project and a foundation for exploring adaptive indexing techniques used in modern databases and storage engines.

---

## Disclaimer

AHI is currently an experimental prototype intended for educational and research purposes. The current switching strategy is heuristic-based and simplified. Future versions aim to incorporate more realistic performance models and machine learning-based adaptation mechanisms.

---


Prototype project exploring adaptive data structures through Object-Oriented Programming, Data Structures & Algorithms, and self-optimizing system design.
