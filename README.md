<a name="readme-top"></a>
<br />
<h1 align="center">DJC - Diminished Java Compiler</h1>

  <p align="center">
    A straightforward compiler for a simple, but complete Object Oriented Programming Language
  </p>

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
    </li>
    <li><a href="#usage">Usage</a></li>
  </ol>
</details>

<!-- ABOUT THE PROJECT -->
## About The Project
DJC is a compiler built for Diminished Java (DJ), a programming language developed by Dr. Jay Ligatti at the University of South Florida. DJC converts source programs written in DJ, to target code written in the simplified instruction set architecture DISM. DJC ensures that the program follows DJ sytax, is well-typed and also generates code to perform dynamic runtime checks to limit errors.

A detailed description of DJ can be found here: https://cse.usf.edu/~ligatti/compilers/24/as1/dj/DJ-definition.pdf

A deatialed description of DISM can be found here: https://cse.usf.edu/~ligatti/compilers/24/as1/dism/DISM-definition.pdf

Also included are some test cases used when building the compiler. These example DJ programs show the sophistication of the compiler to recognize lexical errors, syntax errors and type errors. The good examples also show the kinds of programs that can be written in DJ.

### Built With
* C
* FLEX
* BISON

<!-- GETTING STARTED -->
## Getting Started
Since the source files for the compiler are part of the USF course curriculum, they are not directly available in this repo. In order to access all the source files for DJC, request access using Google drive. Otherwise, download the executable for DJC as well as all the files to run the DISM interpreter.


<!-- USAGE EXAMPLES -->
## Usage
To build the DISM interpreter
```sh
flex dism.l
bison dism.y
sed -i '/extern YYSTYPE yylval/d' dism.tab.c
gcc dism.tab.c ast.c interp.c -o sim-dism
```

To build the DJC executable
```sh
flex dj.l
bison -v dj.y
sed -i '/extern YYSTYPE yylval/d' dj.tab.c
gcc dj.tab.c ast.c symtbl.c typecheck.c codegen.c -o djc 
```
After generating the executable for the compiler as well as the DISM simulator, compile the test case programs using
```sh
djc filename.dj
```

DJC will create the target file ```filename.dism``` given the source file ```filename.dj```. Run this target file using the DISM interpreter
```sh
./sim-dism filename.dism
```


 
