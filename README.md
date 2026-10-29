# C++ Data Structures Projects

Academic projects implementing custom data structures in **C++11**.

## World Cup Management — `mevni3/`

A team and player management system built around custom AVL trees. The API supports adding and removing teams and players, updating statistics, playing matches, merging teams, top-scorer queries, and knockout-range queries.

- `AVLTree.h`: generic balanced search tree.
- `worldcup23a1.h` / `.cpp`: management API and implementation.
- `Team` / `Player`: domain models.
- `WorldCupTests.cpp`: bundled Catch test suite.

### Build and run

Requires a C++11 compiler and CMake 3.19 or newer:

```sh
cmake -S mevni3 -B build
cmake --build build
./build/Wet1
```

These commands use the existing CMake target. The test suite has not been executed as part of this documentation update.

## Car Agency Management — `mevni22/`

A second project combining a ranked AVL tree (`AVLRankTree.h`) with union-find (`UnionFindAgency`) to manage car agencies and sales. The external interface is in `library.h`.

This folder provides library sources and does not include a standalone test executable or build configuration.

## Scope

This repository preserves coursework implementations and bundled test/support code. It demonstrates balanced trees, ranking, disjoint sets, and C++ object-oriented design; it is not a production service.
