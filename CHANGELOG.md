# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.1.3] - 2025-03-31
### :sparkles: New Features
- [`dbc3578`](https://github.com/vtramo/rtl-mc/commit/dbc35782d99cb0a64d6ba98075a428de90812d06) - **cli**: add option `--export-dot` to export graph dot files *(commit by [@vtramo](https://github.com/vtramo))*
- [`3106730`](https://github.com/vtramo/rtl-mc/commit/3106730d64a71045583615d0c1264140573b4739) - **rtl-gen**: add `rtl-gen` tool for generating RTL formulae *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`e5fb512`](https://github.com/vtramo/rtl-mc/commit/e5fb512f5197fbcfa392578a635036ccf51e93b0) - **automata**: replace `!sing` with `true` in state denotation *(commit by [@vtramo](https://github.com/vtramo))*

### :recycle: Refactors
- [`c4b5abc`](https://github.com/vtramo/rtl-mc/commit/c4b5abccd12923636fdccf0ec620053eb348e447) - **system**: store observables after generation *(commit by [@vtramo](https://github.com/vtramo))*


## [0.1.2] - 2025-03-29
### :sparkles: New Features
- [`fadd7b4`](https://github.com/vtramo/rtl-mc/commit/fadd7b4f8b4f50e4ce8588aaebdf730405a97b90) - **abstraction**: add setInitialState method in PolyhedralAbstraction *(commit by [@vtramo](https://github.com/vtramo))*
- [`f768fa6`](https://github.com/vtramo/rtl-mc/commit/f768fa677214163395c7272f25e7361db1d21991) - **automata**: add get name method in Automaton *(commit by [@vtramo](https://github.com/vtramo))*
- [`fd922b8`](https://github.com/vtramo/rtl-mc/commit/fd922b8ffe3bfeee6c6d9e03ccd9109b8e32b18a) - add hash function for spot::atomic_prop_set *(commit by [@vtramo](https://github.com/vtramo))*
- [`3dad2b6`](https://github.com/vtramo/rtl-mc/commit/3dad2b6a707aea91140879616fd95d60b24c0176) - **system**: add `hasCompactFlow` and `hasCompactInvariant` methods in `PolyhedralSystem` *(commit by [@vtramo](https://github.com/vtramo))*
- [`8905ce9`](https://github.com/vtramo/rtl-mc/commit/8905ce98433c344caf7dc4ce7466735b65403aec) - **system**: add `extend` method in `PolyhedralSystem` *(commit by [@vtramo](https://github.com/vtramo))*
- [`2b68e6c`](https://github.com/vtramo/rtl-mc/commit/2b68e6cc48e5c734425366815c0c1e68f2089851) - **automata**: add `postprocessAutomaton` virtual method *(commit by [@vtramo](https://github.com/vtramo))*
- [`216aa9a`](https://github.com/vtramo/rtl-mc/commit/216aa9af613051538d4415e65d73cbc89ba8b302) - **system**: add `generateEmptyObservable` method in `PolyhedralSystem` *(commit by [@vtramo](https://github.com/vtramo))*
- [`351a93a`](https://github.com/vtramo/rtl-mc/commit/351a93a9dd65c95c82bfd1119f7dece3b22c1472) - **cli**: refactor cli and separate system input from formula input *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`e73bdda`](https://github.com/vtramo/rtl-mc/commit/e73bdda4115b4580d1a7719a7a3f37981a92d4cb) - **tile**: replace TileExtractorComplex with TileExtractorGraph due to bugs *(commit by [@vtramo](https://github.com/vtramo))*
- [`8c68de5`](https://github.com/vtramo/rtl-mc/commit/8c68de56370576067c9d0ba6b318b24b42add7c8) - Non-Recurrent RTL Formula and COMPACT Flow *(commit by [@vtramo](https://github.com/vtramo))*
- [`171a1bb`](https://github.com/vtramo/rtl-mc/commit/171a1bbae8aca32e4970e66da75a9dd7e3a100c0) - **stay**: fix stay interpretation when 0 belongs to Flow *(commit by [@vtramo](https://github.com/vtramo))*
- [`9e088d3`](https://github.com/vtramo/rtl-mc/commit/9e088d36a00b499dbd248c250965188eda932b92) - **system**: include the empty observable when generating observables *(commit by [@vtramo](https://github.com/vtramo))*

### :recycle: Refactors
- [`121fb48`](https://github.com/vtramo/rtl-mc/commit/121fb48ef9807863fb78dcb974e7d861ab84cc4b) - **utils**: rename `combination` to `simpleCombinations` and improve documentation *(commit by [@vtramo](https://github.com/vtramo))*
- [`eadc04a`](https://github.com/vtramo/rtl-mc/commit/eadc04a7d29cefff1d733c0d247356b10eebd196) - **system**: simplify observables generation by using bitwise operations *(commit by [@vtramo](https://github.com/vtramo))*
- [`9332e75`](https://github.com/vtramo/rtl-mc/commit/9332e75f1a8dccf3836d2d9af0343b73f64a806d) - **cone**: rename `cone` to `characteristicCone` *(commit by [@vtramo](https://github.com/vtramo))*
- [`1276294`](https://github.com/vtramo/rtl-mc/commit/127629423fb1a81750f62c3716b1c1f5250eca0c) - **tile**: delete TileExtractorList and rename TileExtractorList to TileExtractorDoublyLinkedList *(commit by [@vtramo](https://github.com/vtramo))*
- [`22a4255`](https://github.com/vtramo/rtl-mc/commit/22a4255ad77b3a5e4912c45b6f469251d0f12e1e) - **abstractions**: rename ObservableTraversalNode to ObservableTraverseNode *(commit by [@vtramo](https://github.com/vtramo))*

### :white_check_mark: Tests
- [`bfca2f1`](https://github.com/vtramo/rtl-mc/commit/bfca2f1db91b0490aae1f477fa798d5d2bd9cd7d) - **tile**: add a new tilextractor test case and benchmark *(commit by [@vtramo](https://github.com/vtramo))*
- [`312051a`](https://github.com/vtramo/rtl-mc/commit/312051a4d4471fac4914b90742e56a38726cb9b1) - **tile**: add new test cases and benchmarks *(commit by [@vtramo](https://github.com/vtramo))*
- [`6c90967`](https://github.com/vtramo/rtl-mc/commit/6c9096726514ab64a98a679c2b0af536f839caf0) - **automata**: add simple test case for BackwardNFA *(commit by [@vtramo](https://github.com/vtramo))*


## [0.1.1] - 2025-03-24
### :sparkles: New Features
- [`f29c1c3`](https://github.com/vtramo/rtl-mc/commit/f29c1c3faf103a13ed9144540504435713a6ef69) - refactor and expand execution statistics formatting with placeholders *(commit by [@vtramo](https://github.com/vtramo))*
- [`e642367`](https://github.com/vtramo/rtl-mc/commit/e642367cc6cc4687d92c2ea188be1467c598da22) - **tile**: add TileExtractorGraph version *(commit by [@vtramo](https://github.com/vtramo))*
- [`4534949`](https://github.com/vtramo/rtl-mc/commit/45349499c586297f1189d35ec1aae04aedf3738b) - **logging**: add verbosity for polyhedral abstraction and synchronous product *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`b4575f9`](https://github.com/vtramo/rtl-mc/commit/b4575f9282aaccc52598e389f1a2ac497d307cbc) - **abstractions**: set PolyhedralAbstraction space dimension *(commit by [@vtramo](https://github.com/vtramo))*
- [`6b4bbe3`](https://github.com/vtramo/rtl-mc/commit/6b4bbe384f86aa286b224b94dbaa1cd38a392065) - **abstractions**: reorder discretisation and automaton construction in GeneralFiniteSolver *(commit by [@vtramo](https://github.com/vtramo))*
- [`c6e637a`](https://github.com/vtramo/rtl-mc/commit/c6e637ae066009c181d94885a5950a66f23783d4) - **traverse**: resolve issue with TraversePlus and update test cases *(commit by [@vtramo](https://github.com/vtramo))*
- [`1e0853c`](https://github.com/vtramo/rtl-mc/commit/1e0853cfe7a6c0c7cee1f39037ba2a2cba77d7a3) - **mcsolvers**: prevent SEGFAULT by ensuring discretisation occurs before automaton construction *(commit by [@vtramo](https://github.com/vtramo))*
- [`0e35b7b`](https://github.com/vtramo/rtl-mc/commit/0e35b7baac5c40699024211876a5f0f0fc908d62) - **cli**: correct automata optimization flags (--high, --low, --medium) *(commit by [@vtramo](https://github.com/vtramo))*
- [`64d9189`](https://github.com/vtramo/rtl-mc/commit/64d91899831d41a01842ddeea0adfa6e5adda914) - **mcsolvers**: correct finite-time semantics DFS *(commit by [@vtramo](https://github.com/vtramo))*

### :recycle: Refactors
- [`c806870`](https://github.com/vtramo/rtl-mc/commit/c80687087c573400bd5a0a5dc71ddc627e18802b) - **tile**: switch to TileExtractorComplex (the most efficient implementation) *(commit by [@vtramo](https://github.com/vtramo))*
- [`6ade007`](https://github.com/vtramo/rtl-mc/commit/6ade007b18228a9e49a42a272e5494011d8c3bf0) - refactoring: move formula functions from spot-utils to formula package *(commit by [@vtramo](https://github.com/vtramo))*
- [`f74372d`](https://github.com/vtramo/rtl-mc/commit/f74372d57936dfbf298552534e1c338e7be99e4c) - **automata**: extract AutomatonOptimizationLevel enum *(commit by [@vtramo](https://github.com/vtramo))*
- [`f93833b`](https://github.com/vtramo/rtl-mc/commit/f93833b5ef581a958eb83f38a12d45af8c97f542) - **system**: create "instances" package and move polyhedral system examples *(commit by [@vtramo](https://github.com/vtramo))*

### :white_check_mark: Tests
- [`5a6b301`](https://github.com/vtramo/rtl-mc/commit/5a6b30192f5526d33df88996b915bc0ced62b1c3) - **tile**: add tilextractor benchmarks *(commit by [@vtramo](https://github.com/vtramo))*
- [`8fa4d32`](https://github.com/vtramo/rtl-mc/commit/8fa4d325aa040a5555914c72fbce675a39650541) - **tile**: add essential test case for TileExtractor algorithms *(commit by [@vtramo](https://github.com/vtramo))*
- [`59f60ef`](https://github.com/vtramo/rtl-mc/commit/59f60ef7e55e5891189be68755ff4bc1df21eaee) - **tile**: add additional test cases for TileExtractorGraph *(commit by [@vtramo](https://github.com/vtramo))*
- [`f9257fe`](https://github.com/vtramo/rtl-mc/commit/f9257fe45227b9e7b66c02390bdddbe1118adb94) - **traverse**: add empty test case *(commit by [@vtramo](https://github.com/vtramo))*

[0.1.1]: https://github.com/vtramo/rtl-mc/compare/0.1.0...0.1.1
[0.1.2]: https://github.com/vtramo/rtl-mc/compare/0.1.1...0.1.2
[0.1.3]: https://github.com/vtramo/rtl-mc/compare/0.1.2...0.1.3
