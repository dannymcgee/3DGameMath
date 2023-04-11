## What is this?

This repo is a small learning project that I am writing while working through [3D Math Primer for Graphics and Game Development](https://gamemath.com). It consists of the formulas and concepts in that book implemented as a C++17 library, with some unit tests, benchmarks, and a "sandbox" application for ad-hoc testing by printing stuff to the terminal.

## Why?

The goals of this project are basically to fill a couple of critical gaps in my knowledge and understanding re: gamedev:

### The Math

While I've been "hacking on" game development for a long time and have developed a certain level of intuition for the math involved as a result of lots of trial, error, and practice, my formal math education is relatively limited. I've never been great at committing formulas to memory or reasoning about math in the abstract.

My hope is that connecting the dots between those formulas and their practical application by writing an _actual program_ that _actually does stuff_ with them (and stepping outside of the abstractions provided by off-the-shelf game engines in doing so), I'll gain a much better foundation on which to reason about the problems that are commonly encountered in game development work.

### The Code

I've been writing C++ for a long time in the (very specific) context of Unreal Engine, in addition to taking a few beginner-to-intermediate courses on the language, but that experience has left a couple of vital areas unexplored:

* Projects, build systems, tooling, etc. This project is my first non-trivial experiment in spinning up a (non-Unreal Engine) C++ project from scratch, managing third-party dependencies, dealing with compilers and linters in an IDE-agnostic way, etc.
* Templates. Templates in C++ are substantially different than generics in virtually any other language I've used, so I wanted to get some practice in writing templated classes, functions, etc. (And incidentally, I _might_ have gone just a bit overboard in pursuit of that goal.)

## Licensing

Putting aside the obvious fact that you shouldn't use this code simply because there are innumerable better options available, the licensing issue is somewhat ambiguous. There are a few small samples here and there that are quite similar to code samples found in the book linked above, and several passages under the `notes` folder that were copied verbatim.

While all of this content is freely available at the link above, I could not find any particular license attached to it. So if, for whatever reason, you feel possessed to use any of the code here, you do so at your own risk. (I'll also go ahead and take this opportunity to politely request that the rights-holders of the book in question refrain from suing me &mdash; to the best of my knowledge, all of the material here is either my own work or fair use, but I am not a lawyer.)

## A Note on Style

In this codebase I have knowingly and willfully committed numerous crimes against style and formatting conventions that seem to be common in game development. (For example, perhaps owing to my background in TypeScript and Rust, I'm fond of the way trailing return types and `auto` declarations keep identifiers aligned to the left, and I find K&R braces aesthetically pleasing.)

However, if you happen to have stumbled across this codebase because you're considering me for employment, I'd just like to ask that you refrain from judging me too harshly for such vulgarities, and note that I'm perfectly capable of adapting to whatever style or conventions are expected of me in the context of a shared codebase or team environment. 🙂
