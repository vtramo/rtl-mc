# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

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
