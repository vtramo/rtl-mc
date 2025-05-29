# Changelog
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-05-29
### :sparkles: New Features
- [`7407caf`](https://github.com/vtramo/rtl-mc/commit/7407caf93234d0e7b1d7c072601a5767b552008c) - **denot**: add `totalRedundantPaths` stats in `Denot` *(commit by [@vtramo](https://github.com/vtramo))*

### :recycle: Refactors
- [`de08664`](https://github.com/vtramo/rtl-mc/commit/de086640416ee838024c026fd0f3bb9e7f9754fe) - **automata**: replace 'AutomatonOptimizationFlags' with 'TranslationOptimizationFlags' *(commit by [@vtramo](https://github.com/vtramo))*

### :white_check_mark: Tests
- [`1e3f67a`](https://github.com/vtramo/rtl-mc/commit/1e3f67af5a09498bbeb54d08d74c8f7cbd6e890a) - add is compact and is non-recurrent test cases *(commit by [@vtramo](https://github.com/vtramo))*

### :wrench: Chores
- [`a8002d0`](https://github.com/vtramo/rtl-mc/commit/a8002d0ac9479d3bb485070a536e37f5e6a45765) - update experiment scripts *(commit by [@vtramo](https://github.com/vtramo))*
- [`f154199`](https://github.com/vtramo/rtl-mc/commit/f1541994dc6e96c5b4fa2caf77285e1090e725b3) - add must experiment *(commit by [@vtramo](https://github.com/vtramo))*
- [`5f8d848`](https://github.com/vtramo/rtl-mc/commit/5f8d848dc85543cba27409ac7973864ce52024fb) - add incremental tanks and alternations experiment *(commit by [@vtramo](https://github.com/vtramo))*
- [`e0acc5d`](https://github.com/vtramo/rtl-mc/commit/e0acc5da99e1d542cd7d9fd4f3b66f6971ea0e41) - add incremental gap experiment gbb files *(commit by [@vtramo](https://github.com/vtramo))*


## [0.1.7] - 2025-04-30
### :sparkles: New Features
- [`d629637`](https://github.com/vtramo/rtl-mc/commit/d6296370d7a71263da50cc19a19043ed60c58440) - **denot**: add debug assert to check for duplicate pairs in recursive call stack *(commit by [@vtramo](https://github.com/vtramo))*
- [`58d10e3`](https://github.com/vtramo/rtl-mc/commit/58d10e344d86d22f18f2dee558c9bebec952a3ad) - **utils**: add utility function to format current time as a string *(commit by [@vtramo](https://github.com/vtramo))*
- [`862442e`](https://github.com/vtramo/rtl-mc/commit/862442e8f2b96e3568df60c2c1c3a490f07a5acd) - **mcsolvers**: Add solver name parameter and logging enhancements *(commit by [@vtramo](https://github.com/vtramo))*
- [`32ca34f`](https://github.com/vtramo/rtl-mc/commit/32ca34f55837d4afbb65416aa5c508910737ae7f) - **denot**: Add `denot_path_tree.h` for building Denot path trees *(commit by [@vtramo](https://github.com/vtramo))*
- [`7c59f81`](https://github.com/vtramo/rtl-mc/commit/7c59f812228cfb05e56071af1fa4f63d1736028f) - add support for denot path tree export and enhance solver logging *(commit by [@vtramo](https://github.com/vtramo))*
- [`14c1c04`](https://github.com/vtramo/rtl-mc/commit/14c1c046f3722435d7651cd4a0659a01c5d6cc30) - **denot**: add duplicate pair check logic *(commit by [@vtramo](https://github.com/vtramo))*
- [`ed3debb`](https://github.com/vtramo/rtl-mc/commit/ed3debbdadc01b1c8eef0247f39ca59323239b35) - **automata**: add state denotation to state names in automata logging *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`2c3f314`](https://github.com/vtramo/rtl-mc/commit/2c3f314eb46dd9604694a710f8df131728fd2258) - **denot**: fix collect paths logic *(commit by [@vtramo](https://github.com/vtramo))*
- [`ab143fe`](https://github.com/vtramo/rtl-mc/commit/ab143fe113d134921864538f928fa42040c55e76) - **system**: throw `std::invalid_argument` when extending `PolyhedralSystem` with mismatched dimension interpretation *(commit by [@vtramo](https://github.com/vtramo))*
- [`53dea3a`](https://github.com/vtramo/rtl-mc/commit/53dea3a71c2de46cc2981ec7057598b1792eed68) - **automata**: add labels to dummy edges for improved visualization *(commit by [@vtramo](https://github.com/vtramo))*

### :wrench: Chores
- [`45fd041`](https://github.com/vtramo/rtl-mc/commit/45fd041898b0f1ecc5ed433e3c9dfbc4716caa0f) - add stl examples *(commit by [@vtramo](https://github.com/vtramo))*
- [`a49689a`](https://github.com/vtramo/rtl-mc/commit/a49689a404f2fe4240957f7d747f846ef229cb0b) - update solutions for incremental gap flow rectangle *(commit by [@vtramo](https://github.com/vtramo))*
- [`c62c7b5`](https://github.com/vtramo/rtl-mc/commit/c62c7b52ee792091eb69c46cfa9ded8d087f3349) - add denot path trees for incremental gap flow rectangle experiment *(commit by [@vtramo](https://github.com/vtramo))*
- [`6062b37`](https://github.com/vtramo/rtl-mc/commit/6062b37bd5a2f18756fade1f8b10fbdbdb66f2a8) - add dot/pdf files graph visualizations for denot (k=254) *(commit by [@vtramo](https://github.com/vtramo))*


## [0.1.6] - 2025-04-22
### :sparkles: New Features
- [`fa9b44c`](https://github.com/vtramo/rtl-mc/commit/fa9b44c1559e17788f37c82cf69e23c47008a998) - **sys-gen**: generate unique variable names for tank experiment *(commit by [@vtramo](https://github.com/vtramo))*
- [`2d39d95`](https://github.com/vtramo/rtl-mc/commit/2d39d95e3a27edae9331ead7469d7b9867316be6) - **denot**: add more stats for Denot + max iterations *(commit by [@vtramo](https://github.com/vtramo))*
- [`663684b`](https://github.com/vtramo/rtl-mc/commit/663684b07712c9a2cad28e3f09762919c8e550bc) - **mcsolver**: add `SolverResult` class *(commit by [@vtramo](https://github.com/vtramo))*
- [`4d02b5d`](https://github.com/vtramo/rtl-mc/commit/4d02b5d980ff6869ade1100ceda1d9394a6f66c1) - extract `SymbolTable` interface and refactor rational point parser *(commit by [@vtramo](https://github.com/vtramo))*
- [`accaf16`](https://github.com/vtramo/rtl-mc/commit/accaf16f48c54e3f5b35970b9ff92594a783a66f) - **poly-ggb**: add `--fixed` argument *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`90dd2b2`](https://github.com/vtramo/rtl-mc/commit/90dd2b290f1cbebe9d584268b925cbed5131e9ad) - **sys-gen**: fix `maxTime` flag *(commit by [@vtramo](https://github.com/vtramo))*
- [`010ea67`](https://github.com/vtramo/rtl-mc/commit/010ea67bc26a6fa4260f35f96990e0a42724fbec) - **emptiness**: fix emptiness algorithms *(commit by [@vtramo](https://github.com/vtramo))*
- [`ea1f46d`](https://github.com/vtramo/rtl-mc/commit/ea1f46de01a9d836a77923446da56a326561a290) - **mcsolvers**: fix universal denotation calculation *(commit by [@vtramo](https://github.com/vtramo))*
- [`a0acc6b`](https://github.com/vtramo/rtl-mc/commit/a0acc6baa57079e7845875bc0f9e158f4d2f4c81) - **poly-ggb**: avoid inconsistent constraints *(commit by [@vtramo](https://github.com/vtramo))*

### :wrench: Chores
- [`90a583f`](https://github.com/vtramo/rtl-mc/commit/90a583fb07afb72f9aa4bb63b97508dd79b4f322) - update gap omnidirectional inf semantics pdf result *(commit by [@vtramo](https://github.com/vtramo))*
- [`6b03252`](https://github.com/vtramo/rtl-mc/commit/6b032529ce05db0e09895b3e818cc4e3d6c993ea) - add incremental tanks script and results *(commit by [@vtramo](https://github.com/vtramo))*
- [`7a041bd`](https://github.com/vtramo/rtl-mc/commit/7a041bd661ee734f2850f458c362b846400d04e1) - update scripts and add flow pdfs *(commit by [@vtramo](https://github.com/vtramo))*
- [`6fdb6f0`](https://github.com/vtramo/rtl-mc/commit/6fdb6f0879252da3b029d92cdeb757f5106e551e) - add certificates and visualization for incremental gap flow *(commit by [@vtramo](https://github.com/vtramo))*
- [`396c1ab`](https://github.com/vtramo/rtl-mc/commit/396c1ab6726bb20be60e8dc574eb6a37997fff39) - add incremental tanks experiment results *(commit by [@vtramo](https://github.com/vtramo))*


## [0.1.5] - 2025-04-14
### :sparkles: New Features
- [`d75b7c5`](https://github.com/vtramo/rtl-mc/commit/d75b7c5a2a920fa87e865815ca738dd73b1be3f2) - **sys-gen**: add `sys-gen`, a tool for generating polyhedral systems *(commit by [@vtramo](https://github.com/vtramo))*
- [`3fa2d38`](https://github.com/vtramo/rtl-mc/commit/3fa2d38788477732f88dd56724a432115a4c845c) - **system**: add `setOutputExtraInformation` method in `PolyhedralSystem` *(commit by [@vtramo](https://github.com/vtramo))*
- [`f1166f9`](https://github.com/vtramo/rtl-mc/commit/f1166f91ffa30ca7f4d1ba773658f3ec8b108721) - **sys-gen**: add additional gap parameters *(commit by [@vtramo](https://github.com/vtramo))*
- [`376846f`](https://github.com/vtramo/rtl-mc/commit/376846f4f93736cbed9f209e651e6ae5ff527bca) - **sys-gen**: enriched atomic props p and q with even/odd tank sums, optional maxTime adds t0/t1 *(commit by [@vtramo](https://github.com/vtramo))*
- [`c48bafa`](https://github.com/vtramo/rtl-mc/commit/c48bafa63342b4750910139433af4fa365ace5e4) - **system**: add `getVariableNameBySpaceDimension` getter method in `PolyhedralSystemSymbolTable` *(commit by [@vtramo](https://github.com/vtramo))*
- [`042b510`](https://github.com/vtramo/rtl-mc/commit/042b510f891df6f81ccc743ddf9c797143cc121b) - **utils**: implement `std::hash` and custom equality for `PPL::Variable` *(commit by [@vtramo](https://github.com/vtramo))*
- [`529b298`](https://github.com/vtramo/rtl-mc/commit/529b29834441ba1ba1eaa41bd62a2aaa3484de89) - allow you to customize certain aspects of PPL output *(commit by [@vtramo](https://github.com/vtramo))*
- [`b6380d8`](https://github.com/vtramo/rtl-mc/commit/b6380d8af9c9a6e2e897c29840de14a742d69646) - **poly-ggb**: add `poly-ggb`, a tool to generate 2D Geogebra files from a polyhedra specification *(commit by [@vtramo](https://github.com/vtramo))*
- [`3620079`](https://github.com/vtramo/rtl-mc/commit/3620079ecfd5862c3127e479bc7269c916033729) - **automata**: introduce `AutomatonAcceptance` enum *(commit by [@vtramo](https://github.com/vtramo))*
- [`d52c3bc`](https://github.com/vtramo/rtl-mc/commit/d52c3bcecb1e25c55c6f6b1a36d142bceff5c91d) - **emptiness**: add ad-hoc emptiness algorithms *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`e361e75`](https://github.com/vtramo/rtl-mc/commit/e361e75afae9bfc1aad87cd6fa0146fa64503efd) - **automata**: fix `eraseInitialEdgesWithEmptyDenotation` method *(commit by [@vtramo](https://github.com/vtramo))*
- [`2f6482b`](https://github.com/vtramo/rtl-mc/commit/2f6482b7c1648bb8c7c629808a22b2a8f406a581) - **geometry**: fix `characteristicCone` function (add `is_line`) *(commit by [@vtramo](https://github.com/vtramo))*
- [`fae6f03`](https://github.com/vtramo/rtl-mc/commit/fae6f035f16a4a7bd32d36e2e985fad137f4ae54) - **denot**: return `reachPlus(X, Rn)` when is open initial state *(commit by [@vtramo](https://github.com/vtramo))*

### :recycle: Refactors
- [`c451492`](https://github.com/vtramo/rtl-mc/commit/c451492b089802b0a8fbec3619f3962a83591a07) - **system**: lazily compute flow cone *(commit by [@vtramo](https://github.com/vtramo))*
- [`dfd9486`](https://github.com/vtramo/rtl-mc/commit/dfd9486d01cb526747faadfe50031ab593280295) - enable constraint minimization based on flag *(commit by [@vtramo](https://github.com/vtramo))*
- [`73657e3`](https://github.com/vtramo/rtl-mc/commit/73657e36483a8efbf28a320a9a24aa5bf280bb42) - **sys-gen**: disable constraint minimization in default tank experiment *(commit by [@vtramo](https://github.com/vtramo))*

### :white_check_mark: Tests
- [`483dd32`](https://github.com/vtramo/rtl-mc/commit/483dd32967338a9b31015a6b4cf95c6ff378fb69) - add brink test cases *(commit by [@vtramo](https://github.com/vtramo))*
- [`32feeea`](https://github.com/vtramo/rtl-mc/commit/32feeea646d642a99c7d9f5e0ef0c84690be0c1e) - add finite automaton test case with `brink` proposition *(commit by [@vtramo](https://github.com/vtramo))*
- [`ae2a067`](https://github.com/vtramo/rtl-mc/commit/ae2a06729f03288dca7337e6ee00dd11f22bf7f9) - add more test cases for `PolyhedralSystemFormulaDenotationMap` *(commit by [@vtramo](https://github.com/vtramo))*
- [`485a1fa`](https://github.com/vtramo/rtl-mc/commit/485a1fa8e1d2872696d856f44024141b65afe0cc) - add more test cases for `removeSing` function *(commit by [@vtramo](https://github.com/vtramo))*

### :wrench: Chores
- [`edb2575`](https://github.com/vtramo/rtl-mc/commit/edb2575e18dc3dbab969efce7c6ee8fe5b963048) - add experiment results *(commit by [@vtramo](https://github.com/vtramo))*


## [0.1.4] - 2025-04-07
### :sparkles: New Features
- [`1e745bb`](https://github.com/vtramo/rtl-mc/commit/1e745bb955979166869cd5c0c292da61aff34f91) - **geometry**: add `isClosed` function that checks if a `Powerset` is closed *(commit by [@vtramo](https://github.com/vtramo))*
- [`31ccefb`](https://github.com/vtramo/rtl-mc/commit/31ccefbf0e612261fb7659bffd3b92060c86c0d1) - **geometric**: add `coneGeometric` and `coneGenerator` functions *(commit by [@vtramo](https://github.com/vtramo))*
- [`1983015`](https://github.com/vtramo/rtl-mc/commit/1983015651ff3200a6937610b4f749a226af50aa) - **system**: add `hasFlowWithClosedCone()` method in `PolyhedralSystem` *(commit by [@vtramo](https://github.com/vtramo))*
- [`42428f9`](https://github.com/vtramo/rtl-mc/commit/42428f9c50692fca47a3b45b4cf4d30f18303234) - apply `fastPairwiseReduce` on final result *(commit by [@vtramo](https://github.com/vtramo))*
- [`b1415e1`](https://github.com/vtramo/rtl-mc/commit/b1415e1c340b4c8c3b96e8d27336b74ba10bae69) - **automata**: add `top()` factory method in `StateDenotation` *(commit by [@vtramo](https://github.com/vtramo))*
- [`871941e`](https://github.com/vtramo/rtl-mc/commit/871941e4a54d18bbdade27eeeab3ecb50beea326) - **automata**: add `filterEdge` and `isAcceptingEdge` virtual methods in `PolyhedralLtlAutomaton` *(commit by [@vtramo](https://github.com/vtramo))*
- [`5086362`](https://github.com/vtramo/rtl-mc/commit/5086362d6b4d60cb7cfdd4551da8ea70c7299039) - **logging**: log full automaton at debug level *(commit by [@vtramo](https://github.com/vtramo))*
- [`de643b2`](https://github.com/vtramo/rtl-mc/commit/de643b2df365a6b57aa31caf201ee8155338e105) - **logging**: log accepting runs at debug level (infinite-time semantics omnidirectional case) *(commit by [@vtramo](https://github.com/vtramo))*
- [`302ff56`](https://github.com/vtramo/rtl-mc/commit/302ff56ce0bdb27d72a0bb7e8628f4135e160af2) - **abstractions**: add `polyhedralSystem()` method in `PolyhedralAbstraction` *(commit by [@vtramo](https://github.com/vtramo))*
- [`6490a75`](https://github.com/vtramo/rtl-mc/commit/6490a75933565ea0f3865c06122ad9937ff79c87) - **logging**: log full `PolyhedralSynchronousProductAutomaton` at debug level *(commit by [@vtramo](https://github.com/vtramo))*
- [`d3e52eb`](https://github.com/vtramo/rtl-mc/commit/d3e52ebfc4958a505e5759d43c01e81291936faa) - **mcsolvers**: add `EmptinessCheckAlgorithm` enumeration *(commit by [@vtramo](https://github.com/vtramo))*
- [`ba58508`](https://github.com/vtramo/rtl-mc/commit/ba58508f850764395e35f61f40ab1bf006749522) - **mcsolvers**: collect initial states and measure elapsed time in seconds in `emptinessCheckDenotationSearch` *(commit by [@vtramo](https://github.com/vtramo))*

### :bug: Bug Fixes
- [`be5749f`](https://github.com/vtramo/rtl-mc/commit/be5749fe9e2c023a9a8d226ab523a841af162288) - **abstractions**: return `bdd_false()` when `Observable` is empty *(commit by [@vtramo](https://github.com/vtramo))*
- [`3f9fd3b`](https://github.com/vtramo/rtl-mc/commit/3f9fd3b0f3eca91c9cc73f9e3ddf0a4cd8823b21) - use `isClosed` instead of `is_topologically_closed` (from PPL) *(commit by [@vtramo](https://github.com/vtramo))*
- [`aa27f6f`](https://github.com/vtramo/rtl-mc/commit/aa27f6f414e1737ef5de2d1923474e844439c916) - set `bdd_true()` if labels are empty in sync product or abstraction *(commit by [@vtramo](https://github.com/vtramo))*
- [`9c8b8bc`](https://github.com/vtramo/rtl-mc/commit/9c8b8bc7602a4bbdbc6b6bc5c60fa4b1f6361a96) - **product**: handle self-loop edges labeled with `bdd_false()` *(commit by [@vtramo](https://github.com/vtramo))*

### :zap: Performance Improvements
- [`047ade1`](https://github.com/vtramo/rtl-mc/commit/047ade186125ce6db6da102026dd24d00d91871a) - **automata**: improve formula denotation calculation *(commit by [@vtramo](https://github.com/vtramo))*

### :recycle: Refactors
- [`99151ad`](https://github.com/vtramo/rtl-mc/commit/99151ad1824fd18df3d87520acd34af80b567cb2) - **automata**: change acceptance condition and kill accepting states at the end *(commit by [@vtramo](https://github.com/vtramo))*
- [`1f60ac2`](https://github.com/vtramo/rtl-mc/commit/1f60ac20f9d8f087b1ce5ccfdee9a2a63b74b688) - **cli**: separate input options for polyhedral system and formula *(commit by [@vtramo](https://github.com/vtramo))*
- [`93e95b4`](https://github.com/vtramo/rtl-mc/commit/93e95b46084bdbab4cc24a17ed5d79ba8de681ad) - **mcsolvers**: move emptiness algorithms to the `emptiness` package *(commit by [@vtramo](https://github.com/vtramo))*

### :white_check_mark: Tests
- [`1c3bbd6`](https://github.com/vtramo/rtl-mc/commit/1c3bbd6443a3bb1f23ee71dc71c8e3ccb60acdeb) - **geometry**: test `isClosed` function *(commit by [@vtramo](https://github.com/vtramo))*
- [`f0723e3`](https://github.com/vtramo/rtl-mc/commit/f0723e3cb3525a730bcc6d41c8bdf6d4c0ab2d85) - **geometric**: test `coneGeometric` and `coneGenerator` functions *(commit by [@vtramo](https://github.com/vtramo))*
- [`7903a13`](https://github.com/vtramo/rtl-mc/commit/7903a1377254e0743d06e6c1fcb08dcb7dcf940b) - **omnidirectional**: add `isOmnidirectional` test case *(commit by [@vtramo](https://github.com/vtramo))*

### :wrench: Chores
- [`257d9a0`](https://github.com/vtramo/rtl-mc/commit/257d9a02fcdaddd5e5ffdd9f10ba7c02898dc4f5) - add experiment scripts *(commit by [@vtramo](https://github.com/vtramo))*


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
[0.1.4]: https://github.com/vtramo/rtl-mc/compare/0.1.3...0.1.4
[0.1.5]: https://github.com/vtramo/rtl-mc/compare/0.1.4...0.1.5
[0.1.6]: https://github.com/vtramo/rtl-mc/compare/0.1.5...0.1.6
[0.1.7]: https://github.com/vtramo/rtl-mc/compare/0.1.6...0.1.7
[1.0.0]: https://github.com/vtramo/rtl-mc/compare/0.1.7...1.0.0
