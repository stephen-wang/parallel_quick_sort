# <p style="text-align: center;">paralllel\_quick_sort</P>

parallel\_quick_sort (provided by Sorter::parallel_quick_sort()) is the multi\-threaded version implementation of traditional quick sort algorithm.The following test results is based on  [ctimeit](https://github.com/stephen-wang/ctimeit):<br> 

```
Parallel Sorting: data num <1000000>, thread num <4>, parallel_gate <5000>,  time consumption avg: 1s529ms245us (1529245000 ns)<br>
Traditional Quick Sorting: data num <1000000>, time consumption avg: 1s270ms986us (1270986000 ns)<br>

Parallel Sorting: data num <5000000>, thread num <4>, parallel_gate <5000>,  time consumption avg: 3s803ms176us (3803176000 ns)<br>
Traditional Quick Sorting: data num <5000000>, time consumption avg: 6s746ms215us (2451247704 ns)<br>

Parallel Sorting: data num <10000000>, thread num <4>, parallel_gate <5000>,  time consumption avg: 6s847ms559us (2552591704 ns)<br>
Traditional Quick Sorting: data num <10000000>, time consumption avg: 14s454ms724us (1569822112 ns)<br>
```
The code is currently intergrated as a Code::Blocks project, please ignore files（\*.cbp \*.layout \*.depend) if you don't use Code::Blocks.  
