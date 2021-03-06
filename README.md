# Repository name: libecg-clr-interface

## Repo Description
Microsoft.NET Common Language Runtime (CLR) interface for libecg: A wrapper class for managed code.

## Dependency
This repository depends on [libecg](https://github.com/kamyar-nemati/libecg)
* To get _libecg_ sub-project, run the following commands:
    * `git submodule init`
    * `git submodule update`

_All headers and source files of libecg repo are expected to be included in the solution. Precompiled headers must be disabled for all the included source files to have a successful compilation._

## Design
Context-level Architectural Design

![Context-level Architectural Design](https://raw.githubusercontent.com/kamyar-nemati/libecg-clr-interface/trunk/architecture.png "Context-level Architectural Design")
